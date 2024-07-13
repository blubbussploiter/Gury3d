#ifndef SELECTION_H
#define SELECTION_H

#include <vector>

#include "ISelectable.h"
#include "instance.h"

#include "Mouse.h"

namespace RBX
{
	typedef std::vector<RBX::ISelectable*> ISelectableArray;

	class Selection : public RBX::Instance
	{
	public:

		ISelectableArray selection;

		Vector2 selectionDragBoxStart, selectionDragBoxEnd;
		Vector2 worldSelectStart, worldSelectEnd;

		bool clicked, down;
		bool canSelect, multiSelect;

		bool isSelected(ISelectable* i);

		Instances selectionAsInstances();

		void renderDragBox(RenderDevice* rd);

		void renderSelected(RenderDevice* rd, ISelectable* i);
		void renderSelection(RenderDevice* d);

		void dragSelect();

		void update(UserInput* ui);

		bool select(PVInstance* p, bool multiSelect);
		void deselect(RBX::ISelectable* pv)
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

		void deselectAll()
		{
			selection.clear();
		}

		void select(RBX::ISelectable* pv, bool selectInExplorer=1);

		static Selection* get();
	};
}

#endif