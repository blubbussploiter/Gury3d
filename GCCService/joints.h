#ifndef JOINTS_H
#define JOINTS_H

#pragma once

#include <G3DAll.h>

#include "btBulletDynamicsCommon.h"
#include "instance.h"

#define checkJoint() { if(!part0 || !part1 || \
(part0&& part1 && !part0->body->_body || !part1->body->_body)) return; }

namespace RBX
{
	class PVInstance;

	enum class JointType
	{
		None,
		Snap,
		Weld,
		Glue,
		Hinge,
		Motor
	};

	class Joint : public RBX::Instance
	{
	public:

		RBX::PVInstance* part0;
		RBX::PVInstance* part1;

		btTypedConstraint* constraint;
		CoordinateFrame c0, c1;

		JointType type;
		bool isCreated;

		virtual void breakJoint() {};
		virtual void createJoint() {};
		void make(RBX::PVInstance* p0, RBX::PVInstance* p1);

		Joint()
		{
			type = JointType::None;
			part0 = 0;
			part1 = 0;
			isCreated = 0;
			constraint = 0;
			setClassName("Joint");
			setName("Joint");
		}
	};
}

#endif