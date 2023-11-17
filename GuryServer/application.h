#pragma once

#include "datamodel.h"
#include "workspace.h"

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
			//SkyRef sky; /* not used by server */

			bool                 _mouseButtons[3];
			bool                 _keyboardButtons[0xFF];

			double simTimeRate = 1.0;

		public:

			bool                justReceivedFocus;

			WNDPROC wndProc;
			MSG msg;

			float fps;

			/* onXXX */

			void onLogic();
			void onInit();

			void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);

			/* Application stuff */

			void mainProcessStep();
			void start();

			Datamodel* getDatamodel();
			Camera* getCamera();

			Rect2D getViewport() { return Rect2D::xywh(Vector2::zero(), Vector2(1,1)); }

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
}