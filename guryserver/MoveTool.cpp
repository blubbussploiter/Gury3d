
#include "selection.h"
#include "collisionTest.h"

#include "MoveTool.h"

void RBX::Studio::MoveTool::onMove()
{

}

void RBX::Studio::MoveTool::doGraphics(RenderDevice* rd)
{
	Selection* selectionService;
	selectionService = Selection::get();
	Instances selection = selectionService->selectionAsInstances();

	if (selection.size() == 1)
	{
		return;
	}


}

void RBX::Studio::MoveTool::doLogic(UserInput* userInput)
{

}
