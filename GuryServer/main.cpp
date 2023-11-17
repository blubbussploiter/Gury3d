//#include "rbx.h"

//#include "crashreporter.h"
//#include "strings.h"

//#include "CStudioMainWindow.h"

#include <thread>

#include "rbx.h"
#include "stdout.h"
#include "appmanager.h"
#include "crashreporter.h"

int main()
{

	//CStudioApp* studio;
	//studio = new CStudioApp();

	RBX::AppManager* manager = RBX::AppManager::singleton();
	RBX::CrashReporter::singleton()->start();

	WNDCLASS wndClass = { };
	HINSTANCE hInst = GetModuleHandle(0);

	wndClass.style = 0;         // Windows Style (future article)
	wndClass.lpfnWndProc = DefWindowProc;         // The Window Procedure
	wndClass.cbClsExtra = 0;         // Extra memory for this class
	wndClass.cbWndExtra = 0;         // Extra memory for this window
	wndClass.hInstance = hInst;         // The definition of the instance of the application
	wndClass.hIcon = 0;         // The icon in the upper left corner
	wndClass.hCursor = 0;//::LoadCursor(0, IDC_ARROW);   // The cursor for this window
	wndClass.hbrBackground = 0;//(HBRUSH)(COLOR_WINDOW + 1);   // The background color for this window
	wndClass.lpszMenuName = 0;         // The name of the class for the menu for this window
	wndClass.lpszClassName = "gcc";      // The name of this class

	RegisterClass(&wndClass);

	manager->setCurrentApplication(manager->instantiate(0));
	manager->getApplication()->start();

	manager->start();

	return 0;
}