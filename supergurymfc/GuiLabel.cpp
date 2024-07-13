#include "GuiRoot.h"

void RBX::Gui::GuiLabel::render(RenderDevice* d)
{
	if (!visible) return;

	if (alignRight)
	{
		Vector2 pos;
		Vector2 bounds;

		bounds = get()->font->get2DStringBounds(title, sz);
		pos = Vector2(d->width() - (position.x+bounds.x/2), position.y);

		get()->font->draw2D(d, title, pos, sz, textColor, outlineColor);
		return;
	}

	get()->font->draw2D(d, title, position, sz, textColor, outlineColor);
}