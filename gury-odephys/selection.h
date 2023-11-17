#ifndef SELECTION_H
#define SELECTION_H

#include <vector>

#include "ISelectable.h"
#include "instance.h"

#include "Mouse.h"

namespace RBX
{
	class Selection : public RBX::Instance
	{
	private:
		static std::vector<RBX::ISelectable*> selection;
	public:

		static Vector2 selectionDragBoxStart, selectionDragBoxEnd;

		static bool clicked, down;
		static bool canSelect, multiSelect;

		static bool isSelected(ISelectable* i);

		static void renderDragBox(RenderDevice* rd);

		static void renderSelected(RenderDevice* rd, ISelectable* i);
		static void renderSelection(RenderDevice* d);

		static void update(UserInput* ui);

		static void deselect(RBX::ISelectable* pv)
		{
			if (pv)
			{
				auto b = selection.begin(), e = selection.end();
				if (isSelected(pv))
				{
					selection.erase(std::remove(b, e, pv));
				}
			}
		}

		static void deselectAll()
		{
			selection.clear();
		}

		static void select(RBX::ISelectable* pv)
		{
			selection.push_back(pv);
		}
	};
}

#endif