#ifndef MOTOR_H
#define MOTOR_H

#include "joints.h"

namespace RBX
{
	class MotorJoint : public RBX::Joint
	{
	public:
		void createJoint();
		MotorJoint()
		{
			setName("Motor");
			setClassName("Motor");
		}
	};
}

#endif
