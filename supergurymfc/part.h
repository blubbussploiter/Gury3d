
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

		BrickColor brickColor;

		void setBrickColor(int bc)
		{
			color = BrickColor::BrickMap::singleton()->fromNumber(bc);
		}

		int getBrickColor()
		{
			return brickColor.number.number;
		}

		void renderDecals(RenderDevice* rd, Render::Renderable* base);
		virtual void render(RenderDevice* rd);
		virtual ~PartInstance() {}

		PartInstance()
		{
			setClassName("Part");
			setName("Part");

			shape = Shape::part;

			setTopSurface(Studs);
			setBottomSurface(Inlet);

		}

		RTTR_ENABLE(RBX::PVInstance)
	};
}

#endif