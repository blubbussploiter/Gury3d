#include "humanoid.h"
#include "rbxmath.h"

#include "camera.h"

#include "sounds.h"
#include "ray.h"

#include "jointservice.h"

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
    RBX::JointService::singleton()->manualBuild(humanoidRootPart, humanoidHead);
    if (getRightLeg()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getRightLeg());
    if (getLeftLeg()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getLeftLeg());
    if (getRightArm()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getRightArm());
    if (getLeftArm()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getLeftArm());
}

bool RBX::Humanoid::isFalling()
{
    return 1;
}

bool RBX::Humanoid::isInAir()
{
    return 1;
}

bool RBX::Humanoid::isJoined()
{
    return (checkHumanoidAttributes() && RBX::JointService::singleton()->areJointed(humanoidHead, humanoidRootPart));
}

bool RBX::Humanoid::isGrounded()
{
    return 1;
}

void RBX::Humanoid::setWalkDirection(Vector3 value)
{
    double x, z;
    double v4;

    if (value != walkDirection)
    {
        if (value == G3D::Vector3::zero())
            walkDirection = value;
        else
        {
            x = value.x;
            z = value.z;

            v4 = 1.0 / sqrt(z * z + x * x);
            walkDirection.x = x * v4;
            walkDirection.y = 0;
            walkDirection.z = z * v4;
            walkMode = DIRECTION_MOVE;
        }
    }
}

void RBX::Humanoid::setJump()
{
    if (isGrounded() && !isFalling() && !isInAir())
    {
        bsls_steps->stop();
        whoosh->play();
    }
}

void RBX::Humanoid::balance()
{
    PhysBody* body;
    body = humanoidRootPart->body;
    if (body)
    {

    }

}

void RBX::Humanoid::onDied()
{
    if (isDead)
        return;

    printf("%s\n", getParent()->getName().c_str());

    isDead = 1;
    uuhhh->play();
}

void RBX::Humanoid::onStep()
{

    CoordinateFrame _old, look;

    if (!health)
        onDied();

    if (!checkHumanoidAttributes())
    {
        correctHumanoidAttributes();
    }

    balance();

    if (!jointsBuilt)
    {
        buildJoints();
        jointsBuilt = 1;
    }

    if (!isJoined())
    {
        health = 0;
    }

}

void RBX::Humanoid::render(RenderDevice* rd)
{
    GFontRef fnt;
    fnt = Gui::singleton()->font;

    if (!checkHumanoidAttributes())
    {
        correctHumanoidAttributes();
    }

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

    Draw::rect2D(red, rd, Color3::red() * 1.5f);
    Draw::rect2D(green, rd, Color3(0.50588238f, 0.77254903f, 0.086274512f) * 1.5f);

}

RBX::PartInstance* RBX::Humanoid::getRightArm()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Right Arm"));
}

RBX::PartInstance* RBX::Humanoid::getLeftArm()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Left Arm"));
}

RBX::PartInstance* RBX::Humanoid::getRightLeg()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Right Leg"));
}

RBX::PartInstance* RBX::Humanoid::getLeftLeg()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Left Leg"));
}
