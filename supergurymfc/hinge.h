#pragma once

#include "jointsservice.h"

namespace RBX
{
	class HingeConnector : public Connector
	{
	private:

		Body* body0, * body1;
		dJointID hinge;

	public:

		void build();

		HingeConnector(Primitive* prim0, Primitive* prim1, NormalId connectedAt) : Connector(prim0, prim1, connectedAt)
		{
			body0 = prim0->body;
			body1 = prim1->body;
			hinge = 0;
			setName("Hinge");
			setClassName("Hinge");
		}
	};
}