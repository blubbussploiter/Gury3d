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
    return (getHumanoidGroundData()->distanceFrom < genieHeight);
}

void RBX::Humanoid::adjustLimbPhysics()
{
    humanoidRootPart->getPrimitive()->friction = 2.f;
    humanoidRootPart->getPrimitive()->elasticity = 0.f;
    setLegCollisions(false);
    setArmCollisions(false);
}

void RBX::Humanoid::tryEnable()
{
    Body* body = humanoidHead->getBody();
    if ((body && body->body) && !dBodyIsEnabled(body->body))
        dBodyEnable(body->body);
}

void RBX::Humanoid::getFeetOffGround(float damper, float multiplier)
{
    CoordinateFrame cframe = humanoidHead->getCoordinateFrame();
    Quat rotation = cframe.rotation.inverse();
    Body* body = humanoidHead->getBody();

    if (!body || !body->body) return;

    Vector3 rotVelocity = humanoidHead->getRotVelocity();
    Vector3 torque = ((Vector3(rotation.x, 0, rotation.z) - rotVelocity / 2 * damper) * multiplier) * body->getFMass() * 1 / RunService::singleton()->deltaTime;

    attemptingToBalance = (cframe.rotation == rotation);

    dBodyAddTorque(body->body, torque.x, torque.y + r_turnVelocity, torque.z);

}

void RBX::Humanoid::genieFloat()
{
    Body* body = humanoidHead->getBody();

    if (currentlyJumping) return;
    if (!body || !body->body) return;
    
    groundData* ground = getHumanoidGroundData();

    Vector3 position = humanoidRootPart->getPosition();
    Vector3 velocity = body->pv->velocity.linear;

    if (ground->distanceFrom <= genieHeight)
    {
        Vector3 desired = Vector3(position.x, ground->hit.y + genieHeight, position.z);
        dBodyAddForceAtRelPos(body->body, 0, -velocity.y, 0, 0, genieHeight/2, 0);
        dBodySetPosition(body->body, desired.x, desired.y, desired.z);
    }
}

void RBX::Humanoid::onTurn()
{
    Body* body = humanoidHead->getBody();

    if (!body || !body->body) return;

    tryEnable();

    CoordinateFrame origin = humanoidRootPart->getCFrame(), changed = origin;
    changed.lookAt(origin.translation + walkDirection);

    Vector3 rotVelocity = humanoidRootPart->getRotVelocity();
    Matrix3 difference = changed.rotation * origin.rotation.inverse();

    r_turnVelocity = Math::getEulerAngles(difference).y * 0.5f;

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
        dBodyAddForceAtRelPos(body->body, 0, 7.5f, 0, 0, -genieHeight/2, 0);

        setLegCollisions(true);

        jumping = false;
        currentlyJumping = true;
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
        doSounds();
        switch (humanoidState)
        {
        case Tripped:
        {
            if (currentlyJumping)
                break;
            getFeetOffGround(0.2f, 3.0f);
            setLegCollisions(false);
            wasTripped = 1;
            break;
        }
        default:
        {
            adjustLimbPhysics();
            tryEnable();

            genieFloat();
            onJump();

            if(wasTripped) /* just got out of being tripped, reset rotational velocity */
            {
                const dReal* torque = dBodyGetTorque(body->body);
                dBodySetTorque(body->body, -torque[0], -torque[1], -torque[2]);
                wasTripped = false;
            }

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
                            dBodyAddForce(body->body, -velocity.x, 0, -velocity.z);
                        }
                        r_turnVelocity = 0.0f;
                    }
                    break;
                }
            }

            getFeetOffGround(0.2f, 2.5f);
            break;
        }
        }

    }

}

void RBX::Humanoid::onKernelStep()
{

    if (!checkHumanoidAttributes())
        return;

    onStrafe();
}