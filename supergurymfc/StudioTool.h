#pragma once

#include <map>
#include <G3DAll.h>

namespace RBX
{
	namespace Studio
	{
		enum ToolTypes
		{
			 Mouse,
			 Move,
			 Resize
		};

		class StudioTool
		{
		public:

			bool isUsing;
			 
			virtual void doLogic(UserInput* userInput) {}

			virtual void doGraphics(RenderDevice* renderDevice) {}

		};

		extern StudioTool* current_Tool;

		extern void setToolFromType(ToolTypes type);

		extern bool currentToolType(ToolTypes type);
	}
}