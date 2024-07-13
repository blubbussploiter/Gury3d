#pragma once

#include "StudioTool.h"
#include "ISelectable.h"

#include "pvinstance.h"

namespace RBX
{
	namespace Studio
	{
		class DragTool : public StudioTool
		{
		private:
			bool isDragging;
			bool justReleased;
		public:

			void onDrag();

			void doGraphics(RenderDevice* rd) {}
			void doLogic(UserInput* userInput);

		};
	}
}