#include "pvinstance.h"
#include "jointservice.h"
#include "stdout.h"

bool RBX::PhysBody::isColliding(NormalId preferredFace)
{
    btDynamicsWorld* dynamicsWorld = RBX::RunService::singleton()->getPhysics()->_world;
	RBX::JointService* jointService = RBX::Datamodel::getDatamodel()->jointService;

	dynamicsWorld->performDiscreteCollisionDetection();
    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contact = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		
		const btCollisionObject* obj0 = contact->getBody0();
		const btCollisionObject* obj1 = contact->getBody1();

		if (obj0 == _body)
		{
			for (int m = 0; m < contact->getNumContacts(); m++)
			{
				NormalId intersect = jointService->getIntersectingFace(contact, m);
				if (intersect == preferredFace && preferredFace != UNDEFINED)
				{
					return 1;
				}
			}
		}
	}

    return 0;
}
