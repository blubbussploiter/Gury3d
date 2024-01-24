
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"

#include "selection.h"
#include "Mouse.h"

#include "stdout.h"

std::vector<RBX::ISelectable*> RBX::Selection::selection = std::vector< RBX::ISelectable*>();
bool RBX::Selection::down = 0;
bool RBX::Selection::clicked = 0;
bool RBX::Selection::canSelect = 0;
bool RBX::Selection::multiSelect = 0;
Vector2 RBX::Selection::selectionDragBoxStart = Vector2::zero();
Vector2 RBX::Selection::selectionDragBoxEnd = Vector2::zero();

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
	target = RBX::Mouse::getTarget();

	bool ctrlShift = ui->keyDown(SDLK_LCTRL) || ui->keyDown(SDLK_LSHIFT)
		|| ui->keyDown(SDLK_RCTRL) || ui->keyDown(SDLK_LCTRL);

	clicked = ui->keyPressed(SDL_LEFT_MOUSE_KEY);
	down = ui->keyDown(SDL_LEFT_MOUSE_KEY);

	if (down)
	{
		selectionDragBoxEnd = ui->getMouseXY();
	}

	if (clicked)
	{
		selectionDragBoxStart = ui->getMouseXY();
	}
	
	multiSelect = (down && !clicked || ctrlShift);

	if (down || clicked)
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
					CMainFrame::mainFrame->m_wndClassView.SelectInstance(target);
					return;
				}
			}
			//else return;
		}
		if (clicked)
		{
			selection.clear();
			CMainFrame::mainFrame->m_wndClassView.SelectInstance(0);
		}
	}
}
