#include "snaps.h"
#include "pvinstance.h"
#include "jointservice.h"
#include "runservice.h"
#include "stdout.h"
#include "bthelp.h"

void copyCompoundShape(btCompoundShape* from, btCompoundShape* to)
{
	size_t cs = from->getNumChildShapes();
	for (size_t i = 0; i < cs; i++)
	{
		btCompoundShapeChild* s = from->getChildList() + i;
		if (s)
		{
			to->addChildShape(s->m_transform, s->m_childShape);
		}
	}
}

void addShape(RBX::PVInstance* pv, btTransform offset, btCompoundShape* _shape, btCollisionShape* n)
{
	for (int i = 0; i < _shape->getNumChildShapes(); i++)
	{
		btCollisionShape* s = _shape->getChildShape(i);
		if (s->getUserPointer() == pv)
		{
			return; /* shape already exists */
		}
	}
	n->setUserPointer(pv);
	_shape->addChildShape(offset, n);
}

btTransform getPartFromCompound(RBX::PVInstance* p, btCompoundShape* _shape, btRigidBody* _body)
{
	for (int i = 0; i < _shape->getNumChildShapes(); i++)
	{
		btCompoundShapeChild* s = _shape->getChildList() + i;
		if (s)
		{
			btCollisionShape* cs = s->m_childShape;
			if (cs && cs->getUserPointer() == p)
			{
				btTransform childTransform = s->m_transform;
				return _body->getCenterOfMassTransform() * childTransform;
			}
		}
	}

	return btTransform::getIdentity();
}

btTransform RBX::SnapJoint::getCenterOfMass()
{

	float lastArea = -1;
	RBX::PVInstance* largest = 0;
	
	for (int i = 0; i < _shape->getNumChildShapes(); i++)
	{
		btCompoundShapeChild* s = _shape->getChildList() + i;
		if (s)
		{
			btCollisionShape* cs = s->m_childShape;
			if (cs)
			{
				void* ptr = cs->getUserPointer();
				RBX::Instance* i = 0;

				if ((i = static_cast<RBX::Instance*>(ptr)) && i)
				{
					RBX::PVInstance* pv = dynamic_cast<RBX::PVInstance*>(i);
					if (pv)
					{
						RBX::Extents extents = pv->getWorldExtents();
						float area = extents.area();
						if (area > lastArea)
						{
							lastArea = area;
							largest = pv;
						}
					}
				}
			}
		}
	}
	
	if (largest) return largest->body->_body->getCenterOfMassTransform();
	return btTransform();
}

btTransform RBX::SnapJoint::getPartCompoundPosition(RBX::PVInstance* p)
{
	return getPartFromCompound(p, _shape, p->body->_body);
}

RBX::SnapJoint* RBX::SnapJoint::getPreExistingSnap(RBX::PVInstance* p0)
{
	RBX::Joint* joint = p0->body->connector;

	if (RBX::IsA<SnapJoint>(joint))
		return (RBX::SnapJoint*)joint;

	return 0;
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

	btDiscreteDynamicsWorld* _world = RBX::RunService::singleton()->getPhysics()->_world;
	_shape = new btCompoundShape();

	if (snap0)
	{
		copyCompoundShape(snap0->_shape, _shape);
	}

	if (snap1)
	{
		copyCompoundShape(snap1->_shape, _shape);
	}

	int num;

	btVector3 bodyInertia;
	btTransform center, center0, center1;

	center0 = ToTransform(part0->getCFrame());
	center1 = ToTransform(part1->getCFrame());

	btCollisionShape* s0 = part0->body->_body->getCollisionShape(), * s1 = part1->body->_body->getCollisionShape();

	addShape(part0, center0, _shape, s0);
	addShape(part1, center1, _shape, s1);

	num = _shape->getNumChildShapes();
	if (!num) num = 2;

	btScalar* masses = new btScalar[num];

	for (int i = 0; i < num; i++)
		masses[i] = BODY_MASS;

	_shape->calculatePrincipalAxisTransform(masses, center, bodyInertia);

	btTransform cOfM = getCenterOfMass();

	btRigidBody* body = new btRigidBody(BODY_MASS * num, new btDefaultMotionState(cOfM.inverse(), cOfM.inverse()), _shape, bodyInertia);

	body->setRestitution(0.75f);
	body->setFriction(0.300000012f * 10);

	body->setAngularFactor(btVector3(0, 0, 0));

	for (size_t i = 0; i < num; i++) 
	{
		btCompoundShapeChild* s = _shape->getChildList() + i;
		if (s)
		{
			void* ptr = s->m_childShape->getUserPointer();
			RBX::Instance* i = 0;

			if ((i = static_cast<RBX::Instance*>(ptr)) && i)
			{
				RBX::PVInstance* pv = dynamic_cast<RBX::PVInstance*>(i);
				if (pv)
				{
					RBX::SnapJoint* snap = dynamic_cast<RBX::SnapJoint*>(pv->body->connector);
					if (pv->body->_body != body)
					{
						_world->removeRigidBody(pv->body->_body);
					}
					if (pv->anchored)
					{
						body->setMassProps(0, btVector3(0, 0, 0));
						body->updateInertiaTensor();
					}
					if (snap) snap->_shape = _shape;
					pv->body->_body = body;
				}
			}
		}
	}

	_world->addRigidBody(body);

	part0->body->connector = this;
	part1->body->connector = this;

	_body = body;

}