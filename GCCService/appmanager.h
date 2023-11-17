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

		std::string toLoad;
		std::string fileName;

		Experimental::Application* instantiate(HWND wnd);
		Experimental::Application* getApplication();

		void setCurrentApplication(Experimental::Application* app);
		void initOneTimeAppliances();

		void start();

		bool mainLoopStarted();

		static AppManager* singleton();
		static bool isReady()
		{
			return (singleton()->getApplication() != 0);
		}
	};
}
#endif