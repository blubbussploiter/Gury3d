#pragma once

#include "GL/osmesa.h"
#include <G3DAll.h>

#pragma comment(lib, "OSMESA32.lib")

class DummyWindow
{
private:
	Win32Window* window;
public:
	Win32Window* getWindow()
	{
		return window;
	};
	DummyWindow(int width, int height);
	~DummyWindow() { delete window; }
};

class OSContext
{
private:
	OSMesaContext Context;
public:

	GImage image;

	void reallyMakeCurrent();

	OSContext(int width, int height, int channels);
};