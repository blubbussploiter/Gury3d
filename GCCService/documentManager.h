#ifndef DOC_MANAGER
#define DOC_MANAGER

#include "rbx.h"
#include "strings.h"

#include "ChildFrm.h"

namespace RBX
{
	class DocumentManager
	{
	private:
		GAppSettings settings;

		Rendering::G3DApp* app;
		Rendering::G3DApplet* applet;

		CMDIFrameWnd* currentMdi;

		HWND mainWnd, renderWnd;

	public:

		void newDocument(CMDIFrameWnd* mdi, HWND mainWnd);
		void resizeCurrentDocument(int width, int height);

		static DocumentManager* singleton();
		static LRESULT CALLBACK DocumentLong(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		DocumentManager()
		{
			app = 0;
			applet = 0;
			settings.window.resizable = 0;
			settings.window.framed = 0;
			settings.window.refreshRate = 30;
			settings.dataDir = ConFileInPath("\\content\\");
		}
	};
}

#endif