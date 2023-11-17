
#ifndef PART_H
#define PART_H

#include "pvinstance.h"
#include "brickcolor.h"

#include "enum.h"

namespace RBX
{

	class PartInstance :
		public RBX::PVInstance
	{
	public:

		BrickColor* brickColor;

		void setBrickColor(int bc) 
		{
			Color3* b;

			brickColor = BrickColor::fromNumber(bc);
			b = brickColor->getColor();

			color = Color3(b->r, b->g, b->b);
		}

		int getBrickColor()
		{
			if (brickColor)
			{
				return brickColor->getNumber();
			}
			return 0;
		}

		void renderDecals(RenderDevice* rd, Render::Renderable* base);
		virtual void render(RenderDevice* rd);
		virtual ~PartInstance() {}

		PartInstance()
		{
			setClassName("Part");
			setName("Part");

			shape = Shape::part;

		}

		RTTR_ENABLE(RBX::PVInstance)
	};
}

#endif