
#include "stdout.h"

#include "body.h"
#include "kernel.h"
#include "primitive.h"

dMass RBX::Body::getMass()
{
	dMass mass;

	if (!body)
	{
		mass.setZero();
		return mass;
	}

	dBodyGetMass(body, &mass);
	return mass;
}

void RBX::Body::modifyUserdata(void* data)
{
	if (!body) return;
	dBodySetData(body, data);
}

void RBX::Body::modifyVelocity(Velocity velocity)
{
	if (!body) return;

	Vector3 linear = velocity.linear;
	Vector3 rotational = velocity.rotational;

	dBodySetLinearVel(body, linear.x, linear.y, linear.z);
	dBodySetAngularVel(body, rotational.x, rotational.y, rotational.z);
}

void RBX::Body::modifyPosition(CoordinateFrame position)
{
	Vector3 translation = position.translation;
	Matrix3 rotation = position.rotation;

	if (!body) return;

	float dRotation[12] = { rotation[0][0], rotation[0][1], rotation[0][2], 0, rotation[1][0], rotation[1][1], rotation[1][2], 0, rotation[2][0], rotation[2][1], rotation[2][2], 0 };

	dBodySetRotation(body, dRotation);
	dBodySetPosition(body, translation.x, translation.y, translation.z);
}

void RBX::Body::modifyMass(dMass mass)
{
	if (!body) return;

	dBodySetMass(body, &mass);

}

void RBX::Body::modifySize(Vector3 size)
{
	destroyBody();
	createBody(size);
}

void RBX::Body::step()
{
	Velocity velocity;

	const dReal* linear;
	const dReal* rotational;

	linear = dBodyGetLinearVel(body);
	velocity.linear = Vector3(linear[0], linear[1], linear[2]);

	rotational = dBodyGetAngularVel(body);
	velocity.rotational = Vector3(rotational[0], rotational[1], rotational[2]);

	pv->velocity = velocity;
}

void RBX::Body::setDisabled(bool disabled)
{
	this->disabled = disabled;
	if (body)
	{
		dBodyDisable(body);
	}
}

void RBX::Body::createBody(Vector3 size)
{
	Vector3 position = pv->position.translation;
	Matrix3 rotation = pv->position.rotation;
	
	if (body) return;

	this->size = size;

	dMass mass;
	mass.setBox(0.01f, sqrt(size.x * 2), sqrt(size.y * 2), sqrt(size.z * 2));

	body = dBodyCreate(Kernel::get()->world);
	dBodySetMass(body, &mass);

	float dRotation[12] = { rotation[0][0], rotation[0][1], rotation[0][2], 0, rotation[1][0], rotation[1][1], rotation[1][2], 0, rotation[2][0], rotation[2][1], rotation[2][2], 0 };

	dBodySetRotation(body, dRotation);
	dBodySetPosition(body, position.x, position.y, position.z);

	dBodySetData(body, 0);

}

void RBX::Body::destroyBody()
{

	if (body != NULL)
	{
		dBodyEnable(body);

		for (int i = 0; i < dBodyGetNumJoints(body); i++) {
			dBodyID b1 = dJointGetBody(dBodyGetJoint(body, i), 0);
			dBodyID b2 = dJointGetBody(dBodyGetJoint(body, i), 1);

			dJointDestroy(dBodyGetJoint(body, i));
		}

		dBodyDestroy(body);
		body = NULL;
	}
}

void RBX::Body::attachPrimitive(Primitive* primitive)
{
	if (primitive->geom[0])
	{
		dGeomSetBody(primitive->geom[0], body);
		primitive->body = this;
	}
}

void RBX::Body::detachPrimitive(Primitive* primitive)
{
	if (primitive->geom[0])
	{
		dGeomSetBody(primitive->geom[0], 0);
		primitive->body = 0;
	}
}
