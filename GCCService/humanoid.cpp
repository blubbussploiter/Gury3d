#include "humanoid.h"
#include "rbxmath.h"

#include "camera.h"

#include "sounds.h"
#include "ray.h"

#include "players.h"
#include "jointservice.h"

#include "GuiRoot.h"
#include "bthelp.h"

RBX::Sound* whoosh = RBX::Sound::fromFile(GetFileInPath("/content/sounds/button.wav"));
RBX::Sound* uuhhh = RBX::Sound::fromFile(GetFileInPath("/content/sounds/uuhhh.wav"));
RBX::Sound* bsls_steps = RBX::Sound::fromFile(GetFileInPath("/content/sounds/bfsl-minifigfoots1.mp3"), 1);

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::Humanoid>("Humanoid")
        .constructor<>()
        .property("Health", &RBX::Humanoid::health)(rttr::metadata("Type", RBX::Game))
        .property("health", &RBX::Humanoid::health)(rttr::metadata("Type", RBX::Game));
}

void setLocalTransparency(RBX::PVInstance* part, float transparency)
{
    part->alpha = 1 - transparency;
    part->localTransparency = transparency;
}

RBX::Humanoid* RBX::Humanoid::modelIsCharacter(RBX::Instance* testModel)
{
    if (testModel)
        return testModel->findFirstChildOfClass<RBX::Humanoid>("Humanoid");
    else
        return 0;
}

void RBX::Humanoid::setLocalTransparency(float transparency)
{
    ::setLocalTransparency(humanoidHead, transparency);
    ::setLocalTransparency(humanoidRootPart, transparency);
    ::setLocalTransparency(getRightArm(), transparency);
    ::setLocalTransparency(getLeftArm(), transparency);
    ::setLocalTransparency(getRightLeg(), transparency);
    ::setLocalTransparency(getLeftLeg(), transparency);
}

void RBX::Humanoid::setHumanoidAttributes()
{
    if (!getParent())
        return;

    humanoidHead = static_cast<RBX::PVInstance*>(getParent()->findFirstChild("Head"));
    humanoidRootPart = static_cast<RBX::PVInstance*>(getParent()->findFirstChild("Torso"));

}

void RBX::Humanoid::correctHumanoidAttributes()
{
    if (checkHumanoidAttributes())
        return;

    setHumanoidAttributes();
}

void RBX::Humanoid::buildJoints()
{
    if (!checkHumanoidAttributes())
        correctHumanoidAttributes();

    snap(humanoidHead, humanoidRootPart);

    if (getRightLeg())
        snap(humanoidRootPart, getRightLeg());

    if (getLeftLeg())
        snap(humanoidRootPart, getLeftLeg());

    if (getRightArm())
        snap(humanoidRootPart, getRightArm());

    if (getLeftArm())
        snap(humanoidRootPart, getLeftArm());

    humanoidRootPart->body->_body->setRestitution(0.0f);
    humanoidRootPart->body->_body->setSleepingThresholds(0.1f, 0.1f);
}

bool RBX::Humanoid::isFalling()
{
    btVector3 lin;
    if (!checkHumanoidAttributes())
        return 0;
    lin = humanoidRootPart->body->_body->getLinearVelocity();
    return !(lin.y() >= -0.1);
}

bool RBX::Humanoid::isInAir()
{
    btVector3 lin;
    if (!checkHumanoidAttributes())
        return 0;
    lin = humanoidRootPart->body->_body->getLinearVelocity();
    return (lin.y() > 0.1);
}

bool RBX::Humanoid::isJoined()
{
    return (checkHumanoidAttributes() && RBX::JointService::singleton()->areJointed(humanoidHead, humanoidRootPart));
}

bool RBX::Humanoid::isGrounded()
{

    if (rightLeg && leftLeg)
    {
        bool bottomCollide = rightLeg->body->isColliding(BOTTOM) || leftLeg->body->isColliding(BOTTOM);
        bool backCollide = rightLeg->body->isColliding(BACK) || leftLeg->body->isColliding(BACK);
        bool frontCollide = rightLeg->body->isColliding(FRONT) || leftLeg->body->isColliding(FRONT);
        return bottomCollide || frontCollide || backCollide;
    }

    return 0;
}

void RBX::Humanoid::setWalkDirection(Vector3 value)
{
    double x, z;
    double v4;

    if (value != walkDirection)
    {
        if (value == G3D::Vector3::zero())
        {
            walkDirection = value;
            walkMode = DIRECTION_STAY;
        }
        else
        {
            x = value.x;
            z = value.z;

            v4 = 1.0 / sqrt(z * z + x * x);
            walkDirection.x = x * v4;
            walkDirection.y = 0;
            walkDirection.z = z * v4;
            walkDirection *= 14;
            walkMode = DIRECTION_MOVE;
        }
    }
}

void RBX::Humanoid::setJump(bool jump)
{
    isJumping = jump;
}

void RBX::Humanoid::climb()
{
    btDynamicsWorld* btWorld = RBX::RunService::singleton()->getPhysics()->_world;

    Vector3 look = getRightLeg()->getCFrame().lookVector();
    Vector3 pos = getRightLeg()->getPosition();
    Vector3 to = pos + look;

    btVector3 btFrom(pos.x, pos.y, pos.z);
    btVector3 btTo(to.x, to.y, to.z);

    btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

    btWorld->rayTest(btFrom, btTo, res);

    if (res.hasHit())
    {
        btVector3 world = res.m_hitPointWorld;
        const btCollisionObject* obj = res.m_collisionObject;

        isClimbing = (world.distance(btFrom) < 1);
        if (obj)
        {
            const btCollisionShape* shape = obj->getCollisionShape();
            if (shape)
            {
                btVector3 min, max;
                shape->getAabb(obj->getWorldTransform(), min, max);
                climbHeight = max.y() - min.y();
            }
        }
    }
    else
    {
        humanoidState = Running;
        isClimbing = (elapsedClimb >= climbHeight);
    }

}

void RBX::Humanoid::onDied()
{
    if (isDead)
        return;

    isDead = 1;
    uuhhh->play();
}

void RBX::Humanoid::computeFallLumbedo()
{
    btVector3 lin;
    float y;

    lin = humanoidRootPart->body->_body->getLinearVelocity();
    y = lin.y();

    humanoidRootPart->body->_body->setRestitution(0.05f * (abs(y) / 2));
}

void RBX::Humanoid::turn()
{

    PhysBody* body = humanoidRootPart->body;
    btVector3 angularVelocity = body->_body->getAngularVelocity();

    /* actual stuff */

    Vector3 lookVector = humanoidRootPart->getCFrame().lookVector();
    RBX::Camera* camera = RBX::Camera::singleton();

    Vector3 unit = Vector3::unitY();
    float rad;

    camera->cframe.rotation.toAxisAngle(unit, rad);
    lookVector.unitize(0.000001f);

    float fTolerance = Math::angleToE0(lookVector);
    float lookAng = Math::radWrap(fTolerance);
    float fTolerancea = Math::radWrap(rad) - lookAng;
    float v8 = Math::radWrap(fTolerancea);

    if (fabs(v8) < 0.1)
        v8 = 0.0;
    
    if (angularVelocity.length() < 2)
    {
       // body->_body->setAngularVelocity(btVector3(0, v8, 0));
    }
}

void RBX::Humanoid::onStep()
{
    if (!checkHumanoidAttributes())
    {
        correctHumanoidAttributes();
        return;
    }

    if (!health)
        onDied();

    if (!jointsBuilt)
    {
        buildJoints();
        jointsBuilt = 1;
    }

    if (!isJoined())
    {
        health = 0;
    }

    updateHumanoidState();

    btRigidBody* body = humanoidRootPart->body->_body;

    switch (walkMode)
    {
        case DIRECTION_MOVE:
        {
            if (body)
            {
                btVector3 velocity = body->getLinearVelocity();
                float speed = velocity.length();

                if (humanoidState == Jumping)
                {
                    break;
                }

                body->activate(1);

                if (humanoidState == Running || isClimbing) /* only play when on ground, but do movement off ground too :) */
                {
                    if (!bsls_steps->isPlaying())
                        bsls_steps->play();

                    humanoidState = Strafing;

                    if (speed < 6.56)
                    {
                        btVector3 strafeVelocity(walkDirection.x, velocity.y(), walkDirection.z);
                        body->setLinearVelocity(strafeVelocity);
                        turn();
                    }

                }

                climb();

            }
            break;
        }
        case DIRECTION_STAY:
        {
            bsls_steps->stop();
            break;
        }
    }

    if (isJumping)
    {
        btRigidBody* body = humanoidRootPart->body->_body;
        if (body)
        {
            btVector3 velocity = body->getLinearVelocity();
            btVector3 newVelocity = btVector3(velocity.x(), 12, velocity.z());

            if (isGrounded())
            {
                humanoidState = Jumping;

                whoosh->play();
                bsls_steps->stop();

                body->activate(1);
                body->setLinearVelocity(newVelocity);
            }
        }
        isJumping = 0;
    }
    
}

void RBX::Humanoid::updateHumanoidState()
{
    if (isGrounded())
    {
        humanoidState = Running;
    }

    if (!isGrounded() && isFalling())
    {
        if (humanoidState != Jumping)
        {
            computeFallLumbedo();
            humanoidState = Falling;
        }
    }

    if (isGrounded() && (humanoidState == Falling || humanoidState == Jumping))
    {
        humanoidState = Landed;
    }
}

void RBX::Humanoid::render(RenderDevice* rd)
{
    GFontRef fnt;
    RBX::Network::Player* localPlayer;

    fnt = Gui::singleton()->font;
    localPlayer = RBX::Network::getPlayers()->localPlayer;

    if (!checkHumanoidAttributes())
    {
        correctHumanoidAttributes();
        return;
    }

    if (localPlayer && localPlayer->character == getParent()) return;

    if (!fnt.isNull()) 
        /* 
        * thanks
        https://github.com/Vulpovile/Blocks3D/blob/a75f7bf381c6714b8c55bb2e72acfb9077c29a08/src/source/DataModelV2/PartInstance.cpp#L114
        */
    {
        Vector3 position = humanoidHead->getPosition();
        Vector3 gamepoint = position + Vector3(0, 1.5, 0);
        Vector3 camerapoint = rd->getCameraToWorldMatrix().translation;
        float distance = pow(pow((double)gamepoint.x - (double)camerapoint.x, 2) + pow((double)gamepoint.y - (double)camerapoint.y, 2) + pow((double)gamepoint.z - (double)camerapoint.z, 2), 0.5);
        if (distance < 100 && distance > -100)
        {
            if (distance < 0)
                distance = distance * -1;

            glDisable(GL_DEPTH_TEST);

            CoordinateFrame pos(rd->getCameraToWorldMatrix().rotation, gamepoint);

            fnt->draw3D(rd, getParent()->name, pos, 0.03 * distance, Color3::white(), Color3::black(), G3D::GFont::XALIGN_CENTER, G3D::GFont::YALIGN_CENTER);
            drawHealthbar(rd, pos, distance);

            glEnable(GL_DEPTH_TEST);

        }
    }
}

void RBX::Humanoid::drawHealthbar(RenderDevice* rd, CoordinateFrame center, float distance)
{
    float f0 = (distance * 4);

    float width = f0 * (health / maxHealth) * 0.032f;
    float height = distance * 0.015f;

    float x = (distance) * -0.065f;
    float y = (distance) * -0.04f;

    Rect2D green, red;

    rd->setObjectToWorldMatrix(center);

    green = Rect2D::xywh(Vector2(x, y), Vector2(width, height));
    red = Rect2D::xywh(Vector2(x, y), Vector2(f0 * 0.032f, height));

    Draw::rect2D(red, rd, Color3::red());
    Draw::rect2D(green, rd, Color3(0.50588238f, 0.77254903f, 0.086274512f));

}

void RBX::Humanoid::snap(PVInstance* p0, PVInstance* p1)
{
    RBX::SnapJoint* snap = new RBX::SnapJoint();
    snap->make(p0, p1);
    snap->createJoint();
}

RBX::PVInstance* RBX::Humanoid::getRightArm()
{
    if (!rightArm)
    {
        rightArm = static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Right Arm"));
    }
    return rightArm;
}

RBX::PVInstance* RBX::Humanoid::getLeftArm()
{
    if (!leftArm)
    {
        leftArm = static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Left Arm"));
    }
    return leftArm;
}

RBX::PVInstance* RBX::Humanoid::getRightLeg()
{
    if (!rightLeg)
    {
        rightLeg = static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Right Leg"));
    }
    return rightLeg;
}

RBX::PVInstance* RBX::Humanoid::getLeftLeg()
{
    if (!leftLeg)
    {
        leftLeg = static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Left Leg"));
    }
    return leftLeg;
}
