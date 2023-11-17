#ifndef JOINTSERVICE_H
#define JOINTSERVICE_H

#include "runservice.h"
#include "pvenums.h"

#include "instance.h"
#include "joints.h"

#include "service.h"

namespace RBX
{
	class JointService : 
		public RBX::Instance,
		public RBX::Service<JointService>
	{
	public:
		NormalId getIntersectingFace(btPersistentManifold* manifold, int index);

		void applyJoint(RBX::PVInstance* p0, RBX::PVInstance* p1, NormalId intersect);
		void manualBuild(RBX::PVInstance* p0, RBX::PVInstance* p1);
		bool areJointed(RBX::PVInstance* p0, RBX::PVInstance* p1);
		void buildJoints(RBX::Instance* holder=0, RBX::PVInstance* p0=0, RBX::PVInstance* p1=0);
		static JointService* singleton();
		JointService()
		{
			setName("JointsService");
			setClassName("JointsService");
		}
	};
}

#endif