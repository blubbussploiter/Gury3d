#include "appmanager.h"

#include "selection.h"
#include "players.h"

#include "camera.h"
#include "stdout.h"

#include "Mouse.h"
#include "view.h"

#include "networkserver.h"

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
	getDatamodel()->step();
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

	getDatamodel()->loadContent("D:\\erik.rbxl");
	RBX::Network::NetworkServer::networkServer()->start(440, 30);

	//getDatamodel()->runService->run();

}

void RBX::Experimental::Application::mainProcessStep()
{
	RealTime desiredFrameDuration = 1.0 / fps;
	RealTime now = System::time();

	lastTime = now;
	RealTime timeStep = now - lastTime;

	onLogic();

	//RBX::View::singleton()->oneFrame(renderDevice, getCamera(), sky); /* future rendering (within client) */

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