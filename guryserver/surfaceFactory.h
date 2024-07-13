#pragma once

#include <G3DAll.h>
#include "pvenums.h"

namespace RBX
{
	class RenderSurfaceFactory
	{
	private:
		static TextureRef base_texture_bumps;
		static TextureRef base_texture_studs;
		static TextureRef base_texture_inlets;
		static TextureRef base_texture_weld;
	public:
		static unsigned int getSurfaceTexture(SurfaceType s);
		static void initSurfaces();
	};
}