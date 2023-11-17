#include "selection.h"
#include "Mouse.h"

RBX::PVInstance * RBX::Selection::selection = 0;
bool RBX::Selection::down = 0;
bool RBX::Selection::clicked = 0;
bool RBX::Selection::canSelect = 0;

/* straight up not mine lmao, https://github.com/Vulpovile/Blocks3D/blob/develop/src/source/DataModelV2/SelectionService.cpp */

void drawOutline(Vector3 from, Vector3 to, RenderDevice* rd, CoordinateFrame c)
{
	Color3 outline = Color3::cyan();
	float offsetSize = 0.1F;

	Draw::box(c.toWorldSpace(Box(Vector3(from.x - offsetSize, from.y + offsetSize, from.z + offsetSize), Vector3(to.x + offsetSize, from.y - offsetSize, from.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(from.x - offsetSize, to.y + offsetSize, from.z + offsetSize), Vector3(to.x + offsetSize, to.y - offsetSize, from.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(from.x - offsetSize, to.y + offsetSize, to.z + offsetSize), Vector3(to.x + offsetSize, to.y - offsetSize, to.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(from.x - offsetSize, from.y + offsetSize, to.z + offsetSize), Vector3(to.x + offsetSize, from.y - offsetSize, to.z - offsetSize))), rd, outline, Color4::clear());

	Draw::box(c.toWorldSpace(Box(Vector3(from.x + offsetSize, from.y - offsetSize + 0.1, from.z + offsetSize), Vector3(from.x - offsetSize, to.y + offsetSize - 0.1, from.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(to.x + offsetSize, from.y - offsetSize + 0.1, from.z + offsetSize), Vector3(to.x - offsetSize, to.y + offsetSize - 0.1, from.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(to.x + offsetSize, from.y - offsetSize + 0.1, to.z + offsetSize), Vector3(to.x - offsetSize, to.y + offsetSize - 0.1, to.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(from.x + offsetSize, from.y - offsetSize + 0.1, to.z + offsetSize), Vector3(from.x - offsetSize, to.y + offsetSize - 0.1, to.z - offsetSize))), rd, outline, Color4::clear());

	Draw::box(c.toWorldSpace(Box(Vector3(from.x + offsetSize, from.y + offsetSize, from.z - offsetSize), Vector3(from.x - offsetSize, from.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(from.x + offsetSize, to.y + offsetSize, from.z - offsetSize), Vector3(from.x - offsetSize, to.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(to.x + offsetSize, from.y + offsetSize, from.z - offsetSize), Vector3(to.x - offsetSize, from.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());
	Draw::box(c.toWorldSpace(Box(Vector3(to.x + offsetSize, to.y + offsetSize, from.z - offsetSize), Vector3(to.x - offsetSize, to.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());

}

void RBX::Selection::renderSelection(RenderDevice* rd)
{
	if (selection)
	{
		Vector3 size = selection->getSize();
		Vector3 pos = selection->getPosition();
		float div = 2.f, divY = 2.2f;
		switch (selection->shape)
		{
			case part:
			{
				break;
			}
			case ball:
			{
				div = 1;
				divY = 1;
				break;
			}
		}
		drawOutline(Vector3(0 + size.x / div, 0 + size.y / divY, 0 + size.z / div), Vector3(0 - size.x / div, 0 - size.y / divY, 0 - size.z / div), rd, selection->getCFrame());
	}
}

/* ok now straight up mine */

void RBX::Selection::update(UserInput* ui)
{
	RBX::PVInstance* target;
	target = RBX::Mouse::getTarget();

	if (ui->keyPressed(SDL_LEFT_MOUSE_KEY)) clicked = 1;
	if (ui->keyReleased(SDL_LEFT_MOUSE_KEY))
	{
		down = 0;
		selection = 0;
	}

	if (ui->keyDown(SDL_LEFT_MOUSE_KEY))
	{
		down = 1;
		if (!canSelect || selection) return;
		if (target && !target->getLocked())
		{
			selection = target;
		}
	}


}
