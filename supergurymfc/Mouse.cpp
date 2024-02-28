#include "Mouse.h"
#include "camera.h"
#include "players.h"
#include "humanoid.h"
#include "appmanager.h"
#include "ray.h"
#include "strings.h"

TextureRef cursor_far;
TextureRef cursor_close;

float szx = 150, szy = 150;

RBX::Mouse* mouse;

RBX::PVInstance* RBX::Mouse::getTarget() /* ignore part for dragger tool */
{
	RBX::Camera* camera = RBX::Camera::singleton();
	RBX::ISelectable* selected;
	Ray ray;

	ray = camera->camera->worldRay(x, y, RBX::AppManager::singleton()->getApplication()->getViewport());

	selected = RBX::World::getPartFromG3DRay<Instance>(ray, hitWorld);
	target = (RBX::PVInstance*)(selected);

	return target;
}

void RBX::Mouse::update(UserInput* ui)
{
	RBX::Camera* camera = RBX::Camera::singleton();
	if (ui->keyDown(SDL_RIGHT_MOUSE_KEY)) return;
	x = ui->getMouseX();
	y = ui->getMouseY();
	cx = x - szx / 2;
	cy = y - szy / 2;
}

void RBX::Mouse::render(RenderDevice* rd)
{
	if (cursor_far.isNull() && cursor_close.isNull())
	{
		cursor_far = Texture::fromFile(GetFileInPath("/content/textures/ArrowFarCursor.png"));
		cursor_close = Texture::fromFile(GetFileInPath("/content/textures/ArrowCursor.png"));
	}

	rd->push2D();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, cursor_close->openGLID());
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0);
	glVertex2f(cx, cy);

	glTexCoord2d(1.0, 0.0);
	glVertex2f(cx + szx,cy);

	glTexCoord2d(1.0, 1.0);
	glVertex2f(cx + szx,cy + szy);

	glTexCoord2d(0.0, 1.0);
	glVertex2f(cx, cy + szy);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	rd->pop2D();
}

RBX::Mouse* RBX::Mouse::getMouse()
{
	if (!mouse) mouse = new RBX::Mouse();
	return mouse;
}
