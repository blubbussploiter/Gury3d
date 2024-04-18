#pragma once

#include <G3DAll.h>

#include "instance.h"
#include "pvenums.h"

namespace RBX
{
	namespace Render
	{
		class Renderable : 
			public RBX::Instance
		{
		public:

			bool unaffectedByLight, renderedLast;
			float alpha, transparency, reflectance;

			Renderable* specialShape;

			void renderDecals(RenderDevice* rd, Instance* parent=0);

			virtual void render(RenderDevice* rd) {};
			virtual void renderSurfaces(RenderDevice* rd) {};

			virtual void render3DSurfaces(RenderDevice* rd) {};
			virtual void renderFace(RenderDevice* rd, NormalId face) {};

			/* renderface but with (preferably) different uv mapping */
			virtual void renderFaceFitForDecal(RenderDevice* rd, NormalId face) {};

			virtual ~Renderable() {}
			Renderable() 
			{
			}
			RTTR_ENABLE(RBX::Instance);
		};

	}
}
