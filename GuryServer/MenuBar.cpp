#include "GuiRoot.h"
#include "rbxwindow.h"

bool inFullScreen = 0;
int x, y, width, height;

void setFullScreen(bool s)
{

}

void RBX::Gui::MenuBar::onExitBtnClick(RBX::Gui::GuiButton* btn)
{
	
}

void RBX::Gui::MenuBar::onFullscreenBtnClick(GuiButton* btn)
{

	if (!inFullScreen)
	{
		inFullScreen = 1;
		btn->title = "x Fullscreen";
	}
	else
	{
		inFullScreen = 0;
		btn->title = "Fullscreen";
	}

	setFullScreen(inFullScreen);

}
