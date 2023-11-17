#include "snaps.h"
#include "pvinstance.h"
#include "jointservice.h"
#include "runservice.h"
#include "stdout.h"
#include "bthelp.h"

RBX::SnapJoint* RBX::SnapJoint::getPreExistingSnap(RBX::PVInstance* p0)
{
	RBX::Joint* joint = p0->body->connector;
	if (joint && joint->getClassName() == "Snap")
		return (RBX::SnapJoint*)joint;
	return 0;
}

void RBX::SnapJoint::addShape(RBX::PVInstance* pv, btTransform offset, btCompoundShape* _shape, btCollisionShape* n)
{
	for (int i = 0; i < _shape->getNumChildShapes(); i++)
	{
		btCollisionShape* s = _shape->getChildShape(i);
		if (s == n)
		{
			return; /* shape already exists */
		}
	}
	_shape->addChildShape(offset, n);
}

btTransform getPartFromCompound(RBX::PVInstance* p, btCompoundShape* _shape, btRigidBody* _body)
{
	for (int i = 0; i < _shape->getNumChildShapes(); i++)
	{
		btCollisionShape* s = _shape->getChildShape(i);
		if (s == p->body->_shape)
		{
			btTransform origin = _body->getCenterOfMassTransform(), child = _shape->getChildTransform(i);
			return origin * child;
		}
	}

	return  btTransform::getIdentity();
}

btTransform RBX::SnapJoint::getPartCompoundPosition(RBX::PVInstance* p)
{
	if (!_shape)
	{
		if (p->body->_body)
		{
			return p->body->_body->getCenterOfMassTransform();
		}
	}
	else
	{
		return getPartFromCompound(p, _shape, p->body->_body);
	}

	return btTransform::getIdentity();
}

void RBX::SnapJoint::breakJoint()
{
	hasBeenCalledBroken = 1;
}

bool RBX::SnapJoint::snapBroken()
{
	if (!part0 && !part1) return 0;
}

void RBX::SnapJoint::createJoint()
{
	checkJoint();

	if (_shape) return;

	RBX::SnapJoint* snap0 = getPreExistingSnap(part0);
	RBX::SnapJoint* snap1 = getPreExistingSnap(part1);

	btTransform center0, center1;

	btDiscreteDynamicsWorld* _world = RBX::RunService::singleton()->getPhysics()->_world;

	center0 = ToTransform(part0->getCFrame());
	center1 = ToTransform(part1->getCFrame());

	if (snap0)
	{
		_shape = snap0->_shape;
		_body = snap0->_body;
		if (snap1)
		{
			snap1->_shape = _shape;
			snap1->_body = _body;
		}
	}
	if (snap1)
	{
		_shape = snap1->_shape;
		_body = snap1->_body;
		if (snap0)
		{
			snap0->_shape = _shape;
			snap0->_body = _body;
		}
	}
	if (!_shape)
	{
		_shape = new btCompoundShape();
	}

	btVector3 bodyInertia;
	int num;

	isCreated = 1;

	_world->removeRigidBody(part0->body->_body);
	_world->removeRigidBody(part1->body->_body);

	addShape(part0, center0, _shape, part0->body->_shape);
	addShape(part1, center1, _shape, part1->body->_shape);

	num = _shape->getNumChildShapes();
	_shape->calculateLocalInertia(BODY_MASS * num, bodyInertia);

	if (!snap0 && !snap1)
	{
		_body = new btRigidBody(BODY_MASS * num, new btDefaultMotionState(), _shape, bodyInertia);

		_body->setFriction(0.5f * num);
		_body->setRestitution(0.0f);
	}
	else
	{
		_world->removeRigidBody(_body);
		_body->setCollisionShape(_shape);
		_body->setMassProps(BODY_MASS * num, bodyInertia);
		_body->updateInertiaTensor();
	}

	part0->body->connector = this;
	part1->body->connector = this;

	part0->body->_body = _body;
	part1->body->_body = _body;

	_world->addRigidBody(_body);

}