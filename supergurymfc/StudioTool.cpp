#include "StudioTool.h"
#include "DragTool.h"

RBX::Studio::StudioTool* RBX::Studio::current_Tool = 0;

std::map<RBX::Studio::ToolTypes, RBX::Studio::StudioTool*> tools =
{
	{RBX::Studio::ToolTypes::Mouse, new RBX::Studio::DragTool()}
};

void RBX::Studio::setToolFromType(ToolTypes type)
{
	current_Tool = tools[type];
}

bool RBX::Studio::currentToolType(ToolTypes type)
{
	return current_Tool == tools[type];
}
