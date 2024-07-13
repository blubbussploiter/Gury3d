
#include "studio/pch.h"
#include "studio/StudioTool.h"
#include "studio/MainFrm.h"

#include "appmanager.h"

#include "selection.h"
#include "players.h"

#include "camera.h"
#include "stdout.h"

#include "Mouse.h"
#include "view.h"

#include "networkclient.h"
#include "scriptcontext.h"

#include "soundservice.h"
#include "diagnosticsWorldDrawer.h"

RBX::Datamodel* RBX::Experimental::Application::getDatamodel()
{
	if (!datamodel)
	{
		datamodel = new RBX::Datamodel();
		datamodel->open();
	}
	return datamodel;
}

RBX::Camera* RBX::Experimental::Application::getCamera()
{
	if (!camera)
	{
		camera = new RBX::Camera();
		camera->setCamera(new GCamera());
	}
	return camera;
}


void RBX::Experimental::Application::doUserInput()
{
	GEvent e;

	userInput->beginEvents();

	while (pollEvent(mouse, e) || pollEvent(key, e))
	{
		if (mouse.message != 0)
		{
			mouse.message = 0;
		}

		if (key.message != 0)
		{
			key.message = 0;
		}

		userInput->processEvent(e);
	}

	window->pollEvent(e);

	userInput->endEvents();
}

void RBX::Experimental::Application::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	getDatamodel()->step(idt);
	SoundService::get()->update();
}

void RBX::Experimental::Application::onLogic()
{
	if (userInput->keyPressed(SDLK_o))
	{
		RBX::Camera::get()->cam_zoom(0);
	}
	if (userInput->keyPressed(SDLK_i))
	{
		RBX::Camera::get()->cam_zoom(1);
	}
	if (RBX::Studio::current_Tool)
	{
		RBX::Studio::current_Tool->doLogic(userInput);
	}

	Mouse::getMouse()->update(userInput);
	getCamera()->update(userInput->keyDown(SDL_RIGHT_MOUSE_KEY));

	/* todo: add something that puts the window into focus if it gets clicked on... for escaping explorer wnd grasps */

	RBX::Gui::get()->doButtonLogic(userInput, renderDevice);
	RBX::Network::getPlayers()->onStep();

	RBX::ControllerService::get()->updateControllers(userInput);
	RBX::Selection::get()->update(userInput);

}

void RBX::Experimental::Application::onFocus()
{
	inFocus = 1;
	justReceivedFocus = 1;
}

void RBX::Experimental::Application::onKillFocus()
{
	inFocus = 0;
	justReceivedFocus = 0;

	mouse.message = 0;
	mouse.wParam = 0;
	mouse.lParam = 0;

	key.message = 0;
	key.wParam = 0;
	key.lParam = 0;
}

void RBX::Experimental::Application::onGraphics()
{
	View::get()->oneFrame(renderDevice, getCamera(), sky);
}

void RBX::Experimental::Application::onInit()
{
	Diagnostics::get_Renderer()->diagnostics_enabled = false;

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "Application::onInit()");
	RBX::AppManager::get()->initOneTimeAppliances();

	setWindowLong();

	getCamera();
	getDatamodel();

	RBX::ControllerService::get()->mapControllers(userInput);
	RBX::ControllerService::get()->addController(RBX::Camera::get());
	
	RBX::ScriptContext* context = getDatamodel()->scriptContext;
	context->openState();
	
	inEditMode = true;

	if (!rbxlFile.empty())
	{
		getDatamodel()->name = RBX::AppManager::get()->fileName;
		getDatamodel()->loadContent(rbxlFile);
	}

}

void RBX::Experimental::Application::exitEditMode(bool _inEditMode)
{
	CMainFrame* mainFrame = CMainFrame::mainFrame;
	if (!mainFrame) return;

	inEditMode = _inEditMode;
	int n;
	n = inEditMode ? SW_RESTORE : SW_HIDE;

	mainFrame->m_wndMainTools.ShowWindow(n);
	mainFrame->m_wndCameraTools.ShowWindow(n);
	mainFrame->m_wndClassView.ShowWindow(n);
	mainFrame->m_wndStudioTools.ShowWindow(n);
	mainFrame->m_wndRunServiceTools.ShowWindow(n);
	mainFrame->m_wndProperties.ShowWindow(n);
	mainFrame->m_wndOutput.ShowWindow(n);
	mainFrame->m_wndMenuBar.ShowWindow(n);
	mainFrame->m_wndStatusBar.ShowWindow(n);

	if (inEditMode)
	{
		ShowWindow(parent, SW_RESTORE);
	}
	else
	{
		ShowWindow(parent, SW_MAXIMIZE);
	}

	resizeWithParent();
}

void RBX::Experimental::Application::mainProcessStep()
{

	if (isThinking)
	{
		RealTime desiredFrameDuration = 1.0 / fps;
		RealTime now = System::time();

		lastTime = now;
		RealTime timeStep = now - lastTime;
		
		doUserInput();

		onLogic();

		onGraphics();

		double rate = simTimeRate;
		RealTime rdt = timeStep;
		SimTime  sdt = timeStep * rate;
		SimTime  idt = desiredFrameDuration * rate;

		onSimulation(rdt, sdt, idt);

		now = System::time();

		lastWaitTime = System::time();
	}

}

void RBX::Experimental::Application::start()
{
	if (!isInitialized)
	{
		RBX::StandardOut::print(RBX::MESSAGE_INFO, "Application::start()");

		isInitialized = true;
		isThinking = true;

		onInit();
	}
}

void RBX::Experimental::Application::sendClose()
{
	SendMessage(parent, WM_CLOSE, 0, 0);
}

void RBX::Experimental::Application::close()
{
	RBX::Datamodel::getDatamodel()->close();
	renderDevice->cleanup();
}
