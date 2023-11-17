#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include <G3DAll.h>

#include <ode/ode.h>
#include <ode/odecpp.h>

#include "instance.h"
#include "service.h"

#define BODY_MASS 0.7f
#define contains(v, i) (std::find(v.begin(), v.end(), i) != v.end())

#define g3d2ode(g3dRot) { g3dRot[0][0], g3dRot[0][1], g3dRot[0][2], 0, g3dRot[1][0], g3dRot[1][1], g3dRot[1][2], 0, g3dRot[2][0], g3dRot[2][1], g3dRot[2][2], 0 }

namespace RBX
{
	class PhysBody; class PVInstance;
}

namespace RBX
{

	class XplicitNgine
	{
	public:

		dWorldID physWorld;
		dSpaceID physSpace;
		dJointGroupID contactgroup;

		void update();

		void init();
		void close();

		void checkBodies(RBX::Instances* PVInstances);
		void createBody(RBX::PVInstance* part);

		void removeBody(RBX::PVInstance* part);
		void resetBody(RBX::PVInstance* part);

		void updateBody(RBX::PVInstance* part);
		void updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p);
		void updateAnchor(RBX::PVInstance* part);

		bool isTouching(RBX::PVInstance* part, bool ignoreSiblings=0);
		bool areColliding(RBX::PVInstance* part1, RBX::PVInstance* part2);

		int getNumberOfGeoms() 
		{
			return 0;
		}

		int getNumberOfConstraints() {

			return 0;
		}

		XplicitNgine()
		{
			physWorld = dWorldCreate();
			physSpace = dSimpleSpaceCreate(0);
			contactgroup = dJointGroupCreate(0);

			dWorldSetGravity(physWorld, 0, -9.81F, 0);
			dWorldSetAutoDisableFlag(physWorld, 1);
			dWorldSetAutoDisableLinearThreshold(physWorld, 0.5F);
			dWorldSetAutoDisableAngularThreshold(physWorld, 0.5F);
		}
	};

	class RunService :
		public RBX::Instance,
		public RBX::Service<RunService>
	{
	private:
		XplicitNgine* physics;
		RBX::Instances* steppers;
	public:
		bool isRunning;
		void run();
		void stop();
		void reset();
		void update();
		void heartbeat();
		void updateSteppers();
		void workspaceOnDescendentAdded(RBX::Instance* descendent);
		XplicitNgine* getPhysics() { return physics; }
		/* deprecated, use Datamodel->runService */
		static RunService* singleton();
		RunService()
		{
			setClassName("RunService");
			setName("RunService");
			isRunning = 0;
			steppers = new RBX::Instances();
			physics = new XplicitNgine();
			physics->init();
		}
		RTTR_ENABLE(RBX::Instance);
	};

}

#endif