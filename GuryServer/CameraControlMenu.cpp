#include "GuiRoot.h"

#include "sounds.h"
#include "appmanager.h"

void RBX::Gui::CameraPanMenu::onArrowUpBtnClick(GuiButton* btn)
{
	Camera::singleton()->tiltDown();
	Camera::switch3->play();
}

void RBX::Gui::CameraPanMenu::onArrowDwnBtnClick(GuiButton* btn)
{
	Camera::singleton()->tiltUp();
	Camera::switch3->play();
}

void RBX::Gui::CameraPanMenu::onZoomOutBtnClick(GuiButton* btn)
{
	Camera::singleton()->cam_zoom(0);
	Camera::switch3->play();
}

void RBX::Gui::CameraPanMenu::onZoomInBtnClick(GuiButton* btn)
{
	Camera::singleton()->cam_zoom(1);
	Camera::switch3->play();
}
