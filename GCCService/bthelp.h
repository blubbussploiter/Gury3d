#ifndef BTHELP_H
#define BTHELP_H

#include <G3DAll.h>
#include "btBulletDynamicsCommon.h"

#define g3Vector3(g) (btVector3(g.x, g.y, g.z))
#define b3Vector3(b) (Vector3(b.x(), b.y(),b.z()))

static CoordinateFrame ToCoordinateFrame(btTransform transform)
{
	btVector3 physPosition;
	btMatrix3x3 physRotation;

	Matrix3 rotation;
	Vector3 position;

	physRotation = transform.getBasis();
	physPosition = transform.getOrigin();

	position = b3Vector3(physPosition);

	rotation = Matrix3(physRotation[0].x(), physRotation[0].y(), physRotation[0].z(),
		physRotation[1].x(), physRotation[1].y(), physRotation[1].z(),
		physRotation[2].x(), physRotation[2].y(), physRotation[2].z());

	return CoordinateFrame(rotation, position);
}

static btTransform ToTransform(CoordinateFrame cframe)
{
	btVector3 physPosition;
	btQuaternion physRotation;

	Vector3 position;
	float y, p, r;

	cframe.rotation.toEulerAnglesYXZ(y, p, r);
	position = cframe.translation;

	physPosition = g3Vector3(position);
	physRotation = btQuaternion();

	physRotation.setEuler(y, p, r);
	return btTransform(physRotation, physPosition);
}

#endif