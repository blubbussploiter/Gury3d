#pragma once
#ifndef RENDER_H
#define RENDER_H

#include <G3DAll.h>
#include "instance.h"

namespace RBX
{
	namespace Render
	{
		class Renderable : 
			public RBX::Derivable<RBX::Instance>
		{
		public:

			bool isSpecialShape;
			float transparency;

			Renderable* specialShape;

			virtual void render(RenderDevice* rd) {};
			virtual void renderSurfaces(RenderDevice* rd) {};
			virtual void render3DSurfaces(RenderDevice* rd) {};

			virtual ~Renderable() {}
			Renderable() { isRenderable = 1; isSpecialShape = false; }

		};

	}
}

#endif
