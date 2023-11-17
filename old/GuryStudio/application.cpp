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
			RBX::Datamodel::getDatamodel()->close();
			exit(0);
			break;
		}
		case WM_SIZE:
		{
			app->resizeWithParent(hwnd);
			//app->onGraphics();
			break;
		}
		case WM_SETFOCUS:
		{
			app->justReceivedFocus = true;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			short d = HIWORD(wParam);
			app->getCamera()->cam_zoom(d > 0);
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		{
			app->msg.hwnd = hwnd;
			app->msg.message = uMsg;
			app->msg.wParam = wParam;
			app->msg.lParam = lParam;
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

void RBX::Experimental::Application::resizeWithParent(HWND _parent)
{
	RECT rect;

	if (!_parent) _parent = parent;

	GetClientRect(_parent, &rect);
	resizeViewport(rect.right, rect.bottom);
}

void RBX::Experimental::Application::resizeViewport(int viewportWidth, int viewportHeight)
{
	Rect2D viewportRect = Rect2D::xywh(0, 0, viewportWidth, viewportHeight); 
	renderDevice->notifyResize(viewportWidth, viewportHeight);
	renderDevice->setViewport(viewportRect);
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
	fps = 30.0f;

	_settings.window.resizable = true;
	_settings.window.framed = false;
	_settings.dataDir = ConFileInPath("\\content\\");

	isThinking = false;
	parent = wnd;

	window = Win32Window::create(_settings.window, parent);
	renderDevice = new RenderDevice();

	renderDevice->init(window, 0);
	userInput = new G3D::UserInput();

	sky = Sky::create(renderDevice, _settings.dataDir + "sky/");

}