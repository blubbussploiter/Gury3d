#ifndef SNAPS_H
#define SNAPS_H

#include "joints.h"

namespace RBX
{
	class SnapJoint : public Joint
	{
	private:

	public:
		bool hasBeenCalledBroken;
		SnapJoint* getPreExistingSnap(RBX::PVInstance* p0);
		void breakJoint();
		void createJoint();
		bool snapBroken();
		SnapJoint()
		{
			type = JointType::Snap;
			part0 = 0;
			part1 = 0;
			isAffectedByPhysics = 1;
			hasBeenCalledBroken = 0;
			setClassName("Snap");
			setName("Snap");
		}
		~SnapJoint() { breakJoint(); }
	};
}

#endif