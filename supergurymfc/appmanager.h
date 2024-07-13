#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <G3DAll.h>
#include "application.h"

namespace RBX
{

	class AppManager
	{
	private:

		std::vector<Experimental::Application*> applications;
		Experimental::Application* currentApplication;
		bool bOneTimeInitialized, bMainLoopStarted;

	public:

		std::string toLoad;
		std::string fileName;

		Experimental::Application* instantiate(HWND wnd);
		Experimental::Application* getApplication();

		void closeCurrentApplication();

		void setCurrentApplication(Experimental::Application* app);
		void setCurrentApplicationFromIndex(int index);
		void initOneTimeAppliances();

		void start();

		bool mainLoopStarted();

		static AppManager* get();
		static bool isReady()
		{
			return (get()->getApplication() != 0);
		}
	};
}
#endif