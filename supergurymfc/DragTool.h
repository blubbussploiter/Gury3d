#pragma once

#include "StudioTool.h"
#include "ISelectable.h"

namespace RBX
{
	namespace Studio
	{
		class DragTool : public StudioTool
		{
		public:
			void getAccordingY(ISelectable* s, float& y);
			void doGraphics(RenderDevice* rd);
			void doLogic(UserInput* userInput);
		};
	}
}