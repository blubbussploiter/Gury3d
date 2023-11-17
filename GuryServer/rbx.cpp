
#include "rbx.h"
#include "part.h"

#include "GuiRoot.h"
#include "Mouse.h"

#include "players.h"
#include "camera.h"
#include "jointservice.h"

#include "workspace.h"
#include "sounds.h"

#include "runservice.h"
#include "serializer.h"

#include "rbxwindow.h"
#include "selection.h"

#include "clonebin.h"
#include "gametool.h"

#include "datamodel.h"
#include "lighting.h"

#include "debugDrawer.h"

#pragma comment(lib, "fmod_vc.lib")

RBX::G3DApp* RBX::global_app = 0;

void RBX::G3DApplet::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	RBX::Datamodel::getDatamodel()->step();
	RBX::SoundService::singleton()->update();
	RBX::Camera::singleton()->update(app);
}

void RBX::G3DApplet::doGraphics()
{

	app->renderDevice->clear();
	app->renderDevice->setProjectionAndCameraMatrix(app->debugCamera);

	app->renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);
	sky->render(RBX::Lighting::singleton()->getParameters());

	RBX::Selection::renderSelection(app->renderDevice);
	RBX::View::singleton()->render(app->renderDevice);

	RBX::Network::getPlayers()->updatePlayerList();

	app->renderDevice->push2D();

	RBX::Datamodel::getDatamodel()->guiRoot->render(app->renderDevice);
	Rendering::renderCursor(app->userInput, app->renderDevice);

	app->renderDevice->pop2D();

	sky->renderLensFlare(RBX::Lighting::singleton()->getParameters());
}

void RBX::G3DApplet::doLogic()
{
	UserInput* ui;
	ui = app->userInput;

	if (ui->keyPressed(SDLK_o))
	{
		RBX::Camera::singleton()->cam_zoom(0);
	}
	if (ui->keyPressed(SDLK_i))
	{
		RBX::Camera::singleton()->cam_zoom(1);
	}

	if (ui->appHasFocus())
	{
		RBX::Gui::singleton()->doButtonLogic(ui, app->renderDevice);
		RBX::updateControllers(app->userInput);
	}

	RBX::Selection::update(app->userInput);

}

void RBX::G3DApp::main()
{

	RBX::Camera::cameraInit(&debugCamera, renderDevice);
	RBX::initSurfaces();

	RBX::addController(RBX::Camera::singleton());

	RBX::RunService::singleton();
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
	playerList->position = Vector2(130, 10);
	playerList->listTitle = "Player List";
	playerList->size = Vector2(130, 0);
	playerList->visible = false;
	playerList->alignRight = true;

	/* CameraPanMenu */

	arrowUpBtn = new RBX::Gui::GuiImageButton();
	arrowUpBtn->alignBottomRight = true;
	arrowUpBtn->position = Vector2(70, 75);
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
	zoomInBtn->position = Vector2(40, 75);
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

	RBX::Network::getPlayers()->setPlayerList(playerList);

	applet = new G3DApplet(this);
	applet->setDesiredFrameRate(30);
	applet->run();

}