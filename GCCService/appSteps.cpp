#include "appmanager.h"

#include "selection.h"
#include "players.h"

#include "camera.h"
#include "stdout.h"

#include "Mouse.h"
#include "view.h"

#include "networkserver.h"
#include "scriptcontext.h"

#include "soundservice.h"
#include "thumbnailGenerator.h"

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
	getCamera()->update(0);
}

void RBX::Experimental::Application::onInit()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "Application::onInit()");
	RBX::AppManager::singleton()->initOneTimeAppliances();

	getCamera();
	getDatamodel();

	RBX::ControllerService::singleton()->addController(RBX::Camera::singleton());
	
	RBX::ScriptContext* context = getDatamodel()->scriptContext;
	context->openState();

	(new RBX::Network::NetworkServer())->start(800);

	//getDatamodel()->setMessage("gury");

	//getDatamodel()->setMessageBrickCount();
	//getDatamodel()->players->createLocalPlayer(1);

	//getDatamodel()->loadContent(GetFileInPath("\\gury.rbxl"));
	getDatamodel()->loadContent("D:\\Alexstrandlegos-house.rbxl");
	//getCamera()->zoomExtents();

}

void RBX::Experimental::Application::mainProcessStep()
{
	if (isThinking)
	{
		RealTime desiredFrameDuration = 1.0 / fps;
		RealTime now = System::time();

		lastTime = now;
		RealTime timeStep = now - lastTime;

		onLogic();

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
