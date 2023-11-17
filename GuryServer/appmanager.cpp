#include "appmanager.h"
#include "players.h"

#include "stdout.h"

RBX::AppManager* appManager;

void RBX::AppManager::initOneTimeAppliances()
{

	if (bOneTimeInitialized) return;
	bOneTimeInitialized = true;

	//RBX::initSurfaces();
	RBX::SoundService::singleton()->init();

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "AppManager::initOneTimeAppliances()");

	//RBX::Network::getPlayers()->setPlayerList(playerList); /* this may not be one time in the future */
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