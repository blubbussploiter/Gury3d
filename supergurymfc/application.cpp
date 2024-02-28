#include <thread>

#include "appmanager.h"
#include "camera.h"

BOOL CALLBACK documentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RBX::Experimental::Application* app = RBX::AppManager::singleton()->getApplication();

	if (app)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
		case WM_QUIT:
		{
			RBX::AppManager::singleton()->closeCurrentApplication();
			break;
		}
		case WM_MOUSEWHEEL:
		{
			short d = HIWORD(wParam);
			app->getCamera()->cam_zoom(d > 0);
			break;
		}
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		{
			app->mouse.hwnd = hwnd;
			app->mouse.message = uMsg;
			app->mouse.wParam = wParam;
			app->mouse.lParam = lParam;
			break;
		}
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			app->key.hwnd = hwnd;
			app->key.message = uMsg;
			app->key.wParam = wParam;
			app->key.lParam = lParam;
			break;
		}
		default:
		{
			break;
		}
		}

		return CallWindowProcA((WNDPROC)app->wndProc, hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

void RBX::Experimental::Application::resizeWithParent(int cx, int cy)
{
	RECT r;
	int w, h;

	GetClientRect(parent, &r);

	w = r.right;
	h = r.bottom;

	if (cx) w = cx;
	if (cy) h = cy;

	resize(w, h);
}

void RBX::Experimental::Application::resize(int cx, int cy)
{
	RECT r;
	int w, h;

	GetClientRect(parent, &r);

	w = r.right;
	h = r.bottom;

	Rect2D viewportRect = Rect2D::xywh(0, 0, w, h);
	renderDevice->notifyResize(cx, cy);
	renderDevice->setViewport(viewportRect);

	//onGraphics();
}

void RBX::Experimental::Application::setWindowLong()
{
	if (!wndProc)
	{
		wndProc = (WNDPROC)GetWindowLongA(parent, GWL_WNDPROC);
		SetWindowLongA(parent, GWL_WNDPROC, (LONG)documentWndProc);
	}
}

RBX::Experimental::Application::Application(HWND wnd)
{
	GAppSettings _settings;

	_settings.window.resizable = true;
	_settings.window.framed = false;
	_settings.window.stereo = false;
	_settings.window.refreshRate = 32;
	_settings.dataDir = ConFileInPath("\\content\\");

	parent = wnd;
	window = Win32Window::create(_settings.window, parent);

	renderDevice = new RenderDevice();
	renderDevice->init(window, 0);

	userInput = new G3D::UserInput();
	window->makeCurrent();

	sky = Sky::create(renderDevice, _settings.dataDir + "sky/");
	resizeWithParent();

	isThinking = false;
	fps = 30.0f;
}

SkyRef RBX::getGlobalSky()
{
	return AppManager::singleton()->getApplication()->sky;
}
