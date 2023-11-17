
#include "bthelp.h"
#include "physicsSender.h"
#include "rbx_identifiers.h"

using namespace RBX::Network;
using namespace RBX::Render;

void RBX::Network::PhysicsSender::sendRigidBody(Instance* instance, btRigidBody* object)
{
    BitStream stream;
    CoordinateFrame cframe;

    Replicator* replicator = Replicator::replicator();

    Vector3 v;
    Matrix3 m;

    if (!instance->id) return;

    cframe = ToCoordinateFrame(object->getWorldTransform());

    v = cframe.translation;
    m = cframe.rotation;

    stream << (unsigned char)ID_SEND_PHYSICS;
    stream << instance->id->guid->data.index;
    stream << object->getActivationState();

    replicator->serialize<CoordinateFrame>(stream, cframe);
    replicator->peer->Send(
        &stream,
        MEDIUM_PRIORITY,
        RELIABLE_ORDERED,
        0,
        replicator->server,
        0
    );
    
}

void PhysicsSender::sendPhysics(btDiscreteDynamicsWorld* world)
{
    if (!shouldSendPhysics()) return;

    std::vector<Renderable*> physicsObjects = Scene::singleton()->getArrayOfObjects();
    for (unsigned int i = 0; i < physicsObjects.size(); i++)
    {
        Renderable* object = physicsObjects.at(i);
        PVInstance* pvinstance = dynamic_cast<PVInstance*>(object);

        if (pvinstance 
            && pvinstance->body
            && pvinstance->body->_body)
        {
            sendRigidBody(pvinstance, pvinstance->body->_body);
        }
    }

}

bool PhysicsSender::shouldSendPhysics()
{
    if (!Replicator::replicator()) return 0;
    return Replicator::replicator()->connected();
}
