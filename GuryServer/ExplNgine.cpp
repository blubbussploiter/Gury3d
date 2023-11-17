/* (2023) Xplicit Ngine */

#include "physicsSender.h"
#include "jointservice.h"
#include "runservice.h"
#include "workspace.h"
#include "stdout.h"
#include "bthelp.h"
#include "snaps.h"

#pragma comment(lib, "/bullet/BulletCollision.lib")
#pragma comment(lib, "/bullet/BulletDynamics.lib")
#pragma comment(lib, "/bullet/LinearMath.lib")

void myTickCallback(btDynamicsWorld* dynamicsWorld, btScalar timeStep) {

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {

		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();

		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			RBX::PVInstance* p0, * p1;

			p0 = (RBX::PVInstance*)objA->getUserPointer();
			p1 = (RBX::PVInstance*)objB->getUserPointer();

			if (p0 && p1 && p0->body && p1->body)
			{
				/* call :touched signal */
			}
		}
	}
}

void RBX::XplicitNgine::createBody(RBX::PVInstance* part)
{
	/* if not body, create body, else update it :) */

	Vector3 size;
	btVector3 bodyInertia;

	btTransform transform;

	btDefaultMotionState* motionState;
	btRigidBody::btRigidBodyConstructionInfo* bodyCI;

	if (part->body->_body)
	{
		updateBody(part);
		return;
	}

	size = part->getSize();

	switch (part->shape)
	{
	case RBX::ball:
	{
		part->body->_shape = new btSphereShape(part->getSize().y);
		break;
	}
	case RBX::cylinder:
	{
		part->body->_shape = new btCylinderShape(btVector3(size.x, size.y, size.z));
		break;
	}
	default: 
	{
		part->body->_shape = new btBoxShape(g3Vector3(size));
		break;
	}
	}

	transform = ToTransform(part->getCFrame());
	motionState = new btDefaultMotionState(transform);

	part->body->_shape->calculateLocalInertia(BODY_MASS, bodyInertia);
	part->body->_shape->setUserPointer(part);

	bodyCI = new btRigidBody::btRigidBodyConstructionInfo(BODY_MASS, motionState, part->body->_shape, bodyInertia);

	bodyCI->m_restitution = 0.0f;
	bodyCI->m_friction = part->getFriction();

	part->body->_body = new btRigidBody(*bodyCI);
	part->body->_body->setUserPointer((void*)part);

	part->body->_body->setCcdMotionThreshold(1e-7f);
	//part->body->_body->setCcdSweptSphereRadius(1.0f);

	if (!part->getAnchored())
		_world->addRigidBody(part->body->_body);
	else
	{
		part->body->_collider = new btCollisionObject();

		part->body->_collider->setWorldTransform(transform);
		part->body->_collider->setCollisionShape(part->body->_shape);

		part->body->_collider->setUserPointer((void*)part);

		_world->addCollisionObject(part->body->_collider);
	}
	
}

void RBX::XplicitNgine::removeBody(RBX::PVInstance* part)
{
	if (part->body->_body)
	{
		_world->removeRigidBody(part->body->_body);
	}
}

void RBX::XplicitNgine::resetBody(RBX::PVInstance* part)
{
	removeBody(part);
	createBody(part);
}

void RBX::XplicitNgine::updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p)
{
	if (!p->body->_body) return;
	p->body->_body->setWorldTransform(ToTransform(cf));
}

void RBX::XplicitNgine::updateBody(RBX::PVInstance* part)
{
	btTransform transform;
	CoordinateFrame cframe;

	RBX::Joint* connector;

	if (!part->body->_body || 
		(part->body->_body && !part->body->_body->isActive())) return;

	connector = part->body->connector;
	transform = part->body->_body->getWorldTransform();

	if (connector)
	{
		switch (connector->type)
		{
			case JointType::Snap:
			{
				RBX::SnapJoint* snap;
				snap = (RBX::SnapJoint*)(connector);
				transform = snap->getPartCompoundPosition(part);
				break;
			}
			default: break;
		}
	}

	cframe = ToCoordinateFrame(transform);
	part->setCFrameNoPhysics(cframe);
}

void RBX::XplicitNgine::checkBodies(RBX::Instances* PVInstances)
{
	RBX::PVInstance* part;
	for (unsigned int i = 0; i < PVInstances->size(); i++)
	{
		part = dynamic_cast<RBX::PVInstance*>(PVInstances->at(i));

		if (part && part->isAffectedByPhysics)
		{
			createBody(part);
		}
		checkBodies(part->getChildren());
	}
}

void RBX::XplicitNgine::updateAnchor(RBX::PVInstance* part)
{

}

bool RBX::XplicitNgine::isTouching(RBX::PVInstance* part, bool ignoreSiblings)
{
	int numManifolds = _world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {

		btPersistentManifold* contactManifold = _world->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();

		for (int j = 0; j < numContacts; j++) {
			RBX::PVInstance* p0, * p1;

			p0 = (RBX::PVInstance*)objA->getUserPointer();
			p1 = (RBX::PVInstance*)objB->getUserPointer();

			if (p0 == part || p1 == part && !(ignoreSiblings && p0->getParent() == part->getParent() || p1->getParent() == part->getParent()))
			{
				return 1;
			}
		}
	}
	return 0;
}

void RBX::XplicitNgine::update()
{
	_world->stepSimulation(0.005f);
	RBX::Network::PhysicsSender::sendPhysics(_world);
}

void RBX::XplicitNgine::close()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "XplicitNgine::close()");
}

void RBX::XplicitNgine::init()
{
	_world->setInternalTickCallback(myTickCallback);
	checkBodies(RBX::Workspace::singleton()->getChildren());
}

bool RBX::XplicitNgine::areColliding(RBX::PVInstance* part1, RBX::PVInstance* part2)
{
	//return part1->body->_shape.;
	return 1;
}