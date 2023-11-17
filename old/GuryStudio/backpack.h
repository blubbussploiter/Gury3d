#ifndef BACKPACK_H
#define BACKPACK_H

#include "GuiRoot.h"
#include "hopperbin.h"

#include <map>

#define BACKPACK_ITEM_HOVER_COLOR  Color4(0.7f, 0.7f, 0.7f, 1.f);
#define BACKPACK_ITEM_CLICKED_COLOR Color3::yellow();

namespace RBX
{
	class BackpackItem;
	class HopperBin;

	extern void onClickFn(RBX::Gui::GuiButton* b);

	class Backpack : public RBX::Instance
	{
	private:
		std::map<HopperBin*, BackpackItem*> items;
	public:
		void keypress(G3D::UserInput* ui);
		void updateGui(RenderDevice* rd, G3D::UserInput* ui);
		void removeBackpackItem(HopperBin* b) { items.erase(b); }
		BackpackItem* getBackpackItem(HopperBin* b) { return items[b]; }
		BackpackItem* createBackpackItem(HopperBin* item);
		Backpack(RBX::Instance* player)
		{
			setName("Backpack");
			setClassName("Backpack");
			setParent(player);
			isParentLocked = 1;
		}
		virtual ~Backpack() {}
	};

	class BackpackItem : public RBX::Gui::GuiButton
	{
	private:
		HopperBin* item;
	public:
		Gui::GuiBox* frame, *f_outline;
		Gui::GuiImage* texture;
		Gui::GuiLabel* number;
	public:
		std::string textureId;
		HopperBin* getItem() { return item; }
		void getTextureId();
		void fromitem(HopperBin* bin);
		void render(RenderDevice* rd);
		void handleMouse(G3D::UserInput* ui);
		BackpackItem()
		{
			alignBottom = 1;
			texture = new RBX::Gui::GuiImage();
			f_outline = new RBX::Gui::GuiBox();
			f_outline->size = Vector2(50, 50);
			f_outline->background = Color4::clear();
			texture->size = Vector2(45, 45);
			size = Vector2(65, 65);
			position = Vector2(0, 65);
			titleOff = Vector2(0, 20);
			titleColor = Color3::white();
			clickedColor = BACKPACK_ITEM_CLICKED_COLOR;
			hoverColor = BACKPACK_ITEM_HOVER_COLOR;
			sz = 10;
			onClick = onClickFn;
			setName("BackpackItem");
			setClassName("BackpackItem");
		}
	};
}

#endif