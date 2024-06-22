#pragma once

#include "StudioTool.h"
#include "ISelectable.h"

namespace RBX
{
	namespace Studio
	{
		class DragTool : public StudioTool
		{
		private:
			bool isDragging;
		public:
			void onDrag();
			void doGraphics(RenderDevice* rd) {}
			void doLogic(UserInput* userInput);
		};
	}
}