#include "GuiRoot.h"

/* quick and dirty only used in backpack gui */

void RBX::Gui::GuiImage::render(RenderDevice* d)
{
	unsigned int glid;

	TextureRef renderImage;

	renderImage = texture;
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
