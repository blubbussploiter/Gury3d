#include "GuiRoot.h"
#include "Mouse.h"

void RBX::Gui::GuiButton::render(RenderDevice* d)
{
	Vector2 end;
	Vector2 right;
	Vector2 bounds;

	Color4 textColor;
	Color4 boxColor;

	textColor = titleColor;

	if (origin.isZero())
		origin = position;

	position = getPosition(d->getWidth(), d->getHeight(), origin, alignRight, alignBottomRight, alignBottom);

	end = (position + size);
	bounds = singleton()->font->get2DStringBounds(title);

	right = Vector2(((end.x - (size.x))+5)+titleOff.x, (position.y)+titleOff.y);
	boxColor = Color4(0.5f, 0.5f, 0.5f, 0.3f);

	if (hovered && !disabled)
		boxColor = hoverColor;

	if (clicked)
		boxColor = clickedColor;

	if (disabled)
		textColor = disabledColor;

	Draw::box(Box(Vector3(position.x, position.y, 0), Vector3(end.x, end.y, 0)), d, boxColor, outline);

	singleton()->font->draw2D(d, title, right, sz, textColor);
}

void RBX::Gui::GuiButton::handleMouse(G3D::UserInput* ui)
{
	Vector2 mousePos;

	GuiObject::handleMouse(ui);
	mousePos = ui->mouseXY();

	if (disabled)
		return;

	if (!mouseIn(mousePos))
	{
		hovered = false;
		return;
	}

	/* Hovered */

	hovered = true;

	/* Clicked */

	if (ui->keyReleased(SDL_LEFT_MOUSE_KEY))
	{
		if (!clicked)
		{
			if (onClick)
				onClick(this);
		}
		clicked = true;
	}
	else
		clicked = false;
}