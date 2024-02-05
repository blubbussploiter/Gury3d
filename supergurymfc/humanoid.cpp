#include "humanoid.h"
#include "rbxmath.h"
#include "camera.h"
#include "players.h"
#include "sounds.h"
#include "ray.h"

#include "jointsservice.h"
#include "snap.h"

#include "GuiRoot.h"

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

RBX::Humanoid* RBX::Humanoid::modelIsCharacter(RBX::Instance* testModel)
{
    if (testModel)
        return testModel->findFirstChildOfClass<RBX::Humanoid>("Humanoid");
    else
        return 0;
}

void RBX::Humanoid::setLocalTransparency(float transparency)
{
    Instance* parent = getParent();

    for (unsigned int i = 0; i < parent->children->size(); i++)
    {
        Instance* child = parent->children->at(i);
        if (IsA<PVInstance>(child))
        {
            PVInstance* pv = toInstance<PVInstance>(child);
            float f;

            f = pv->fauxTransparency;
            pv->setTransparency(transparency);
            pv->fauxTransparency = f;
        }
    }
}

void RBX::Humanoid::setHumanoidAttributes()
{
    if (!getParent())
        return;

    humanoidHead = getParent()->findFirstChild<PVInstance>("Head");
    humanoidRootPart = getParent()->findFirstChild<PVInstance>("Torso");

}

bool RBX::Humanoid::checkHumanoidAttributes()
{
    if (!humanoidHead || !humanoidRootPart)
    {
        setHumanoidAttributes();
        return 0;
    }
    return 1;
}


void RBX::Humanoid::buildJoints()
{
    if (isJoined()) return;

    snap(humanoidRootPart, humanoidHead);
    snap(humanoidRootPart, getRightArm());
    snap(humanoidRootPart, getLeftArm());
    snap(humanoidRootPart, getRightLeg());
    snap(humanoidRootPart, getLeftLeg());
}

bool RBX::Humanoid::isFalling()
{
    //return (humanoidRootPart->body->body->getLinearVelocity().y() <= -0.1);
    return 0;
}

bool RBX::Humanoid::isInAir()
{
   //return (humanoidRootPart->body->_body->getLinearVelocity().y() > 0.1);
    return 0;
}

bool RBX::Humanoid::isJoined()
{
    if (!checkHumanoidAttributes()) return 0;

    return JointsService::areConnectedPrimitives(humanoidRootPart->primitive, humanoidHead->primitive);
}

bool RBX::Humanoid::limbsCheck()
{
    return checkHumanoidAttributes();
}

bool RBX::Humanoid::isGrounded()
{
    if (rightLeg && leftLeg)
    {

    }

    return 0;
}

void RBX::Humanoid::setJump(bool jump)
{
    isJumping = jump;
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
            walkDirection *= 12;
            walkMode = DIRECTION_MOVE;
        }
    }
}

void RBX::Humanoid::snap(PVInstance* p0, PVInstance* p1)
{
    if (!p0 || !p1) return;
    SnapConnector* snap = new SnapConnector(p0->primitive, p1->primitive);
    snap->build();
}

void RBX::Humanoid::balance()
{

}

void RBX::Humanoid::climb()
{
}

void RBX::Humanoid::onDied()
{
    if (isDead)
        return;

    isDead = 1;
    uuhhh->play();
}

void RBX::Humanoid::jumpTimeStep()
{
    if (!canJump && jumpClock >= jumpTimer)
    {
        canJump = 1;
        jumpClock = 0;
    }
    else
    {
        if (!canJump)
            jumpClock += 0.01f;
    }
}

void RBX::Humanoid::turn()
{
   // PhysBody* body = humanoidRootPart->body;
    CoordinateFrame origin = humanoidRootPart->getCFrame(), frame = origin;

    frame.lookAt(origin.translation + walkDirection);

    float Y;
    frame.rotation.toAxisAngle(Vector3(0, 1, 0), Y);

    walkRotationVelocity = Vector3(0, Y / 2, 0);
}

void RBX::Humanoid::onStep()
{
    if (!limbsCheck()) return;

    jumpTimeStep();
    buildJoints();

    if (!health)
        onDied();

    if (!isJoined())
    {
        health = 0;
    }

    updateHumanoidState();

    /*
    switch (walkMode)
    {
    case DIRECTION_MOVE:
    {
        if (body)
        {
            btVector3 velocity = body->getLinearVelocity();
            float speed = velocity.length();

            body->activate(1);
            turn();

            if (humanoidState == Running || isClimbing)
            {
                if (isFalling()) break;

                if (!bsls_steps->isPlaying())
                    bsls_steps->play();

                humanoidState = Strafing;

                if (speed < 6.56)
                {
                    btVector3 strafeVelocity(walkDirection.x, velocity.y(), walkDirection.z);
                    body->setLinearVelocity(strafeVelocity);
                }

            }

            if (walkRotationVelocity)
            {
                btVector3 vel = body->getTotalTorque();
                btVector3 torque = btVector3(0, walkRotationVelocity.y, 0) - vel;
                
                body->setAngularFactor(btVector3(0, 1, 0));
                if (vel.y() < 0.001f)
                {
                    body->applyTorque(torque * 0.001f);
                }
            }

            climb();

        }
        break;
    }
    case DIRECTION_STAY:
    {
        bsls_steps->stop();
        walkRotationVelocity = Vector3::zero();
        body->setAngularVelocity(btVector3(0, 0, 0));
        break;
    }
    default: break;
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
                if (!canJump) return;

                humanoidState = Jumping;

                whoosh->play();
                bsls_steps->stop();

                body->activate(1);
                body->setRestitution(0.8f);

                body->setAngularVelocity(btVector3(0,0,0));
                body->setLinearVelocity(newVelocity);

                jumpClock = 0;
                canJump = 0;
            }
        }
        isJumping = 0;
    }

    if (humanoidRootPart->getPosition().y < -200)
    {
        health = 0;
        onDied();
        getParent()->remove();
    }
    */
}

void RBX::Humanoid::updateHumanoidState()
{
    //btRigidBody* body = humanoidRootPart->body->_body;

    if (isGrounded())
    {
        humanoidState = Running;
    }

    if (!isGrounded() && isFalling())
    {
        if (humanoidState != Jumping)
        {
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


RBX::PVInstance* RBX::Humanoid::getRightArm()
{
    if (!rightArm)
    {
        rightArm = getParent()->findFirstChild<PVInstance>("Right Arm");
    }
    return rightArm;
}

RBX::PVInstance* RBX::Humanoid::getLeftArm()
{
    if (!leftArm)
    {
        leftArm = getParent()->findFirstChild<PVInstance>("Left Arm");
    }
    return leftArm;
}

RBX::PVInstance* RBX::Humanoid::getRightLeg()
{
    if (!rightLeg)
    {
        rightLeg = getParent()->findFirstChild<PVInstance>("Right Leg");
    }
    return rightLeg;
}

RBX::PVInstance* RBX::Humanoid::getLeftLeg()
{
    if (!leftLeg)
    {
        leftLeg = getParent()->findFirstChild<PVInstance>("Left Leg");
    }
    return leftLeg;
}
