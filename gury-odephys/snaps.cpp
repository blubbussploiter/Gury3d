#include "snaps.h"
#include "pvinstance.h"
#include "jointservice.h"
#include "runservice.h"
#include "stdout.h"

RBX::SnapJoint* RBX::SnapJoint::getPreExistingSnap(RBX::PVInstance* p0)
{
	RBX::Joint* joint = p0->body->connector;
	if (joint && joint->getClassName() == "Snap")
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

}