#ifndef VIEW_H
#define VIEW_H

#include "scene.h"
#include "camera.h"

namespace RBX
{
	class View : public Instance
	{
	private:
		bool graphicsInitialized;
	public:

		void onWorkspaceDescendentAdded(RBX::Instance* descendent);
		void onWorkspaceDescendentRemoved(RBX::Instance* descendent);

		void render(RenderDevice* rd);
		void oneFrame(RenderDevice* rd, Camera* projection, SkyRef sky);

		void renderSky(SkyRef sky, LightingParameters parameters)
		{
			sky->render(parameters);
		}

		SkyRef createSky(RenderDevice* rd)
		{
			return Sky::create(rd, ConFileInPath("\\content\\sky\\"));
		}

		void initGraphics()
		{
			if (!graphicsInitialized)
			{
				graphicsInitialized = true;
				RBX::initSurfaces();
			}
		}

		static View* singleton();
	};
}

#endif