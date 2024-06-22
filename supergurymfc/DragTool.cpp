#include "selection.h"

#include "DragTool.h"
#include "Mouse.h"

void RBX::Studio::DragTool::onDrag()
{
	Selection* selectionService;
	selectionService = Selection::get();
	
	if (isDragging)
	{
		selectionService->canSelect = false;
	}
	else
	{
		selectionService->canSelect = true;
	}
}

void RBX::Studio::DragTool::doLogic(UserInput* userInput)
{
	Selection* selectionService;
	selectionService = Selection::get();

	ISelectableArray selection = selectionService->selection;

	if (userInput->keyDown(SDL_LEFT_MOUSE_KEY) &&
		selection.size() > 0)
	{
		isDragging = true;
	}
	else
	{
		isDragging = false;
	}


}
