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
			void doLogic(UserInput* userInput);
		};
	}
}