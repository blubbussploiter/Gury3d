#include "oscontext.h"
#include "stdout.h"

void OSContext::reallyMakeCurrent()
{
	if (!OSMesaMakeCurrent(Context, image._byte, GL_UNSIGNED_BYTE, image.width, image.height))
	{
		throw std::runtime_error("OSMesaMakeCurrent failed");
	}
}

OSContext::OSContext(int width, int height, int channels)
{
	image = GImage(width, height, channels);
	Context = OSMesaCreateContext(OSMESA_RGB+(channels-3), 0);

	if (!Context)
	{
		throw std::runtime_error("OSMesaCreateContext failed");
	}
}

DummyWindow::DummyWindow(int width, int height)
{
	HWND dummy;

	dummy = CreateWindowEx(
		0,
		"gcc",
		"GuryServer",
		WS_DISABLED,   // The style of window to produce (overlapped is standard window for desktop)
		CW_USEDEFAULT,   // x position of the window
		CW_USEDEFAULT,   // y position of the window 
		1, 1,
		NULL,       // Parent window    
		NULL,       // Menu
		NULL,  // Instance handle
		NULL        // Additional application data
	);

	if (!dummy)
	{
		return;
	}

	GWindowSettings settings;

	settings = GWindowSettings();

	settings.width = width;
	settings.height = height;

	window = Win32Window::create(settings, dummy);
}
