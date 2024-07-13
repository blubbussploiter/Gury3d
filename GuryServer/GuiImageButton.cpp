#include "GuiRoot.h"

void RBX::Gui::GuiImageButton::render(RenderDevice* d)
{

	unsigned int glid;
	Vector2 pos;

	TextureRef renderImage;
	
	if (texture.isNull())
	{
		GuiButton::render(d);
		return;
	}

	width = d->width();
	height = d->height();

	if (origin.isZero())
		origin = position;

	renderImage = texture;
	position = getPosition(width, height, origin, alignRight, alignBottomRight, alignBottom);

	if (hovered && !clicked)
		renderImage = hoverTexture;
	
	if (clicked)
		renderImage = clickTexture;

	glid = renderImage->openGLID();

	d->push2D();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, glid);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0);
	glVertex2f(position.x, position.y);

	glTexCoord2d(1.0, 0.0);
	glVertex2f(position.x + size.x, position.y);

	glTexCoord2d(1.0, 1.0);
	glVertex2f(position.x + size.x, position.y + size.y);

	glTexCoord2d(0.0, 1.0);
	glVertex2f(position.x, position.y + size.y);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	d->pop2D();
}

void RBX::Gui::GuiImageButton::handleMouse(G3D::UserInput* ui)
{
	GuiButton::handleMouse(ui);
}
