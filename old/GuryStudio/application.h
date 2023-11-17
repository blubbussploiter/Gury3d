#pragma once

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
			SkyRef sky;

			bool                 _mouseButtons[3];
			bool                 _keyboardButtons[0xFF];

			double simTimeRate = 1.0;

		public:

			bool                justReceivedFocus;

			WNDPROC wndProc;
			MSG msg;

			RenderDevice* renderDevice;
			G3D::UserInput* userInput;

			GWindow* window;

			float fps;

			/* Window stuff */

			void setWindowLong();

			void resizeWithParent(HWND _parent = 0);
			void resizeViewport(int viewportWidth, int viewportHeight);

			/* onXXX */

			void onGraphics();
			void onLogic();
			void onInit();

			void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);
			void doUserInput();

			bool pollEvent(GEvent& e);

			/* Application stuff */

			void mainProcessStep();
			void start();

			Datamodel* getDatamodel();
			Camera* getCamera();

			void onFocus();

			Rect2D getViewport() { return renderDevice->getViewport(); }

			void suspend()
			{
				RBX::StandardOut::print(RBX::MESSAGE_INFO, "Suspending 0x%08X", this);
				isThinking = false;
			}

			void resume()
			{
				RBX::StandardOut::print(RBX::MESSAGE_INFO, "Resuming 0x%08X", this);
				isThinking = true;
				onFocus();
			}

			bool initialized() { return isInitialized; }

			Application(HWND wnd);
		};
	}
}