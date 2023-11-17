#include "GuiRoot.h"

#include "sounds.h"
#include "appmanager.h"

void RBX::Gui::CameraPanMenu::onArrowUpBtnClick(GuiButton* btn)
{
	RBX::AppManager::singleton()->getApplication()->getCamera()->tiltDown(10.f);
}

void RBX::Gui::CameraPanMenu::onArrowDwnBtnClick(GuiButton* btn)
{
	RBX::AppManager::singleton()->getApplication()->getCamera()->tiltUp(10.f);
}

void RBX::Gui::CameraPanMenu::onZoomOutBtnClick(GuiButton* btn)
{
	RBX::AppManager::singleton()->getApplication()->getCamera()->cam_zoom(0);
}

void RBX::Gui::CameraPanMenu::onZoomInBtnClick(GuiButton* btn)
{
	RBX::AppManager::singleton()->getApplication()->getCamera()->cam_zoom(1);
}
