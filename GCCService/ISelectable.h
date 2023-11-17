#pragma once

#include <G3DAll.h>

namespace RBX
{
	class ISelectable
	{
	public:
		virtual Box getBoundingBox() { return Box(); };
	};
}