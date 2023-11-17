#ifndef SELECTION_H
#define SELECTION_H

#include "instance.h"
#include "Mouse.h"

namespace RBX
{
	class Selection : public RBX::Instance
	{
	public:
		static PVInstance* selection;
		static bool clicked, down, canSelect;
		static void renderSelection(RenderDevice* d);
		static void update(UserInput* ui);
	};
}

#endif