
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

#include "StudioTool.h"

#include "datamodel.h"

/* straight up not mine lmao, https://github.com/Vulpovile/Blocks3D/blob/develop/src/source/DataModelV2/SelectionService.cpp */

bool RBX::Selection::isSelected(ISelectable* i)
{
	auto b = selection.begin(), e = selection.end();
	return (std::find(b, e, i) != e);
}

void RBX::Selection::dragSelect()
{
	Vector2 a1(min(worldSelectStart.x, worldSelectEnd.x), min(worldSelectStart.y, worldSelectEnd.y));
	Vector2 a2(max(worldSelectStart.x, worldSelectEnd.x), max(worldSelectStart.y, worldSelectEnd.y));

	Instances instances;
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

		Studio::StudioTool* currentTool;

		currentTool = Studio::current_Tool;

		if (selectionDragBoxEnd != Vector3::zero() && selectionDragBoxStart != Vector3::zero())
		{
			if (!currentTool || (currentTool && !currentTool->isUsing))
			{
				//Draw::box(Box(start, end), rd, Color4::clear(), Color3::gray());
			}
		}
	}
}

void RBX::Selection::renderSelected(RenderDevice* rd, ISelectable* selection)
{
	ISelectable::SelectableBox box;
	box = selection->getBoundingBox();
	rd->setObjectToWorldMatrix(box.cframe);
	Primitives::drawOutline(rd, -box.size, box.size);
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
	if (!canSelect) return;

	RBX::PVInstance* target;
	CoordinateFrame cframe;

	Studio::StudioTool* currentTool;

	currentTool = Studio::current_Tool;
	target = Mouse::getMouse()->getTarget();
	cframe = Camera::singleton()->getCoordinateFrame();

	bool ctrlShift = ui->keyDown(SDLK_RSHIFT) || ui->keyDown(SDLK_LSHIFT)
		|| ui->keyDown(SDLK_RCTRL) || ui->keyDown(SDLK_LCTRL);

	clicked = ui->keyPressed(SDL_LEFT_MOUSE_KEY);
	down = ui->keyDown(SDL_LEFT_MOUSE_KEY);

	multiSelect = (down && !clicked || ctrlShift);

	if (down)
	{
		Vector3 rel = cframe.pointToObjectSpace(Mouse::getMouse()->getHit());
		worldSelectEnd = Vector2(rel.x / -rel.z, rel.y / -rel.z);
		selectionDragBoxEnd = ui->getMouseXY();
	}
	if (clicked)
	{
		Vector3 rel = cframe.pointToObjectSpace(Mouse::getMouse()->getHit());
		worldSelectStart = Vector2(rel.x / -rel.z, rel.y / -rel.z);
		selectionDragBoxStart = ui->getMouseXY();
	}

	if (!currentTool || (currentTool && !currentTool->isUsing))
	{
		//dragSelect();
	}

	if (clicked)
	{
		if (select(target, multiSelect))
		{
			return;
		}
		CMainFrame::mainFrame->m_wndClassView.SelectInstance(0);
		selection.clear();
	}
}

bool RBX::Selection::select(PVInstance* target, bool multiSelect)
{
	if (target)
	{
		if (!target->getLocked())
		{
			if (!isSelected(target))
			{
				if (!multiSelect)
				{
					selection.clear();
				}
				selection.push_back(target);
				select(target);
			}
			return 1;
		}
	}
	return 0;
}

void RBX::Selection::select(RBX::ISelectable* selected, bool selectInExplorer)
{
	Instance* instance = dynamic_cast<Instance*>(selected);
	if (instance && selectInExplorer)
	{
		CMainFrame::mainFrame->m_wndClassView.SelectInstance(instance);
	}
	selection.push_back(selected);
}

RBX::Selection* RBX::Selection::get()
{
	return Datamodel::getDatamodel()->selectionService;
}
