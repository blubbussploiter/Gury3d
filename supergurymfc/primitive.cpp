
#include "kernel.h"

#include "stdout.h"
#include "primitive.h"

void RBX::Primitive::modifySize(Vector3 size)
{
	if (!geom[0]) return;

	this->size = size;
	switch (shape)
	{
	case ball:
	{
		dGeomSphereSetRadius(geom[0], size.y);
		break;
	}
	default:
	{
		dGeomBoxSetLengths(geom[0], size.x * 2, size.y * 2, size.z * 2);
		break;
	}
	}
}

void RBX::Primitive::modifyShape(Shape shape)
{
	this->shape = shape;
	reset();
}

void RBX::Primitive::modifyPosition(CoordinateFrame position)
{
	if (!geom[0]) return;

	Vector3 translation = position.translation;
	Matrix3 rotation = position.rotation;

	float dRotation[12] = { rotation[0][0], rotation[0][1], rotation[0][2], 0, rotation[1][0], rotation[1][1], rotation[1][2], 0, rotation[2][0], rotation[2][1], rotation[2][2], 0 };

	dGeomSetPosition(geom[0], translation.x, translation.y, translation.z);
	dGeomSetRotation(geom[0], dRotation);
}

void RBX::Primitive::modifyCollisions(bool canCollide)
{
	if (!geom[0]) return;
	if (!canCollide)
	{
		dGeomDisable(geom[0]);
	}
	else
	{
		dGeomEnable(geom[0]);
	}
}

void RBX::Primitive::modifyUserdata(void* data)
{
	if (!geom[0]) return;
	dGeomSetData(geom[0], data);
}

void RBX::Primitive::modifyOffsetWorldCoordinateFrame(CoordinateFrame offset)
{
	if (!geom[0]) return;

	Vector3 position = offset.translation;
	Matrix3 rotation = offset.rotation;

	float dRotation[12] = { rotation[0][0], rotation[0][1], rotation[0][2], 0, rotation[1][0], rotation[1][1], rotation[1][2], 0, rotation[2][0], rotation[2][1], rotation[2][2], 0 };

	/* ODE is goated for these functions */

	dGeomSetOffsetWorldPosition(geom[0], position.x, position.y, position.z);
	dGeomSetOffsetWorldRotation(geom[0], dRotation);
}

void RBX::Primitive::reset()
{
	if (!geom[0]) return;

	dGeomDestroy(geom[0]);
	geom[0] = 0;

	createPrimitive(shape, size);

	if (body)
	{
		body->attachPrimitive(this);
	}
}

void RBX::Primitive::createPrimitive(RBX::Shape primitive, Vector3 size)
{
	Vector3 position = pv->position.translation;
	Matrix3 rotation = pv->position.rotation;

	if (geom[0]) return;

	this->shape = primitive;

	switch (shape)
		{
		case ball:
		{
			geom[0] = dCreateSphere(Kernel::get()->space, size.y);
			break;
		}
		default:
		{
			geom[0] = dCreateBox(Kernel::get()->space, size.x * 2, size.y * 2, size.z * 2);
			break;
		}
	}

	float dRotation[12] = { rotation[0][0], rotation[0][1], rotation[0][2], 0, rotation[1][0], rotation[1][1], rotation[1][2], 0, rotation[2][0], rotation[2][1], rotation[2][2], 0 };

	dGeomSetPosition(geom[0], position.x, position.y, position.z);
	dGeomSetRotation(geom[0], dRotation);


}

void RBX::Primitive::step()
{
	CoordinateFrame coord;

	const dReal* position;
	const dReal* rotation;

	if (body)
	{
		body->step();
		pv->velocity = body->pv->velocity;
	}

	position = dGeomGetPosition(geom[0]);
	coord.translation = Vector3(position[0], position[1], position[2]);

	rotation = dGeomGetRotation(geom[0]);
	coord.rotation = Matrix3(rotation[0], rotation[1], rotation[2],
		rotation[4], rotation[5], rotation[6],
		rotation[8], rotation[9], rotation[10]);

	pv->position = coord;

}

RBX::Primitive::Primitive(Body* body)
{
	this->body = body;
	this->pv = body->pv;
}

RBX::Primitive::Primitive()
{
	this->pv = new PV();
	this->body = 0;
}
