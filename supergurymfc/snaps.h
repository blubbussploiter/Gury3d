#ifndef SNAPS_H
#define SNAPS_H

#include "joints.h"

namespace RBX
{

	class SnapConstraint : public btTypedConstraint
	{
	public:

	};

	class SnapJoint : public Joint
	{
	private:
		btRigidBody* rPart0, * rPart1, *_body;
		btCompoundShape* _shape;
	public:
		bool hasBeenCalledBroken;

		SnapJoint* getPreExistingSnap(RBX::PVInstance* p0);
		btTransform getPartCompoundPosition(RBX::PVInstance* p);

		btTransform getCenterOfMass();

		void breakJoint();
		void createJoint();

		bool snapBroken();

		SnapJoint()
		{
			type = JointType::Snap;
			rPart0 = 0;
			rPart1 = 0;
			part0 = 0;
			part1 = 0;
			_shape = 0;
			hasBeenCalledBroken = 0;
			setClassName("Snap");
			setName("Snap");
		}
		~SnapJoint() { breakJoint(); }
	};
}

#endif