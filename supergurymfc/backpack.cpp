#include "backpack.h"
#include "GuiRoot.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<RBX::Backpack>("Backpack")
		.constructor<>();
}

void RBX::Backpack::keypress(G3D::UserInput* ui)
{

}

void RBX::Backpack::updateGui()
{
	for (unsigned int i = 0; i < getChildren()->size(); i++)
	{
		RBX::HopperBin* bin = dynamic_cast<RBX::HopperBin*>(getChildren()->at(i));
		if (bin)
		{
			if (!items[bin])
			{
				items[bin] = createBackpackItem(bin);
				RBX::Gui::get()->add(items[bin]);
			}
			else
			{
				//rd->push2D();
				if (items[bin]->number)
				{
					items[bin]->number->title = std::to_string(i + 1);
				}
				items[bin]->position.x = (i * 65);
				//items[bin]->handleMouse(ui);
				//items[bin]->render(rd);
				//rd->popState();
			}
		}
	}
}

RBX::BackpackItem* RBX::Backpack::createBackpackItem(HopperBin* item)
{
	RBX::BackpackItem* n = new RBX::BackpackItem();
	n->fromitem(item);
	return n;
}