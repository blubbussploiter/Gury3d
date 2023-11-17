#ifndef HINGE_H
#define HINGE_H

#include "joints.h"

namespace RBX
{
	class HingeJoint : public RBX::Joint
	{
	public:
		HingeJoint()
		{
			setName("Hinge");
			setClassName("Hinge");
		}
	};
}

#endif