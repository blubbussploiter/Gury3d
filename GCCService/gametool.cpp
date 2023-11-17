
#include "Mouse.h"
#include "gametool.h"
#include "selection.h"
#include "ray.h"

TextureRef dragCursor, grabRotateCursor;

float getOccludingY(RBX::PVInstance* p)
{
	RBX::PVInstance* target;
	target = RBX::Mouse::getTarget(p);
	if (target && (target != p))
	{ 
		float y;
		y = ceil(p->getSize().z / 2) + 1.2;
		return target->getPosition().y + y;
	}
	return p->getPosition().y;
}

void RBX::GameTool::update(RenderDevice* rd, G3D::UserInput* ui)
{
	/*
	RBX::PVInstance* pv = RBX::Selection::selection, *i;
	Vector3 vec, hit, pos;

	if (!RBX::Selection::down)
	{
		i = RBX::Mouse::getTarget();
		if (dragged)
		{
			if (dragged->body && dragged->body->_body)
			{
				dragged->body->_body->setActivationState(1);
			}
			dragged = 0;
		}
		if (active)
		{
			if (!i || (i && i->getLocked())) Rendering::cursor_custom = 0;
			if (i && !i->getLocked())
				Rendering::cursor_custom = dragCursor;
		}
		return;
	}

	if (pv)
	{
		Vector3 originrot;

		lastPosition = pv->getPosition();
		originrot = pv->getEulerRotation();
		hit = RBX::Mouse::getHit();

		if (!pv->body || (pv->body && !pv->body->_body)) return;
		if (!isFinite(hit.x) || !isFinite(hit.y) || !isFinite(hit.z)) return;

		hit.x = ceil(hit.x);
		hit.y = ceil(hit.y);
		hit.z = ceil(hit.z);

		vec.x = hit.x;
		vec.z = hit.z;

		if (((int)pv->getSize().x) % 2 == 1)
			vec.x += 0.5;

		vec.y = getOccludingY(pv);

		if (((int)pv->getSize().z) % 2 == 1)
			vec.z += 0.5;

		vec.x = floor(vec.x);
		vec.y = floor(vec.y);
		vec.z = floor(vec.z);

		if (isNaN(hit.x) || isNaN(hit.y) || isNaN(hit.z)) return;

		pv->setPosition(vec);

		if (ui->keyReleased(SDLK_t))
		{
			
		}

		if (ui->keyReleased(SDLK_r))
		{

		}

		lastPosition = vec;
		dragged = pv;

		dragged->body->_body->setActivationState(0);

		Rendering::cursor_custom = grabRotateCursor;
	}
	*/
}

void RBX::GameTool::deactivate()
{
	/*
	RBX::Selection::canSelect = 0;
	RBX::Selection::selection = 0;
	RBX::Selection::down = 0;

	if (dragged)
	{
		RBX::Selection::selection = 0;
		if (dragged->body && 
			dragged->body->_body)
		{
			dragged->body->_body->setActivationState(1);
		}
		dragged = 0;
	}

	Rendering::cursor_custom = 0;
	*/
}

void RBX::GameTool::activate() /* g3d doesnt like static TextureRefs */
{
	/*
	RBX::Selection::canSelect = 1;
	if (dragCursor.isNull())
	{
		dragCursor = Texture::fromFile(GetFileInPath("/content/textures/DragCursor.png"));
	}
	if (grabRotateCursor.isNull())
	{
		grabRotateCursor = Texture::fromFile(GetFileInPath("/content/textures/GrabRotateCursor.png"));
	}
	*/
}
