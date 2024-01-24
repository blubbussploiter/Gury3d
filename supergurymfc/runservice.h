#pragma once

#include <G3DAll.h>

#include "instance.h"
#include "service.h"

#include "scriptcontext.h"

#define BODY_MASS 1.333f
#define contains(v, i) (std::find(v.begin(), v.end(), i) != v.end())

namespace RBX
{
	class PhysBody; class PVInstance;
}

namespace RBX
{

	class RunService :
		public RBX::Instance,
		public RBX::Service<RunService>
	{
	private:
		RBX::Instances* steppers;
	public:

		ScriptContext* scriptContext;

		bool isRunning;
		void run();
		void stop();
		void reset();
		void update();
		void heartbeat();
		void updateSteppers();

		void onWorkspaceDescendentAdded(RBX::Instance* descendent);

		/* deprecated, use Datamodel->runService */
		static RunService* singleton();
		RunService()
		{
			setClassName("RunService");
			setName("Run Service");
			isRunning = 0;
			steppers = new RBX::Instances();
		}
		RTTR_ENABLE(RBX::Instance);
	};

}
