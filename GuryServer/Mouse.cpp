#include "Mouse.h"
#include "camera.h"
#include "players.h"
#include "humanoid.h"
#include "appmanager.h"
#include "ray.h"
#include "strings.h"
#include "selection.h"

TextureRef cursor_far;
TextureRef cursor_close;
TextureRef cursor_clicker;

float szx = 150, szy = 150;

RBX::Mouse* mouse;
HCURSOR oldCursor;

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

bool RBX::Mouse::inGuryWindow()
{
	RECT rect;
	HWND currentHWND;
	RBX::Experimental::Application* app;

	app = AppManager::singleton()->getApplication();
	currentHWND = app->parent;

	GetClientRect(currentHWND, &rect);

	return (x >= rect.left && y >= rect.top && (x < rect.right && y < rect.bottom));
}

void RBX::Mouse::update(UserInput* ui)
{
	RBX::Camera* camera = RBX::Camera::singleton();

	updateCursorInfo();

	if (ui->keyDown(SDL_RIGHT_MOUSE_KEY)) return;

	x = ui->getMouseX();
	y = ui->getMouseY();
	cx = x - szx / 2;
	cy = y - szy / 2;
}

void RBX::Mouse::updateCursorInfo()
{
	if (cursor_clicker.isNull() || cursor_close.isNull()) return;
	if (getTarget() && !target->locked)
	{
		currentglId = cursor_clicker->getOpenGLID();
	}
	else
	{
		currentglId = cursor_close->getOpenGLID();
	}
}

void RBX::Mouse::render(RenderDevice* rd)
{
	if (cursor_far.isNull())
	{
		cursor_far = Texture::fromFile(GetFileInPath("/content/textures/ArrowFarCursor.png"));
		cursor_close = Texture::fromFile(GetFileInPath("/content/textures/ArrowCursor.png"));
		cursor_clicker = Texture::fromFile(GetFileInPath("/content/textures/DragCursor.png"));
		currentglId = cursor_close->getOpenGLID();
	}

	if (!inGuryWindow()) return;

	rd->push2D();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, currentglId);
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
