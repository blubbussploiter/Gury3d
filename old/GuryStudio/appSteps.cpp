#include "appmanager.h"

#include "selection.h"
#include "players.h"

#include "camera.h"
#include "stdout.h"

#include "Mouse.h"
#include "view.h"

#include "networkclient.h"
#include "scriptcontext.h"

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

	getCamera()->update(userInput);

	RBX::ControllerService::singleton()->updateControllers(userInput);
	RBX::Gui::singleton()->doButtonLogic(userInput, renderDevice);

}

void RBX::Experimental::Application::onFocus()
{
	window->makeCurrent();
	getDatamodel()->workspace->setCurrentCamera(getCamera());
	resizeWithParent();
}

void RBX::Experimental::Application::onInit()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "Application::onInit()");
	
	RBX::AppManager::singleton()->initOneTimeAppliances();
	setWindowLong();

	getCamera();
	getDatamodel();

	RBX::ControllerService::singleton()->addController(RBX::Camera::singleton());

	getDatamodel()->loadContent("D:\\pirate.rbxl");
	
	RBX::ScriptContext* context = getDatamodel()->scriptContext;
	context->openState();

	try
	{
		context->runScript("wait(1) print('hi') game.RunService:run() wait(2) print('pause') game.RunService:pause()", "test");
	}
	catch(std::exception e)
	{
		printf("%s\n", e.what());
	}

	//getDatamodel()->setMessage("gury");

	//getDatamodel()->setMessageBrickCount();
	//getDatamodel()->players->createLocalPlayer(1);

	//getDatamodel()->loadContent(GetFileInPath("\\gury.rbxl"));
	//getDatamodel()->loadContent("D:\\hq.rbxl");
	//getCamera()->zoomExtents();
	
	//getDatamodel()->runService->run();

	//getDatamodel()->players->createLocalPlayer(0);
	//getDatamodel()->players->localPlayer->loadCharacter();

}

void RBX::Experimental::Application::onGraphics()
{
	RBX::View::singleton()->oneFrame(renderDevice, getCamera(), sky);
}

void RBX::Experimental::Application::mainProcessStep()
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