
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
			color = BrickColor::BrickMap::get()->fromNumber(bc);
		}

		int getBrickColor()
		{
			return brickColor.number.number;
		}

		void renderDecals(RenderDevice* rd);
		virtual void render(RenderDevice* rd);
		virtual ~PartInstance() {}

		PartInstance()
		{
			setClassName("Part");
			setName("Part");

			shape = Shape::Block;

			setSize(Vector3(2, 1.2f, 4));

		}

		RTTR_ENABLE(RBX::PVInstance)
	};
}

#endif