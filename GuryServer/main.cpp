//#include "rbx.h"

//#include "crashreporter.h"
//#include "strings.h"

//#include "CStudioMainWindow.h"

#include <thread>

#include "supergurymfc.h"
#include "supergurymfcview.h"

int main()
{

	//CStudioApp* studio;
	//studio = new CStudioApp();

	//RBX::CrashReporter::singleton()->start();
	/*

	GAppSettings settings;

	settings.window.width = 820;
	settings.window.height = 480;
	settings.window.resizable = true;
	settings.dataDir = ConFileInPath("\\content\\");

	Rendering::G3DApp app(settings);

	app.debugController.setActive(false);

	app.run();
	*/

	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 1))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		return 1;
	}

	theApp.InitApplication();
	theApp.InitInstance();

	theApp.Run();

	return 0;
}