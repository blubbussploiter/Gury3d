#pragma once

#include "StudioTool.h"
#include "ISelectable.h"

#include "pvinstance.h"

namespace RBX
{
	namespace Studio
	{
		class MoveTool : public StudioTool
		{
		private:
			bool isMoving;
			bool justReleased;
		public:

			void onMove();

			void doGraphics(RenderDevice* rd);
			void doLogic(UserInput* userInput);

		};
	}
}