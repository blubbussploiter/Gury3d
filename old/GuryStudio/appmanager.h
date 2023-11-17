#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <G3DAll.h>
#include "application.h"

namespace RBX
{

	class AppManager
	{
	private:

		Experimental::Application* currentApplication;
		bool bOneTimeInitialized, bMainLoopStarted;

	public:

		Experimental::Application* instantiate(HWND wnd);
		Experimental::Application* getApplication();

		void setCurrentApplication(Experimental::Application* app);
		void initOneTimeAppliances();

		void start();

		bool mainLoopStarted();

		static AppManager* singleton();
	};
}
#endif