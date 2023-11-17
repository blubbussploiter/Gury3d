
#include "stdout.h"
#include "model.h"

#include "rbxmath.h"
#include "ray.h"

#include "snaps.h"
#include "hinge.h"
#include "motor.h"

#include "jointservice.h"
#include "datamodel.h"

RBX::JointService* jointService;

RBX::NormalId RBX::JointService::getIntersectingFace(RBX::PVInstance* p0, RBX::PVInstance* p1) /* not 100% sure this works */
{
	RBX::World::Ray* ray;
	RBX::NormalId face;
	Vector3 normal;

	face = RBX::NormalId::UNDEFINED;
	ray = new RBX::World::Ray(p1->getPosition(), p0->getPosition());

	normal = ray->getNormalFromRay();
	if (normal != Vector3::zero())
	{
		float x, y, z;
		float v5, v6, v7, v9;

		x = dot(normal, Vector3::unitX());
		y = dot(normal, Vector3::unitY());
		z = dot(normal, Vector3::unitZ());
		v6 = fabs(x);
		v7 = fabs(z);
		v5 = v7;

		if (v6 > v5)
		{
			if (v7 < v6)
			{
				if (x <= 0.0)
					return RBX::LEFT;
				return RBX::RIGHT;
			}
			v9 = z;
		}
		v9 = z;
		if (v7 >= v5)
		{
			if (v9 <= 0.0)
				return RBX::FRONT;
			return RBX::BACK;
		}
		if (y <= 0.0)
			return RBX::BOTTOM;
		return RBX::TOP;
	}

	return face;
}

void RBX::JointService::applyJoint(RBX::PVInstance* p0, RBX::PVInstance* p1, NormalId intersect)
{
	RBX::SurfaceType surface = p0->getSurface(intersect);

	switch (surface)
	{
	case RBX::SurfaceType::Motor:
	{
		RBX::MotorJoint* motor = new RBX::MotorJoint();
		motor->setParent(this);
		motor->make(p0, p1);
		break;
	}
	default:
	{
		RBX::SnapJoint* snap = new RBX::SnapJoint();
		snap->setParent(this);
		snap->make(p0, p1);
		break;
	}
	}
}

void RBX::JointService::manualBuild(RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	NormalId intersect = getIntersectingFace(p0, p1);
	applyJoint(p1, p0, intersect);
}

bool RBX::JointService::areJointed(RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	return (p0->body->connector == p1->body->connector);
}

void RBX::JointService::buildJoints(RBX::Instance* holder)
{
	btDynamicsWorld* dynamicsWorld = RBX::RunService::singleton()->getPhysics()->_world;
	dynamicsWorld->performDiscreteCollisionDetection();

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contact = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* obj0 = contact->getBody0();
		const btCollisionObject* obj1 = contact->getBody1();

		PVInstance* part0 = (PVInstance*)obj0->getUserPointer();
		PVInstance* part1 = (PVInstance*)obj1->getUserPointer();

		if (part0 && part1)
		{
			if (!areJointed(part0, part1))
			{
				NormalId intersect = getIntersectingFace(part0, part1);
				applyJoint(part1, part0, intersect);
			}
		}
	}
}

void RBX::JointService::update()
{
	RBX::Instances* children = getChildren();
	if (children->size() > 0)
	{
		for (unsigned int i = 0; i < children->size(); i++)
		{
			RBX::Joint* joint = (RBX::Joint*)(children->at(i));
			if (!joint->isCreated)
				joint->createJoint();
		}
	}
}

RBX::JointService* RBX::JointService::singleton()
{
	return RBX::Datamodel::getDatamodel()->jointService;
}

void RBX::Joint::make(RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	part0 = p0;
	part1 = p1;
	//p0->body->connector = this;
	//p1->body->connector = this;
}