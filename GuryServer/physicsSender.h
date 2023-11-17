#pragma once

#include "replicator.h"
#include "btBulletCollisionCommon.h"

namespace RBX
{
	namespace Network
	{
		class PhysicsSender
		{
			friend class Replicator;
		public:

			static void sendRigidBody(Instance* instance, btRigidBody* object);

			static void sendPhysics(btDiscreteDynamicsWorld* world);
			static bool shouldSendPhysics();

		};
	}
}