#include "appmanager.h"
#include "players.h"

#include "stdout.h"
#include "soundservice.h"

RBX::AppManager* appManager;

void RBX::AppManager::initOneTimeAppliances()
{

	if (bOneTimeInitialized) return;
	bOneTimeInitialized = true;

	RBX::initSurfaces();
	RBX::SoundService::singleton()->init();

	RBX::Gui::GuiList* playerList;

	RBX::Gui::GuiButton* toolboxBtn;
	RBX::Gui::GuiButton* editModeBtn;
	RBX::Gui::GuiButton* helpBtn;
	RBX::Gui::GuiButton* fullscreenBtn;
	RBX::Gui::GuiButton* exitBtn;

	RBX::Gui::GuiImageButton* arrowDownBtn;
	RBX::Gui::GuiImageButton* arrowUpBtn;
	RBX::Gui::GuiImageButton* zoomInBtn;
	RBX::Gui::GuiImageButton* zoomOutBtn;

	/* Menu bar */

	toolboxBtn = new RBX::Gui::GuiButton();
	toolboxBtn->position = Vector2(0, 0);
	toolboxBtn->size = Vector2(120, 20);
	toolboxBtn->title = "Toolbox";
	toolboxBtn->disabled = true;

	editModeBtn = new RBX::Gui::GuiButton();
	editModeBtn->position = Vector2(120, 0);
	editModeBtn->size = Vector2(120, 20);
	editModeBtn->title = "Edit Mode";
	editModeBtn->disabled = true;

	helpBtn = new RBX::Gui::GuiButton();
	helpBtn->position = Vector2(240, 0);
	helpBtn->size = Vector2(120, 20);
	helpBtn->title = "Help...";

	fullscreenBtn = new RBX::Gui::GuiButton();
	fullscreenBtn->position = Vector2(360, 0);
	fullscreenBtn->size = Vector2(120, 20);
	fullscreenBtn->title = "Fullscreen";
	fullscreenBtn->onClick = RBX::Gui::MenuBar::onFullscreenBtnClick;

	exitBtn = new RBX::Gui::GuiButton();
	exitBtn->position = Vector2(480, 0);
	exitBtn->size = Vector2(120, 20);
	exitBtn->title = "Exit";
	exitBtn->onClick = RBX::Gui::MenuBar::onExitBtnClick;

	/* Playerlist */

	playerList = new RBX::Gui::GuiList();
	playerList->position = Vector2(150, 10);
	playerList->listTitle = "Player List";
	playerList->size = Vector2(150, 0);
	playerList->visible = false;
	playerList->alignRight = true;

	/* CameraPanMenu */

	arrowUpBtn = new RBX::Gui::GuiImageButton();
	arrowUpBtn->alignBottomRight = true;
	arrowUpBtn->position = Vector2(70, 70);
	arrowUpBtn->size = Vector2(30, 30);
	arrowUpBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltUp.png"));
	arrowUpBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltUp_ovr.png"));
	arrowUpBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltUp_dn.png"));
	arrowUpBtn->onClick = RBX::Gui::CameraPanMenu::onArrowUpBtnClick;

	arrowDownBtn = new RBX::Gui::GuiImageButton();
	arrowDownBtn->alignBottomRight = true;
	arrowDownBtn->position = Vector2(70, 40);
	arrowDownBtn->size = Vector2(30, 30);
	arrowDownBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltDown.png"));
	arrowDownBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltDown_ovr.png"));
	arrowDownBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltDown_dn.png"));
	arrowDownBtn->onClick = RBX::Gui::CameraPanMenu::onArrowDwnBtnClick;

	zoomInBtn = new RBX::Gui::GuiImageButton();
	zoomInBtn->alignBottomRight = true;
	zoomInBtn->position = Vector2(40, 70);
	zoomInBtn->size = Vector2(30, 30);
	zoomInBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomIn.png"));
	zoomInBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomIn_ovr.png"));
	zoomInBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomIn_dn.png"));
	zoomInBtn->onClick = RBX::Gui::CameraPanMenu::onZoomInBtnClick;

	zoomOutBtn = new RBX::Gui::GuiImageButton();
	zoomOutBtn->alignBottomRight = true;
	zoomOutBtn->position = Vector2(40, 40);
	zoomOutBtn->size = Vector2(30, 30);
	zoomOutBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomOut.png"));
	zoomOutBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomOut_ovr.png"));
	zoomOutBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomOut_dn.png"));
	zoomOutBtn->onClick = RBX::Gui::CameraPanMenu::onZoomOutBtnClick;

	RBX::Gui::singleton()->add(toolboxBtn);
	RBX::Gui::singleton()->add(editModeBtn);
	RBX::Gui::singleton()->add(helpBtn);
	RBX::Gui::singleton()->add(fullscreenBtn);
	RBX::Gui::singleton()->add(exitBtn);
	RBX::Gui::singleton()->add(playerList);
	RBX::Gui::singleton()->add(arrowDownBtn);
	RBX::Gui::singleton()->add(arrowUpBtn);
	RBX::Gui::singleton()->add(zoomInBtn);
	RBX::Gui::singleton()->add(zoomOutBtn);

	RBX::Gui::singleton()->initFont();

	RBX::Network::getPlayers()->setPlayerList(playerList); /* this may not be one time in the future */
}

void RBX::AppManager::start()
{
	if (!mainLoopStarted())
	{
		bMainLoopStarted = true;
		while (1)
		{
			if (currentApplication)
			{
				currentApplication->mainProcessStep();
			}
		}
	}
}

bool RBX::AppManager::mainLoopStarted()
{
	return bMainLoopStarted;
}

RBX::AppManager* RBX::AppManager::singleton()
{
	if (!appManager)
	{
		appManager = new RBX::AppManager();
	}
	return appManager;
}

RBX::Experimental::Application* RBX::AppManager::instantiate(HWND wnd)
{
	return new RBX::Experimental::Application(wnd);
}

RBX::Experimental::Application* RBX::AppManager::getApplication()
{
	return currentApplication;
}

void RBX::AppManager::setCurrentApplication(RBX::Experimental::Application* app)
{
	currentApplication = app;
}