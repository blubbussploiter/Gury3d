#pragma once

#include <stack>

#include "workspace.h"
#include "datamodel.h"

#include "stdout.h"

namespace RBX
{
	namespace Experimental
	{

		extern HCURSOR guryCursor;

		class Application
		{
		private:

			bool isThinking, isInitialized;
			RealTime lastWaitTime, lastTime;

			Datamodel* datamodel;

			Camera* camera; /* main viewport */

			bool                 _mouseButtons[3];
			bool                 _keyboardButtons[0xFF];

			double simTimeRate = 1.0;

		public:

			HWND parent;

			SkyRef sky;

			bool justReceivedFocus, inFocus;
			bool inEditMode;

			int width, height;
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

			void onResize();
			void onGraphics();
			void onLogic();
			void onInit();

			void exitEditMode(bool inEditMode=false);

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
				isThinking = true;
			}

			bool initialized() { return isInitialized; }

			Application(HWND wnd);
		};
	}

	extern SkyRef getGlobalSky();
}