#include "GuiRoot.h"

void RBX::Gui::GuiHint::render(RenderDevice* d)
{
	Vector2 end;
	Vector2 bounds;

	if (text.empty()) return;

	end = Vector2(d->width(), d->height());
	bounds = get()->font->get2DStringBounds(text);

	Draw::box(Box(Vector3(0, 0, 0), Vector3(end.x, end.y, 0)), d, Color4(0.5f, 0.5f, 0.5f, 0.3f), Color4::CLEAR);

	get()->font->draw2D(d, text, Vector2((end.x / 2) - (bounds.x / 2), end.y / 2), 12.f, Color3::white(), Color3::black());
}