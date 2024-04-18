#pragma once

#include "body.h"

namespace RBX
{
	/* Rigidbody based physics engine ALTHOUGH, relies on Primitives for seperation of Primitives from Bodies */

	class Kernel 
	{
	public:

		Array<Primitive*> objects;
		
		dWorldID world;
		dSpaceID space;
		dJointGroupID contacts;

		void diag_renderObjects(RenderDevice* rd);

		void addPrimitive(Primitive* primitive);

		void removePrimitive(Primitive* primitive);

		void step();

		void afterStep();

		static void collisionCallback(void* data, dGeomID o1, dGeomID o2);

		static Kernel* get();

		Kernel()
		{

			world = dWorldCreate();
			space = dHashSpaceCreate(0);
			contacts = dJointGroupCreate(0);

			dWorldSetGravity(world, 0, -9.81F, 0);
			dWorldSetAutoDisableFlag(world, 1);
			dWorldSetAutoDisableLinearThreshold(world, 0.5f);
			dWorldSetAutoDisableAngularThreshold(world, 0.5f);
			dWorldSetAutoDisableSteps(world, 20);
			dWorldSetCFM(world, 1^-5);
			dWorldSetERP(world, 0.8f);
		}
	};
}