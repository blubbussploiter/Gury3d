#include "rocket.h"

void RBX::Rocket::step()
{
	RBX::PVInstance* part;
	CoordinateFrame CoordinateFrame;

	Vector3 force;
	Vector3 pAccel;
	Vector3 worldSpace;

	part = static_cast<RBX::PVInstance*>(getParent());
	if (part->getClassName() != "PVInstance") return;

	CoordinateFrame = part->getCFrame();
	worldSpace = CoordinateFrame.pointToWorldSpace(Vector3::zero());
}
