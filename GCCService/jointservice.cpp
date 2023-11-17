
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

RBX::NormalId RBX::JointService::getIntersectingFace(btPersistentManifold* manifold, int index) /* thank you RNR */
{
	btManifoldPoint& pt = manifold->getContactPoint(index);
	btVector3 ptN = pt.m_normalWorldOnB;

	if (ptN == btVector3(1, 0, 0))
		return LEFT;
	else if (ptN == btVector3(-1, 0, 0))
		return RIGHT;
	else if (ptN == btVector3(0, 1, 0))
		return BOTTOM;
	else if (ptN == btVector3(0, -1, 0))
		return TOP;
	else if (ptN == btVector3(0, 0, 1))
		return BACK;
	else if (ptN == btVector3(0, 0, -1))
		return FRONT;

	return UNDEFINED;
}

bool RBX::JointService::areJointed(RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	return p0->body->_body == p1->body->_body;
}

void RBX::JointService::applyJoint(RBX::PVInstance* p0, RBX::PVInstance* p1, NormalId intersect)
{
	RBX::SurfaceType surface = p0->getSurface(intersect);

	switch (surface)
	{
	case RBX::SurfaceType::Motor:
	{
		RBX::MotorJoint* motor = new RBX::MotorJoint();
		motor->make(p0, p1);
		break;
	}
	case RBX::SurfaceType::Weld:
	case RBX::SurfaceType::Glue:
	case RBX::SurfaceType::Studs:
	case RBX::SurfaceType::Inlet:
	{
		RBX::SnapJoint* snap = new RBX::SnapJoint();
		snap->make(p0, p1);
		snap->createJoint();
		break;
	}
	}
}

void RBX::JointService::manualBuild(RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	buildJoints(p0, p1);
}

void RBX::JointService::buildJoints(RBX::Instance* holder, RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	btDynamicsWorld* dynamicsWorld = RBX::RunService::singleton()->getPhysics()->_world;

	dynamicsWorld->performDiscreteCollisionDetection();

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		btPersistentManifold* contact = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* obj0 = contact->getBody0();
		const btCollisionObject* obj1 = contact->getBody1();

		if (obj0 && obj1)
		{
			PVInstance* part0 = (PVInstance*)obj0->getUserPointer(); 
			PVInstance* part1 = (PVInstance*)obj1->getUserPointer();

			if (!part0 || !part1) continue;
			if (p0 && p1 && !(part0 == p0 || part1 == p1)) continue;
			if (areJointed(part0, part1)) continue;

			for (int m = 0; m < contact->getNumContacts(); m++)
			{
				NormalId intersect = getIntersectingFace(contact, m);
				if (intersect != UNDEFINED)
				{
					applyJoint(part1, part0, intersect);
				}
			}
		}
	}

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "JointService::buildJoints(), building %d joints", getChildren()->size());

}

RBX::JointService* RBX::JointService::singleton()
{
	return RBX::Datamodel::getDatamodel()->jointService;
}

void RBX::Joint::make(RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	part0 = p0;
	part1 = p1;
	setParent(JointService::singleton());
}