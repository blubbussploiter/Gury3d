/* (2023) Xplicit Ngine */

#include "jointservice.h"
#include "runservice.h"
#include "workspace.h"
#include "stdout.h"
#include "snaps.h"

#pragma comment(lib, "ode.lib")

void collision(void* data, dGeomID o1, dGeomID o2)
{

	int i;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	if (b1 && b2 && dAreConnected(b1, b2))
		return;

	const int N = 4;
	dContact contact[N];
	int numc = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
	if (numc > 0)
	{
		for (i = 0; i < numc; i++)
		{

			contact[i].surface.mode = dContactBounce | dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;

			// Define contact surface properties

			contact[i].surface.bounce = 0.5f; //Elasticity
			contact[i].surface.mu = 0.4f; //Friction
			contact[i].surface.slip1 = 0.0f;
			contact[i].surface.slip2 = 0.0f;
			contact[i].surface.soft_erp = 0.8f;
			contact[i].surface.soft_cfm = 0.01f;

			dJointID c = dJointCreateContact(
				RBX::RunService::singleton()->getPhysics()->physWorld,
				RBX::RunService::singleton()->getPhysics()->contactgroup, contact + i);

			dJointAttach(c, b1, b2);
			//RBX::StandardOut::print(RBX::MESSAGE_ERROR, "colliding");

		}

	}
}


void RBX::XplicitNgine::createBody(RBX::PVInstance* part)
{

	if (part->body->_body)
	{
		updateBody(part);
		return;
	}

	Matrix3 g3dRot = part->getCFrame().rotation;
	Vector3 sz = part->getSizeExternal();

	float rotation[12] = g3d2ode(g3dRot);

	part->body->_body = dBodyCreate(physWorld);
	//dBodySetData(part->body->_body, part);

	switch (part->shape)
	{
	case RBX::ball:
	{
		part->body->_shape[0] = dCreateSphere(physSpace, sz.y);
		break;
	}
	//case RBX::cylinder:
	//{
	//	part->body->_shape[0] = dCreateCylinder(physSpace, part->getSize().x, part->getSize().y);
	//	break;
	//}
	default:
	{
		part->body->_shape[0] = dCreateBox(physSpace, sz.x, sz.y, sz.z);
		break;
	}
	}

	dMass mass;
	mass.setBox(sqrt(sz.x), sqrt(sz.y), sqrt(sz.z), BODY_MASS);
	dBodySetMass(part->body->_body, &mass);

	dBodySetPosition(part->body->_body, part->getPosition().x, part->getPosition().y, part->getPosition().z);
	dGeomSetPosition(part->body->_shape[0], part->getPosition().x, part->getPosition().y, part->getPosition().z);

	dBodySetLinearVel(part->body->_body, 0, 0, 0);
	dBodySetAngularVel(part->body->_body, 0, 0, 0);

	dGeomSetRotation(part->body->_shape[0], rotation);
	dBodySetRotation(part->body->_body, rotation);

	if (!part->getAnchored())
		dGeomSetBody(part->body->_shape[0], part->body->_body);

	//dGeomSetData(part->body->_shape[0], part);
}

void RBX::XplicitNgine::removeBody(RBX::PVInstance* part)
{
	if (part->body->_body != NULL)
	{
		dBodyEnable(part->body->_body);
		dGeomEnable(part->body->_shape[0]);

		if (part->getAnchored())
		{
			dGeomSetBody(part->body->_shape[0], part->body->_body);
			dGeomEnable(part->body->_shape[0]);
			updateBody(part);
			update();
		}

		for (int i = 0; i < dBodyGetNumJoints(part->body->_body); i++) {
			dBodyID b1 = dJointGetBody(dBodyGetJoint(part->body->_body, i), 0);
			dBodyID b2 = dJointGetBody(dBodyGetJoint(part->body->_body, i), 1);

			if (b1 != NULL)
			{
				dBodyEnable(b1);
				PVInstance* part = (PVInstance*)dBodyGetData(b1);
				if (part != NULL)
					dGeomEnable(part->body->_shape[0]);
			}

			if (b2 != NULL)
			{
				dBodyEnable(b2);
				PVInstance* part = (PVInstance*)dBodyGetData(b2);
				if (part != NULL)
					dGeomEnable(part->body->_shape[0]);
			}
			dJointDestroy(dBodyGetJoint(part->body->_body, i));
		}

		dBodyDestroy(part->body->_body);
		dGeomDestroy(part->body->_shape[0]);
		part->body->_body = NULL;
	}
}

void RBX::XplicitNgine::resetBody(RBX::PVInstance* part)
{
	removeBody(part);
	createBody(part);
}

void RBX::XplicitNgine::updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p)
{
	if (!p->body->_body || !p->body->_shape[0]) return;

	//resetBody(p);

	Matrix3 g3dRot = cf.rotation;
	Vector3 g3dPos = cf.translation;

	if (!p->body->_body || !p->body->_shape[0])
		return;

	float rotation[12] = { g3dRot[0][0], g3dRot[0][1], g3dRot[0][2], 0,
					g3dRot[1][0], g3dRot[1][1], g3dRot[1][2], 0,
					g3dRot[2][0], g3dRot[2][1], g3dRot[2][2], 0 };

	dBodyEnable(p->body->_body);
	dGeomEnable(p->body->_shape[0]);

	dBodySetPosition(p->body->_body, g3dPos.x, g3dPos.y, g3dPos.z);
	dBodySetRotation(p->body->_body, rotation);

	dGeomSetPosition(p->body->_shape[0], g3dPos.x, g3dPos.y, g3dPos.z);
	dGeomSetRotation(p->body->_shape[0], rotation);

}

void RBX::XplicitNgine::updateBody(RBX::PVInstance* part)
{
	if (!part->isAffectedByPhysics || !part->body->_body)
		return;

	const dReal* physPosition = dGeomGetPosition(part->body->_shape[0]);
	const dReal* physRotation = dGeomGetRotation(part->body->_shape[0]);

	part->setCFrameNoPhysics(CoordinateFrame(
		Matrix3(physRotation[0], physRotation[1], physRotation[2],
			physRotation[4], physRotation[5], physRotation[6],
			physRotation[8], physRotation[9], physRotation[10]),
		Vector3(physPosition[0], physPosition[1], physPosition[2])));
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
}

void RBX::XplicitNgine::update()
{
	if (physWorld && physSpace)
	{
		dJointGroupEmpty(contactgroup);
		dSpaceCollide(physSpace, 0, &collision);
		dWorldQuickStep(physWorld, 0.015f);
	}
}

void RBX::XplicitNgine::close()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "XplicitNgine::close()");	
	dJointGroupDestroy(contactgroup);
	dWorldDestroy(physWorld);
	dSpaceDestroy(physSpace);
	dCloseODE();
}

void RBX::XplicitNgine::init()
{

}

bool RBX::XplicitNgine::areColliding(RBX::PVInstance* part1, RBX::PVInstance* part2)
{
	//return part1->body->_shape.;
	return 1;
}