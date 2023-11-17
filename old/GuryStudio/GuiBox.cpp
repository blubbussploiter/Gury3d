#include "GuiRoot.h"

void RBX::Gui::GuiBox::render(RenderDevice* d)
{
	Vector2 end;
	Vector2 pos;

	if (!visible) return;

	end = (position + size);
	pos = position;

	Draw::box(Box(Vector3(pos.x, pos.y, 0), Vector3(end.x, end.y, 0)), d, background, outline);
}