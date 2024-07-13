#ifndef VIEW_H
#define VIEW_H

#include "scene.h"
#include "camera.h"

#include "effectsetting.h"
#include "shadows.h"

namespace RBX
{
	class View : public Instance
	{
	private:

		Color3 colorClearValue;
		
		bool graphicsInitialized;

	public:

		Render::EffectSettings* effectSettings;
		Shadows* shadows;

		LightingRef lighting;
		LightingParameters params;

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
				effectSettings->_hemisphereLighting = 0;
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
			shadows = new Shadows();
		}

		static View* singleton();
	};
}

#endif