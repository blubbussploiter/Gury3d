
#include "runservice.h"

#include "workspace.h"
#include "stdout.h"

#pragma comment(lib, "ode.lib")

void collision(void* data, dGeomID o1, dGeomID o2)
{
	int i;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	if (b1 && b2 && dAreConnected(b1, b2))
		return;

	dContact contact[4];
	int numc = dCollide(o1, o2, 4, &contact[0].geom, sizeof(dContact));

	if (numc > 0)
	{
		for (i = 0; i < numc; i++)
		{

			contact[i].surface.mode = dContactBounce | dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;

			// Define contact surface properties

			contact[i].surface.bounce = 0.5f; //Elasticity
			contact[i].surface.mu = 0.3f; //Friction
			contact[i].surface.slip1 = 0.0f;
			contact[i].surface.slip2 = 0.0f;
			contact[i].surface.soft_erp = 0.8f;
			contact[i].surface.soft_cfm = 0.005f;

			dJointID c = dJointCreateContact(
				RBX::RunService::singleton()->getPhysics()->physWorld,
				RBX::RunService::singleton()->getPhysics()->contactgroup, 
				contact + i);

			dJointAttach(c, b1, b2);
		}

	}
}


void RBX::XplicitNgine::createBody(RBX::PVInstance* part)
{
	if (part->body->geom[0])
	{
		updateBody(part);
		return;
	}

	Matrix3 g3dRot = part->getCFrame().rotation;

	float rotation[12] = { g3dRot[0][0], g3dRot[0][1], g3dRot[0][2], 0, g3dRot[1][0], g3dRot[1][1], g3dRot[1][2], 0, g3dRot[2][0], g3dRot[2][1], g3dRot[2][2], 0 };

	Vector3 size = part->getSize() * 2;
	Vector3 position = part->getPosition();

	switch (part->shape)
	{
	case RBX::ball:
	{
		part->body->geom[0] = dCreateSphere(physSpace, size.y / 2);
		break;
	}

	case RBX::cylinder:
	{
		part->body->geom[0] = dCreateSphere(physSpace, size.z / 2);
		break;
	}
	default:
	{
		part->body->geom[0] = dCreateBox(physSpace, size.x, size.y, size.z);
		break;
	}
	}

	dMass mass;

	switch (part->shape)
	{
	default:
	{
		mass.setBox(0.7f, sqrt(size.x * 2), sqrt(size.y * 2), sqrt(size.z* 2));
		break;
	}
	}

	if (!part->anchored)
	{
		part->body->body = dBodyCreate(physWorld);

		dBodySetMass(part->body->body, &mass);
		dBodySetPosition(part->body->body, position.x, position.y, position.z);

		dBodySetLinearVel(part->body->body, 0, 0, 0);
		dBodySetAngularVel(part->body->body, 0, 0, 0);

		dBodySetRotation(part->body->body, rotation);
		dGeomSetBody(part->body->geom[0], part->body->body);
	}

	dGeomSetPosition(part->body->geom[0], position.x, position.y, position.z);
	dGeomSetRotation(part->body->geom[0], rotation);

}

void RBX::XplicitNgine::removeBody(RBX::PVInstance* part)
{
	if (part->body->body != NULL)
	{
		dBodyEnable(part->body->body);
		dGeomEnable(part->body->geom[0]);

		if (part->getAnchored())
		{
			dGeomSetBody(part->body->geom[0], part->body->body);
			dGeomEnable(part->body->geom[0]);
			updateBody(part);
			update();
		}

		for (int i = 0; i < dBodyGetNumJoints(part->body->body); i++) {
			dBodyID b1 = dJointGetBody(dBodyGetJoint(part->body->body, i), 0);
			dBodyID b2 = dJointGetBody(dBodyGetJoint(part->body->body, i), 1);

			if (b1 != NULL)
			{
				dBodyEnable(b1);
				PVInstance* part = (PVInstance*)dBodyGetData(b1);
				if (part != NULL)
					dGeomEnable(part->body->geom[0]);
			}

			if (b2 != NULL)
			{
				dBodyEnable(b2);
				PVInstance* part = (PVInstance*)dBodyGetData(b2);
				if (part != NULL)
					dGeomEnable(part->body->geom[0]);
			}
			dJointDestroy(dBodyGetJoint(part->body->body, i));
		}

		dBodyDestroy(part->body->body);
		dGeomDestroy(part->body->geom[0]);
		part->body->body = NULL;
		part->body->geom[0] = NULL;
	}
}

void RBX::XplicitNgine::resetBody(RBX::PVInstance* part)
{
	removeBody(part);
	createBody(part);
}

void RBX::XplicitNgine::updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p)
{
	if (!p->body->body || !p->body->geom[0])
		return;

	//resetBody(p);

	Matrix3 g3dRot = cf.rotation;
	Vector3 g3dPos = cf.translation;

	float rotation[12] = { g3dRot[0][0], g3dRot[0][1], g3dRot[0][2], 0,
					g3dRot[1][0], g3dRot[1][1], g3dRot[1][2], 0,
					g3dRot[2][0], g3dRot[2][1], g3dRot[2][2], 0 };

	dBodyEnable(p->body->body);
	dGeomEnable(p->body->geom[0]);

	dBodySetPosition(p->body->body, g3dPos.x, g3dPos.y, g3dPos.z);
	dBodySetRotation(p->body->body, rotation);

	dGeomSetPosition(p->body->geom[0], g3dPos.x, g3dPos.y, g3dPos.z);
	dGeomSetRotation(p->body->geom[0], rotation);
}

void RBX::XplicitNgine::updateBody(RBX::PVInstance* part)
{
	if (!part->body->body)
		return;

	const dReal* physPosition = dGeomGetPosition(part->body->geom[0]);
	const dReal* physRotation = dGeomGetRotation(part->body->geom[0]);

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

		if (part)
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
	return 0;
}

void RBX::XplicitNgine::update(float stepSize)
{
	dJointGroupEmpty(contactgroup);
	dSpaceCollide(physSpace, 0, &collision);
	dWorldQuickStep(physWorld, stepSize);
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
	checkBodies(RBX::Workspace::singleton()->getChildren());
}

bool RBX::XplicitNgine::areColliding(RBX::PVInstance* part1, RBX::PVInstance* part2)
{
	return 1;
}