#pragma once

#include <G3DAll.h>
#include "ode/ode.h"

#include "body.h"
#include "pvenums.h"

namespace RBX
{
	class Primitive
	{
	public:

		RBX::Shape shape;

		dGeomID geom[1];

		Body* body;
		PV* pv;

		Vector3 size;

		float elasticity, friction;

		void* ud;

		bool collisionsEnabled();

		void modifySize(Vector3 size);

		void modifyShape(Shape shape);

		void modifyPosition(CoordinateFrame position);

		void modifyCollisions(bool canCollide);

		void modifyUserdata(void* data);

		void modifyOffsetWorldCoordinateFrame(CoordinateFrame offset);

		void* getUserdata();

		void setDisabled(bool disabled);

		void reset();

		void createPrimitive(RBX::Shape primitive, Vector3 size);

		void step();

		/* attached to a body: simulated */
		Primitive(Body* body);
		Primitive();
	};
}