#pragma once

#include <G3DAll.h>
#include "instance.h"

namespace RBX
{
	namespace Render
	{
		class Renderable : 
			public RBX::Instance
		{
		public:

			bool isSpecialShape;
			bool renderedLast;

			float transparency;

			Renderable* specialShape;

			virtual void render(RenderDevice* rd) {};
			virtual void renderSurfaces(RenderDevice* rd) {};
			virtual void render3DSurfaces(RenderDevice* rd) {};

			virtual ~Renderable() {}
			Renderable() { isRenderable = 1; isSpecialShape = false; }
			RTTR_ENABLE(RBX::Instance);
		};

	}
}
