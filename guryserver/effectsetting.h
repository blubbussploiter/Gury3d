#pragma once

#include "GLG3D/ToneMap.h"

using namespace G3D;

namespace RBX
{
	namespace Render
	{
		class EffectSettings
		{
		public:

			ToneMapRef toneMap;

			bool _hemisphereLighting;

			void applyEffects(RenderDevice* rd, LightingParameters& pout, LightingRef& lout);
			void polish(RenderDevice* rd, const LightingParameters& pin, LightingParameters& pout, const LightingRef& lin, LightingRef& lout);

			EffectSettings()
			{
				toneMap = ToneMap::create();
			}

		};
	}
}