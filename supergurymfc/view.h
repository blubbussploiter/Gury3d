#ifndef VIEW_H
#define VIEW_H

#include "scene.h"
#include "camera.h"

#include "effectsetting.h"

namespace RBX
{
	class View : public Instance
	{
	private:

		LightingRef lighting;
		LightingParameters params;

		Color3 colorClearValue;
		Render::EffectSettings* effectSettings;

		bool graphicsInitialized;

	public:

		void onWorkspaceDescendentAdded(RBX::Instance* descendent);
		void onWorkspaceDescendentRemoved(RBX::Instance* descendent);

		void presetLighting();
		void turnOnLights(RenderDevice* rd);

		void renderScene(RenderDevice* rd);
		void oneFrame(RenderDevice* rd, Camera* projection, SkyRef sky);

		Render::EffectSettings* getEffectSettings()
		{
			if (!effectSettings)
			{
				effectSettings = new Render::EffectSettings();
				effectSettings->toneMap->setEnabled(0);
			}
			return effectSettings;
		}

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

		View()
		{
			lighting = G3D::Lighting::create();
			params.source = LightingParameters::MOON;
		}

		static View* singleton();
	};
}

#endif