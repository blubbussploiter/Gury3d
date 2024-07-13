#include "humanoid.h"
#include "pvinstance.h"

#include "ray.h"
#include "rbxmath.h"

using namespace RBX;

RBX::Sound* whoosh = RBX::Sound::fromFile(GetFileInPath("/content/sounds/button.wav"));
RBX::Sound* bsls_steps = RBX::Sound::fromFile(GetFileInPath("/content/sounds/bfsl-minifigfoots1.mp3"), 1);

void RBX::Humanoid::setLegCollisions(bool collidable)
{

    PVInstance* leftLeg = getLeftLeg(), *rightLeg = getRightLeg();

    if (leftLeg && rightLeg)
    {
        Primitive* leftLegPrimitive = leftLeg->getPrimitive();
        Primitive* rightLegPrimitive = rightLeg->getPrimitive();

        if (leftLegPrimitive && rightLegPrimitive)
        {
            leftLegPrimitive->modifyCollisions(collidable);
            rightLegPrimitive->modifyCollisions(collidable);
        }
    }

}

void RBX::Humanoid::setArmCollisions(bool collidable)
{
    PVInstance* leftArm = getLeftArm(), * rightArm = getRightArm();

    if (leftArm && rightArm)
    {
        Primitive* leftArmPrimitive = leftArm->getPrimitive();
        Primitive* rightArmPrimitive = rightArm->getPrimitive();

        if (leftArmPrimitive && rightArmPrimitive)
        {
            leftArmPrimitive->modifyCollisions(collidable);
            rightArmPrimitive->modifyCollisions(collidable);
        }
    }
}

groundData* RBX::Humanoid::getHumanoidGroundData()
{
    groundData* data = new groundData();
    CoordinateFrame cframe = humanoidRootPart->getCoordinateFrame();
    Ray ray = Ray::fromOriginAndDirection(cframe.translation, -Vector3::unitY());
    Vector3 hit;

    RBX::ISelectable* maybeGround = World::getPartFromG3DRay<Instance>(ray, hit, true, *getParent()->getChildren());
    data->hit = hit;
    data->normal = normalize(data->hit);
    data->distanceFrom = (ray.origin - hit).magnitude();
    return data;
}

bool RBX::Humanoid::isGrounded()
{
    return (getHumanoidGroundData()->distanceFrom < hipHeight);
}

void RBX::Humanoid::adjustLimbPhysics()
{
    setLegCollisions(false);
    setArmCollisions(false);
}

void RBX::Humanoid::tryEnable()
{
    Body* body = humanoidHead->getBody();
    if ((body && body->body) && !dBodyIsEnabled(body->body))
    {
        dBodyEnable(body->body);
    }
}

void RBX::Humanoid::getFeetOffGround(float damper, float multiplier)
{
    CoordinateFrame cframe = humanoidHead->getCoordinateFrame();
    Quat rotation = cframe.rotation.inverse();
    Body* body = humanoidHead->getBody();

    if (!body || !body->body) return;

    Vector3 rotVelocity = humanoidHead->getRotVelocity();
    Vector3 torque = ((Vector3(rotation.x, 0, rotation.z) - rotVelocity / 4 * damper) * multiplier) * body->getFMass() * (1 / RunService::get()->deltaTime);

    body->applyTorque(torque);
}

void RBX::Humanoid::applyHipHeight()
{
    Body* body = humanoidHead->getBody();

    if (currentlyJumping) return;
    if (!body || !body->body) return;

    groundData* ground = getHumanoidGroundData();

    Vector3 position = humanoidRootPart->getPosition();
    Vector3 velocity = body->pv->velocity.linear;
    Vector3 rotationalVelocity = body->pv->velocity.rotational;

    if (ground->distanceFrom <= hipHeight)
    {
        Vector3 desired = Vector3(position.x, ground->hit.y + hipHeight, position.z);
        body->applyForce(Vector3(0, -velocity.y * 0.25f, 0));
        dBodySetPosition(body->body, desired.x, desired.y, desired.z);
    }
}

void RBX::Humanoid::onTurn()
{
    Body* body = humanoidHead->getBody();

    if (!body || !body->body) return;

    CoordinateFrame origin = humanoidRootPart->getCFrame(), changed = origin;
    changed.lookAt(origin.translation + walkDirection);

    Vector3 rotVelocity = humanoidRootPart->getRotVelocity();
    Matrix3 difference = changed.rotation * origin.rotation.inverse();

    r_turnVelocity = Math::getEulerAngles(difference).y;
}

void RBX::Humanoid::onMovement()
{
    Body* body = humanoidHead->getBody();
    Vector3 velocity = humanoidHead->getVelocity();

    float speed = velocity.length();

    if (speed < 8.56f)
    {
        Vector3 strafeVelocity(walkDirection.x, velocity.y, walkDirection.z);
        humanoidHead->setVelocity(strafeVelocity);
    }

}

void RBX::Humanoid::onJump()
{
    Body* body = humanoidHead->getBody();
    Vector3 velocity = body->pv->velocity.linear;
    if (!body || !body->body) return;

    if (jumping)
    {
        body->applyForceAtRelativePosition(Vector3(0, 7.5f, 0), Vector3(0, -hipHeight / 2, 0));
        setLegCollisions(true);

        whoosh->play();

        jumping = false;
        currentlyJumping = true;
    }

    if (whoosh->isPlaying()
        && jumpClock > 0.05f)
    {
        whoosh->stop();
    }

    switch (humanoidState)
    {
    case Jumping:
    {
        r_turnVelocity = 0;
        break;
    }
    case Landed:
    {
        currentlyJumping = false;
        setLegCollisions(false);
        break;
    }
    default: break;
    }

}

void RBX::Humanoid::doSounds()
{
    switch (humanoidState)
    {
    case Strafing:
    {
        bsls_steps->playOnce();
        break;
    }
    case Running:
    {
        bsls_steps->stop();
        break;
    }
    }
}

void RBX::Humanoid::onStrafe()
{
    Body* body = humanoidHead->getBody();
    Vector3 velocity = humanoidHead->getVelocity();

    if (body && body->body)
    {

        if (!isTripped())
        {
            //doSounds();

            adjustLimbPhysics();
            tryEnable();

            applyHipHeight();
            onJump();

            switch (walkMode)
            {
            case DIRECTION_MOVE:
            {
                if (isGrounded() && !isFalling())
                {
                    humanoidState = Strafing;
                    onMovement();
                }

                if (!currentlyJumping)
                {
                    onTurn();
                }

                break;
            }
            default:
            {
                if (walkDirection == Vector3::zero())
                {
                    if (isGrounded())
                    {
                        if (velocity.length() > 0)
                        {
                            Vector3 brakeVelocity = -velocity * 0.5f;
                            body->applyForce(Vector3(brakeVelocity.x, 0, brakeVelocity.z));
                            body->modifyVelocity(Velocity(body->pv->velocity.linear, Vector3::zero()));
                        }
                    }
                    r_turnVelocity = 0.0f;
                }
                break;
            }
            }

            if (fabs(r_turnVelocity) > 0.0f)
            {
                body->applyTorque(Vector3::unitY() * (r_turnVelocity * 0.3f));
            }
        }
        getFeetOffGround(0.4f, 1.0f);
    }

}