#include "Mouse.h"
#include "camera.h"
#include "players.h"
#include "humanoid.h"
#include "appmanager.h"
#include "ray.h"
#include "strings.h"

unsigned int Rendering::mouse_glid;
bool Rendering::isOverGuiObject;

Vector3 RBX::Mouse::dir = Vector3::zero();
Vector3 RBX::Mouse::hitWorld = Vector3::zero();

RBX::PVInstance* RBX::Mouse::target = 0;
TextureRef Rendering::cursor_custom = 0;

TextureRef cursor_far;
TextureRef cursor_close;

float szx = 150, szy = 150;
float cx, cy, x, y;

bool Rendering::shouldRenderAsFar()
{
	return cursor_custom.isNull();
}

RBX::PVInstance* RBX::Mouse::getTarget(RBX::PVInstance* ignorePart) /* ignore part for dragger tool */
{
	RBX::Camera* camera = RBX::Camera::singleton();
	RBX::ISelectable* selected;
	Ray ray;

	ray = camera->camera->worldRay(x, y, RBX::AppManager::singleton()->getApplication()->getViewport());

	selected = RBX::World::getPartFromG3DRay(ray, hitWorld);
	target = dynamic_cast<RBX::PVInstance*>(selected);

	return target;
}

void RBX::Mouse::update(UserInput* ui)
{
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
}