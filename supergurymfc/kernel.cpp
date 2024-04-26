#include "kernel.h"
#include "primitive.h"

#include "pvinstance.h"

#pragma comment(lib, "ode.lib")

RBX::Kernel* kernel;

void RBX::Kernel::addPrimitive(Primitive* primitive)
{
	objects.append(primitive);
}

void RBX::Kernel::removePrimitive(Primitive* primitive)
{
	objects.remove(objects.findIndex(primitive));
}

void RBX::Kernel::addQueuedPrimitive(Primitive* primitive)
{
	objectQueue.append(primitive);
}

void RBX::Kernel::step()
{
	/* ode stuff, step the simulation */

	dJointGroupEmpty(contacts);
	dSpaceCollide(space, 0, &Kernel::collisionCallback);
	dWorldQuickStep(world, 0.02f);

	Kernel::get()->afterStep();

}

void RBX::Kernel::afterStep()
{
	/* gury stuff, create them or step them */

	/* (re) */

	spawnWorld();

	/* step */

	for (int i = 0; i < objects.size(); i++)
	{
		Primitive* prim = objects[i];
		prim->step();
	}
}

void RBX::Kernel::spawnWorld()
{
	if (objectQueue.size() > 0)
	{
		for (int i = 0; i < objectQueue.size(); i++)
		{
			Primitive* prim = objectQueue[i];
			PVInstance* pv;

			void* ud = prim->getUserdata();
			pv = (PVInstance*)ud;

			if (pv)
			{
				pv->initializeForKernel();
			}
		}

		objectQueue.clear();
	}
}

void RBX::Kernel::collisionCallback(void* data, dGeomID o1, dGeomID o2)
{
	int i;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	if (b1 && b2 && dAreConnected(b1, b2))
		return;

	dContact contact[4];
	int numc = dCollide(o1, o2, 4, &contact[0].geom, sizeof(dContact));

	if (numc > 0)
	{
		for (i = 0; i < numc; i++)
		{

			contact[i].surface.mode = dContactBounce | dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;

			// Define contact surface properties

			contact[i].surface.bounce = 0.5f; //Elasticity
			contact[i].surface.mu = 0.3f; //Friction
			contact[i].surface.slip1 = 0.0f;
			contact[i].surface.slip2 = 0.0f;
			contact[i].surface.soft_erp = 0.8f;
			contact[i].surface.soft_cfm = 0.001f;

			dJointID c = dJointCreateContact(
				Kernel::get()->world,
				Kernel::get()->contacts,
				contact + i);

			dJointAttach(c, b1, b2);
		}

	}
}

void RBX::Kernel::diag_renderObjects(RenderDevice* rd)
{
	float radius = 0.5f;
	Color3 color = Color3::blue();

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		Primitive* primitive = objects[i];
		if (primitive)
		{
			if (primitive->body)
			{
				const dReal* center = dBodyGetPosition(primitive->body->body);
				Draw::sphere(Sphere(Vector3(center[0], center[1], center[2]), radius), rd, color, Color4::clear());
			}
		}
	}
}

RBX::Kernel* RBX::Kernel::get()
{
	if (!kernel) kernel = new Kernel();
	return kernel;
}
