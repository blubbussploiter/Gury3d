#pragma once

#include "service.h"
#include "kernel.h"

#include "primitive.h"
#include "pvinstance.h"

namespace RBX
{

	enum Linkage
	{
		NotLinked,
		Snapped,
		Welded,
		Glued,
		Motored,
		Hinged
	};

	class Connector : public Instance
	{
	public:

		Primitive* prim0, * prim1;

		virtual void build() {};
		virtual void unlink() {};

		bool connected();

		Connector(Primitive* prim0, Primitive* prim1) : prim0(prim0), prim1(prim1)
		{
		}
	};

	class JointsService :
		public Instance,
		public Service<JointsService>
	{
	private:
		Array<Connector*> connectors;
	public:

		dJointGroupID joints;

		void addConnector(Connector* connector);

		void buildConnectors();

		void buildGlobalJoints();

		static bool areConnectedPrimitives(Primitive* prim0, Primitive* prim1);

		static bool areConnectedBodies(Body* body0, Body* body1);

		static bool bodyIsConnector(Body* body);

		static Linkage makeLinkage(SurfaceType s0, SurfaceType s1);

		static NormalId fromNormal(Vector3 normal);

		static Connector* fromLinkageAndPrimitives(Linkage linkage, Primitive* prim0, Primitive* prim1);

		static void collisionCallback(void* data, dGeomID o1, dGeomID o2);

		static JointsService* singleton();

		JointsService()
		{
			setName("JointsService");
			joints = dJointGroupCreate(0);
		}
	};
}