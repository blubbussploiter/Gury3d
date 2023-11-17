#ifndef SNAPS_H
#define SNAPS_H

#include "joints.h"

namespace RBX
{
	class SnapJoint : public Joint
	{
	private:
		btRigidBody* rPart0, * rPart1, *_body;
		btCompoundShape* _shape;
	public:
		bool hasBeenCalledBroken;
		SnapJoint* getPreExistingSnap(RBX::PVInstance* p0);
		btTransform getPartCompoundPosition(RBX::PVInstance* p);
		void addShape(RBX::PVInstance* pv, btTransform offset, btCompoundShape* _shape, btCollisionShape* n);
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
			isAffectedByPhysics = 1;
			hasBeenCalledBroken = 0;
			setClassName("Snap");
			setName("Snap");
		}
		~SnapJoint() { breakJoint(); }
	};
}

#endif