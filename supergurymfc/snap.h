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

		bool isAnchored;
	public:

		SnapConnector* getConnectingSnap(Primitive* prim);

		void build();

		SnapConnector(Primitive* prim0, Primitive* prim1) : Connector(prim0, prim1)
		{
			primitives = new Array<Primitive*>();
			setClassName("Snap");
			setName("Snap");
		}

	};
}