#pragma once

#include "jointsservice.h"

namespace RBX
{
	class SnapConnector : public Connector
	{
	private:
		Array<Primitive*>* primitives;

		Body* body0, *body1;
		Body* connector;

	public:

		SnapConnector* getConnectingSnap(Primitive* prim);

		void build();
		void unlink();

		SnapConnector(Primitive* prim0, Primitive* prim1, NormalId connectedAt) : Connector(prim0, prim1, connectedAt)
		{
			primitives = new Array<Primitive*>();
			setClassName("Snap");
			setName("Snap");
		}

	};
}