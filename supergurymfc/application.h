#pragma once

#include <stack>

#include "workspace.h"
#include "datamodel.h"

#include "stdout.h"

namespace RBX
{
	namespace Experimental
	{

		class Application
		{
		private:

			bool isThinking, isInitialized;
			RealTime lastWaitTime, lastTime;

			HWND parent;
			Datamodel* datamodel;

			Camera* camera; /* main viewport */

			bool                 _mouseButtons[3];
			bool                 _keyboardButtons[0xFF];

			double simTimeRate = 1.0;

		public:

			SkyRef sky;

			bool justReceivedFocus, inFocus;
			float fps;

			WNDPROC wndProc;
			MSG key, mouse; /* jank but it works :D */

			RenderDevice* renderDevice;
			G3D::UserInput* userInput;

			GWindow* window;

			std::string rbxlFile;

			/* Window stuff */

			void setWindowLong();
			void resizeWithParent(int cx=0, int cy=0);
			void resize(int cx, int cy);

			/* onXXX */

			void onGraphics();
			void onLogic();
			void onInit();

			void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);
			void doUserInput();

			bool pollEvent(MSG msg, GEvent& e);

			/* Application stuff */

			void mainProcessStep();
			void start();

			void sendClose();
			void close();

			Datamodel* getDatamodel();
			Camera* getCamera();

			void onFocus();
			void onKillFocus();

			Rect2D getViewport() { return renderDevice->getViewport(); }

			void suspend()
			{
				isThinking = false;
			}

			void resume()
			{
				//getDatamodel()->emptyExplorerWindow();
				//getDatamodel()->fillExplorerWindow();
				isThinking = true;
			}

			bool initialized() { return isInitialized; }

			Application(HWND wnd);
		};
	}

	extern SkyRef getGlobalSky();
}