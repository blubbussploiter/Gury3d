#include "selection.h"

#include "DragTool.h"
#include "Mouse.h"

void RBX::Studio::DragTool::getAccordingY(ISelectable* s, float& y)
{
	PVInstance* target;
	ISelectable* selected;

	std::vector<RBX::ISelectable*> selection;
	selection = RBX::Selection::selection;

	target = Mouse::getMouse()->getTargetWithIgnoreList<ISelectable>(selection);
	selected = toInstance<ISelectable>(target);

	if (!selected || selected == s) return;

	Vector3 size = target->getSize();
	Vector3 pos = target->getPosition();

	y = pos.y + size.y;
}

void RBX::Studio::DragTool::doGraphics(RenderDevice* rd)
{
}

void RBX::Studio::DragTool::doLogic(UserInput* userInput)
{
	return;

	Vector3 hit;
	std::vector<RBX::ISelectable*> selected;
	PVInstance* target;

	bool mouseDown;
	mouseDown = userInput->keyDown(SDL_LEFT_MOUSE_KEY);

	selected = RBX::Selection::selection;

	target = Mouse::getMouse()->getTarget();
	hit = Mouse::getMouse()->hitWorld; /* no need to call getHit, getTarget already called! */

	isUsing = (mouseDown && selected.size() != 0);

	if (!isUsing || !mouseDown) return;

	Vector3 translation;
	float y;

	for (unsigned int i = 0; i < selected.size(); i++)
	{
		if (!isFinite(hit.x) || !isFinite(hit.y) || !isFinite(hit.z)) return;

		ISelectable* s = selected.at(i);

		y = ceil(hit.y);
		getAccordingY(s, y);

		translation = Vector3(ceil(hit.x), y, ceil(hit.z));

		s->setCenter(translation);
	}
}
