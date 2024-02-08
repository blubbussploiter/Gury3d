
/* gury gury */

#include "Mouse.h"

#include "selection.h"
#include "scene.h"

#include "stdout.h"
#include "camera.h"

/* mfc stuff */

#include "pch.h"

#include "framework.h"
#include "MainFrm.h"

std::vector<RBX::ISelectable*> RBX::Selection::selection = std::vector< RBX::ISelectable*>();
bool RBX::Selection::down = 0;
bool RBX::Selection::clicked = 0;
bool RBX::Selection::canSelect = 0;
bool RBX::Selection::multiSelect = 0;
Vector2 RBX::Selection::selectionDragBoxStart = Vector2::zero();
Vector2 RBX::Selection::selectionDragBoxEnd = Vector2::zero();
Vector2 RBX::Selection::worldSelectStart = Vector2::zero();
Vector2 RBX::Selection::worldSelectEnd = Vector2::zero();

/* straight up not mine lmao, https://github.com/Vulpovile/Blocks3D/blob/develop/src/source/DataModelV2/SelectionService.cpp */

void drawOutline(Vector3 from, Vector3 to, RenderDevice* rd)
{
	Color3 outline = Color3::cyan();
	float offsetSize = 0.1F;

	Draw::box(Box(Vector3(from.x - offsetSize, from.y + offsetSize, from.z + offsetSize), Vector3(to.x + offsetSize, from.y - offsetSize, from.z - offsetSize)), rd, outline, Color4::clear());
	Draw::box(Box(Vector3(from.x - offsetSize, to.y + offsetSize, from.z + offsetSize), Vector3(to.x + offsetSize, to.y - offsetSize, from.z - offsetSize)), rd, outline, Color4::clear());
	Draw::box(Box(Vector3(from.x - offsetSize, to.y + offsetSize, to.z + offsetSize), Vector3(to.x + offsetSize, to.y - offsetSize, to.z - offsetSize)), rd, outline, Color4::clear());
	Draw::box(Box(Vector3(from.x - offsetSize, from.y + offsetSize, to.z + offsetSize), Vector3(to.x + offsetSize, from.y - offsetSize, to.z - offsetSize)), rd, outline, Color4::clear());

	Draw::box((Box(Vector3(from.x + offsetSize, from.y - offsetSize + 0.1, from.z + offsetSize), Vector3(from.x - offsetSize, to.y + offsetSize - 0.1, from.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box((Box(Vector3(to.x + offsetSize, from.y - offsetSize + 0.1, from.z + offsetSize), Vector3(to.x - offsetSize, to.y + offsetSize - 0.1, from.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box((Box(Vector3(to.x + offsetSize, from.y - offsetSize + 0.1, to.z + offsetSize), Vector3(to.x - offsetSize, to.y + offsetSize - 0.1, to.z - offsetSize))), rd, outline, Color4::clear());
	Draw::box((Box(Vector3(from.x + offsetSize, from.y - offsetSize + 0.1, to.z + offsetSize), Vector3(from.x - offsetSize, to.y + offsetSize - 0.1, to.z - offsetSize))), rd, outline, Color4::clear());

	Draw::box((Box(Vector3(from.x + offsetSize, from.y + offsetSize, from.z - offsetSize), Vector3(from.x - offsetSize, from.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());
	Draw::box((Box(Vector3(from.x + offsetSize, to.y + offsetSize, from.z - offsetSize), Vector3(from.x - offsetSize, to.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());
	Draw::box((Box(Vector3(to.x + offsetSize, from.y + offsetSize, from.z - offsetSize), Vector3(to.x - offsetSize, from.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());
	Draw::box((Box(Vector3(to.x + offsetSize, to.y + offsetSize, from.z - offsetSize), Vector3(to.x - offsetSize, to.y - offsetSize, to.z + offsetSize))), rd, outline, Color4::clear());

}

bool RBX::Selection::isSelected(ISelectable* i)
{
	auto b = selection.begin(), e = selection.end();
	return (std::find(b, e, i) != e);
}

void RBX::Selection::dragSelect()
{
	Vector2 a1(min(worldSelectStart.x, worldSelectEnd.x), min(worldSelectStart.y, worldSelectEnd.y));
	Vector2 a2(max(worldSelectStart.x, worldSelectEnd.x), max(worldSelectStart.y, worldSelectEnd.y));

	std::vector<RBX::Render::Renderable*> instances;
	instances = RBX::Scene::singleton()->getArrayOfObjects();

	for (unsigned int i = 0; i < instances.size(); i++)
	{
		RBX::Instance* instance = dynamic_cast<RBX::Instance*>(instances.at(i));
		RBX::PVInstance* child = toInstance<PVInstance>(instance);

		if (child)
		{
			CoordinateFrame cframe = Camera::singleton()->getCoordinateFrame();
			Vector3 objectSpace = cframe.pointToObjectSpace(child->getPosition());
			
			float x = objectSpace.x / -objectSpace.z;
			float y = objectSpace.y / -objectSpace.z;

			if ((a1.x < x && x < a2.x) && (a1.y < y && y < a2.y) && objectSpace.z < 0)
			{
				select(child, 1);
			}
		}
	}
}

void RBX::Selection::renderDragBox(RenderDevice* rd)
{
	if (down)
	{
		Vector3 start = selectionDragBoxStart;
		Vector3 end = selectionDragBoxEnd;

		Draw::box(Box(start, end), rd, Color4::clear(), Color3::gray());
	}
}

void RBX::Selection::renderSelected(RenderDevice* rd, ISelectable* selection)
{
	ISelectable::SelectableBox box;
	box = selection->getBoundingBox();
	rd->setObjectToWorldMatrix(box.cframe);
	drawOutline(-box.size, box.size, rd);
}

void RBX::Selection::renderSelection(RenderDevice* rd)
{
	for (RBX::ISelectable* pv : selection)
	{
		renderSelected(rd, pv);
	}
}

void RBX::Selection::update(UserInput* ui)
{
	RBX::PVInstance* target;
	CoordinateFrame cframe = Camera::singleton()->getCoordinateFrame();

	target = RBX::Mouse::getTarget();

	bool ctrlShift = ui->keyDown(SDLK_LCTRL) || ui->keyDown(SDLK_LSHIFT)
		|| ui->keyDown(SDLK_RCTRL) || ui->keyDown(SDLK_LCTRL);

	clicked = ui->keyPressed(SDL_LEFT_MOUSE_KEY);
	down = ui->keyDown(SDL_LEFT_MOUSE_KEY);

	if (down)
	{
		Vector3 rel = cframe.pointToObjectSpace(Mouse::getHit());
		worldSelectEnd = Vector2(rel.x / -rel.z, rel.y / -rel.z);
		selectionDragBoxEnd = ui->getMouseXY();
		dragSelect();
	}

	if (clicked)
	{
		Vector3 rel = cframe.pointToObjectSpace(Mouse::getHit());
		worldSelectStart = Vector2(rel.x / -rel.z, rel.y / -rel.z);
		selectionDragBoxStart = ui->getMouseXY();
	}
	
	multiSelect = (down && !clicked || ctrlShift);

	if (clicked && !hoveringUI)
	{
		if (select(target, multiSelect)) return;
		if (clicked)
		{
			selection.clear();
			CMainFrame::mainFrame->m_wndClassView.SelectInstance(0);
		}
	}
}

bool RBX::Selection::select(PVInstance* target, bool multiSelect)
{
	if (target)
	{
		if (!isSelected(target))
		{
			if (!target->getLocked())
			{
				if (!multiSelect)
				{
					selection.clear();
				}
				selection.push_back(target);
				select(target);
				return 1;
			}
		}
	}
	return 0;
}
