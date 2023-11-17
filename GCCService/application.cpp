#include <thread>

#include "appmanager.h"
#include "camera.h"

RBX::Experimental::Application::Application(HWND wnd)
{
	GAppSettings _settings;

	fps = 30.0f;

	isThinking = false;
	parent = wnd;
}