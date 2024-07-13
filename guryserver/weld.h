#pragma once

#include "snap.h"

namespace RBX
{
	class WeldConnector : public SnapConnector /* copycatter of weld, just different className*/
	{
	public:
		WeldConnector(Primitive* prim0, Primitive* prim1, NormalId connectedAt) : SnapConnector(prim0, prim1, connectedAt)
		{
			setClassName("Weld");
			setName("Weld");
		}
	};
}