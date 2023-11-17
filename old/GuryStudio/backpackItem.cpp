#include "backpack.h"
#include "players.h"

static RBX::Sound* ping = RBX::Sound::fromFile(GetFileInPath("/content/sounds/electronicpingshort.wav"));

void drawOutline(RenderDevice* rd, Vector2 from, Vector2 to, float thickness, Color3 color)
{
	Vector2 p0, p1, p2, p3, p4, p5, p6, p7;

	p0 = from;
	p1 = Vector2(from.x+thickness, from.y+to.y);

	p2 = Vector2(from.x + to.x, from.y);
	p3 = Vector2(p2.x - thickness, from.y+to.y);

	p4 = from;
	p5 = Vector2(from.x + to.x, from.y + thickness);

	p6 = Vector2(from.x, from.y + to.y);
	p7 = Vector2(from.x + to.x, p6.y-thickness);

	Draw::box(Box(Vector3(p0.x, p0.y, 0), Vector3(p1.x, p1.y, 0)), rd, color, Color4::clear());
	Draw::box(Box(Vector3(p2.x, p2.y, 0), Vector3(p3.x, p3.y, 0)), rd, color, Color4::clear());
	
	Draw::box(Box(Vector3(p4.x, p4.y, 0), Vector3(p5.x, p5.y, 0)), rd, color, Color4::clear());
	Draw::box(Box(Vector3(p6.x, p6.y, 0), Vector3(p7.x, p7.y, 0)), rd, color, Color4::clear());

}

void RBX::BackpackItem::getTextureId()
{
	switch (item->binType)
	{
		case SCRIPT_BIN: return;
		case GAMETOOL_BIN:
		{
			textureId = GetFileInPath("/content/textures/GameTool.png");
			break;
		}
		case CLONE_BIN:
		{
			textureId = GetFileInPath("/content/textures/Clone.png");
			break;
		}
		case ROCKET_BIN:
		{
			textureId = GetFileInPath("/content/textures/Rocket.png");
			break;
		}
	}
	if(!textureId.empty()) texture->texture = Texture::fromFile(textureId);
}

void RBX::BackpackItem::fromitem(HopperBin* bin)
{
	item = bin;
	title = bin->getName();
	getTextureId();
}

void RBX::BackpackItem::render(RenderDevice* rd)
{
	Vector2 pos;

	if (!frame && !number)
	{
		frame = new Gui::GuiBox();
		number = new Gui::GuiLabel();

		frame->size = Vector2(15, 15);
		frame->background = Color4(0.8f, 0.8f, 0.8f, 1.f);

		number->textColor = Color3::white();
		number->sz = 10;
	}

	pos = Vector2(position.x + 1, (position.y + size.y) - ((frame->size.y) + 1));

	frame->position = pos;
	number->position = pos + Vector2(2, 0);
	texture->position = position + Vector2(10, 10);
	f_outline->position = position + Vector2(8, 8);

	frame->render(rd);
	number->render(rd);
	GuiButton::render(rd);

	if (!hovered)
	{
		if (item->active)
		{
			drawOutline(rd, position, size, 4, Color3::green());
		}
		f_outline->background = Color4::clear();
	}
	else
	{
		if (item->active)
		{
			drawOutline(rd, position, size, 2, Color3::green());
		}
		f_outline->background = Color3::yellow();
	}

	if (clicked)
	{
		f_outline->background = Color3::blue();
	}
	else
	{
		if(!hovered)
			f_outline->background = Color4::clear();
	}

	if (!texture->texture.isNull())
	{
		f_outline->render(rd);
		texture->render(rd);

		hoverColor = background;
		clickedColor = background;

		title.clear();
	}
	else
	{
		title = item->getName();
		clickedColor = BACKPACK_ITEM_CLICKED_COLOR;
		hoverColor = BACKPACK_ITEM_HOVER_COLOR;
	}

}

void RBX::onClickFn(RBX::Gui::GuiButton* b) /* move this to Backpack::activateBin() */
{
	RBX::BackpackItem* bitem = (RBX::BackpackItem*)(b);
	RBX::HopperBin* item = bitem->getItem();
	RBX::Network::Player* player = RBX::Network::getPlayers()->localPlayer;

	RBX::HopperBin* activeBin;
	RBX::BackpackItem* active;

	activeBin = player->activeBin;
	ping->play();

	if (!player)
		return;

	if (!item->active)
	{
		if (player->activeBin)
		{
			active = player->backpack->getBackpackItem(player->activeBin);
			activeBin->deactivate();
			activeBin->active = 0;
		}

		player->activeBin = item;
		item->activate();
		item->active = 1;
	}
	else
	{
		activeBin->deactivate();
		player->activeBin = 0;
		item->active = 0;
	}
}

void RBX::BackpackItem::handleMouse(G3D::UserInput* ui)
{
	RBX::Gui::GuiButton::handleMouse(ui);
}