#include "GuiRoot.h"

RBX::Gui::GuiRoot* root;

Vector2 RBX::Gui::getPosition(float width, float height, Vector2 position, bool alignRight, bool alignBottomRight, bool alignBottom)
{

	if (alignBottomRight)
		return Vector2(width - position.x, height - position.y);

	if (alignBottom)
		return Vector2(position.x, height-position.y);

	return position;
}

RBX::Gui::GuiRoot* RBX::Gui::singleton()
{
	if (!root)
		root = new RBX::Gui::GuiRoot();
	return root;
}

void RBX::Gui::GuiRoot::render(RenderDevice* d)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects.at(i)->render(d);
}

void RBX::Gui::GuiRoot::doButtonLogic(G3D::UserInput* ui, RenderDevice* d)
{
	for (unsigned int i = 0; i < objects.size(); i++)
		if (objects.at(i)->isButton)
			((GuiButton*)(objects.at(i)))->handleMouse(ui);
}