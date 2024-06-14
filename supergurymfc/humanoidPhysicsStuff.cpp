#include "humanoid.h"
#include "pvinstance.h"

#include "ray.h"
#include "rbxmath.h"

using namespace RBX;

RBX::Sound* whoosh = RBX::Sound::fromFile(GetFileInPath("/content/sounds/button.wav"));
RBX::Sound* bsls_steps = RBX::Sound::fromFile(GetFileInPath("/content/sounds/bfsl-minifigfoots1.mp3"), 1);

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
    return (getHumanoidGroundData()->distanceFrom <= genieHeight);
}

void RBX::Humanoid::adjustLimbPhysics()
{
    humanoidRootPart->getPrimitive()->friction = 1.5f;
    humanoidRootPart->getPrimitive()->elasticity = 0.f;
}

void RBX::Humanoid::tryEnable()
{
    Body* body = humanoidHead->getBody();
    if ((body && body->body) && !dBodyIsEnabled(body->body))
        dBodyEnable(body->body);
}

void RBX::Humanoid::getFeetOffGround(bool isMoving, float damper, float multiplier)
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

    r_turnVelocity = Math::getEulerAngles(difference).y / (difference.determinant());

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

    if (jumping) /* manual banual.. very bad and icky (imo) */
    {
        dBodyAddForce(body->body, 0, 20, 0);
        whoosh->play();

        getLeftLeg()->getPrimitive()->modifyCollisions(1);
        getRightLeg()->getPrimitive()->modifyCollisions(1);

        jumping = false;
        currentlyJumping = true;
    }

    switch (humanoidState)
    {
    case Jumping:
    {
        const dReal* torque = dBodyGetTorque(body->body);
        dBodyAddTorque(body->body, 0, -torque[1], 0); /* stop spinning */
        break;
    }
    case Landed:
    {
        currentlyJumping = false;
        getLeftLeg()->getPrimitive()->modifyCollisions(0);
        getRightLeg()->getPrimitive()->modifyCollisions(0);
        break;
    }
    default: break;
    }

}

void RBX::Humanoid::onStrafe()
{
    Body* body = humanoidHead->getBody();
    Vector3 velocity = humanoidHead->getVelocity();

    if (body && body->body)
    {
        adjustLimbPhysics();
        tryEnable();

        genieFloat();
        onJump();

        switch (walkMode)
        {
            case DIRECTION_MOVE:
            {
                if (!currentlyJumping)
                {
                    onTurn();
                }
                    
                if (isGrounded() && !isFalling())
                {

                    humanoidState = Strafing;

                    if (!bsls_steps->isPlaying())
                    {
                        bsls_steps->play();
                    }

                    onMovement();
                }
                break;
            }
            default:
            {
                if (walkDirection.length() <= 0)
                {
                    if(bsls_steps->isPlaying())
                        bsls_steps->stop();
                    if (isGrounded())
                    {
                        dBodyAddForce(body->body, -velocity.x, 0, -velocity.z);
                    }
                    r_turnVelocity = 0.0f;
                }

                break;
            }
        }

        getFeetOffGround(walkMode == DIRECTION_MOVE, 0.45f, 2);

    }

}

void RBX::Humanoid::onKernelStep()
{

    if (!checkHumanoidAttributes())
        return;

    onStrafe();
}