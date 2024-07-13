#pragma once

#include "jointsservice.h"

namespace RBX
{
	class MotorConnector : public Connector
	{
	private:

		Body* body0, * body1;
		dJointID motor;

	public:

		void build();

		MotorConnector(Primitive* prim0, Primitive* prim1, NormalId connectedAt) : Connector(prim0, prim1, connectedAt)
		{
			body0 = prim0->body;
			body1 = prim1->body;
			motor = 0;
			setName("Motor");
			setClassName("Motor");
		}
	};
}