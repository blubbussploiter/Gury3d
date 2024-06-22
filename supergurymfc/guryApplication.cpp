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

#include "StudioTool.h"

HCURSOR original;

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

		mouse.message = 0;
		mouse.wParam = 0;
		mouse.lParam = 0;

		key.message = 0;
		key.wParam = 0;
		key.lParam = 0;

		userInput->processEvent(e);
	}

	window->pollEvent(e);

	userInput->endEvents();
}

void RBX::Experimental::Application::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	SoundService::singleton()->update();
	getDatamodel()->step(idt);
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
	if (RBX::Studio::current_Tool)
	{
		RBX::Studio::current_Tool->doLogic(userInput);
	}

	Mouse::getMouse()->update(userInput);
	getCamera()->update(userInput->keyDown(SDL_RIGHT_MOUSE_KEY));

	RBX::Gui::singleton()->doButtonLogic(userInput, renderDevice);
	RBX::Network::getPlayers()->onStep();

	RBX::ControllerService::singleton()->updateControllers(userInput);
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
	View::singleton()->oneFrame(renderDevice, getCamera(), sky);

	renderDevice->push2D();

	Diagnostics::get_Renderer()->render2D(renderDevice);
	Mouse::getMouse()->render(renderDevice);

	renderDevice->pop2D();
}

void RBX::Experimental::Application::onInit()
{
	Diagnostics::get_Renderer()->diagnostics_enabled = false;

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "Application::onInit()");
	RBX::AppManager::singleton()->initOneTimeAppliances();

	setWindowLong();

	original = GetCursor();

	getCamera();
	getDatamodel();

	RBX::ControllerService::singleton()->addController(RBX::Camera::singleton());
	
	RBX::ScriptContext* context = getDatamodel()->scriptContext;
	context->openState();

	if (!rbxlFile.empty())
	{
		getDatamodel()->name = RBX::AppManager::singleton()->fileName;
		getDatamodel()->loadContent(rbxlFile);
	}

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
	RBX::Datamodel::getDatamodel()->close();
	renderDevice->cleanup();
}
