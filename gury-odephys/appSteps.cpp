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

	while (pollEvent(e))
	{
		userInput->processEvent(e);

		msg.message = 0;
		msg.wParam = 0;
		msg.lParam = 0;

	}

	window->pollEvent(e);

	userInput->endEvents();
}

void RBX::Experimental::Application::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	if (getDatamodel()->runService->isRunning)
	{
		getDatamodel()->step();
	}
	getDatamodel()->heartbeat(idt);
	SoundService::singleton()->update();
}

void RBX::Experimental::Application::onLogic()
{

	if (userInput->keyPressed(SDLK_o))
	{
		RBX::Camera::singleton()->cam_zoom(0);
	}
	if (userInput->keyPressed(SDLK_i))
	{
		RBX::Camera::singleton()->cam_zoom(1);
	}

	if (userInput->anyKeyPressed())
	{
		if (GetForegroundWindow() != parent)
		{
			SendMessage(parent, WM_SETFOCUS, 0, 0);
			SetActiveWindow(parent);
			SetForegroundWindow(parent);
		}
	}

	getCamera()->update(userInput);

	RBX::ControllerService::singleton()->updateControllers(userInput);
	RBX::Gui::singleton()->doButtonLogic(userInput, renderDevice);

	Selection::update(userInput);
	RBX::Mouse::update(userInput);

}

void RBX::Experimental::Application::onFocus()
{

	window->makeCurrent();
	getDatamodel()->workspace->setCurrentCamera(getCamera());

	inFocus = 1;
	justReceivedFocus = 1;

	ShowCursor(FALSE);
}

void RBX::Experimental::Application::onKillFocus()
{
	inFocus = 0;
	justReceivedFocus = 0;

	ShowCursor(TRUE);
}

void RBX::Experimental::Application::onInit()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "Application::onInit()");
	
	RBX::AppManager::singleton()->initOneTimeAppliances();
	setWindowLong();

	getCamera();
	getDatamodel();

	RBX::ControllerService::singleton()->addController(RBX::Camera::singleton());
	
	RBX::ScriptContext* context = getDatamodel()->scriptContext;
	context->openState();

	if(!rbxlFile.empty())
		getDatamodel()->loadContent(rbxlFile);

	//getDatamodel()->setMessage("gury");

	//getDatamodel()->setMessageBrickCount();
	//getDatamodel()->players->createLocalPlayer(1);

	//getDatamodel()->loadContent(GetFileInPath("\\gury.rbxl"));
	//getDatamodel()->loadContent("D:\\hq.rbxl");
	//getCamera()->zoomExtents();
	
	getDatamodel()->runService->run();

	//getDatamodel()->players->createLocalPlayer(0);
	//getDatamodel()->players->localPlayer->loadCharacter();

}

void RBX::Experimental::Application::onGraphics()
{
	RBX::View::singleton()->oneFrame(renderDevice, getCamera(), sky);
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

		// Compute accumulated time

		System::sleep(max(0.0, desiredFrameDuration - (now - lastWaitTime)));
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
	renderDevice->cleanup();
}
