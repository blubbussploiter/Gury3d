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
		friend class SnapConnector;
	private:

		CoordinateFrame center; // for debug

	public:

		bool isAnchored;

		Primitive* prim0, * prim1;
		NormalId connectedAt;

		virtual void build() {};
		virtual void unlink() {};

		bool diagPrimitivesAreTouching();

		void diagRender(RenderDevice* rd);
		void diagRenderPrimitiveOutlines(RenderDevice* rd);

		CoordinateFrame getInterceptPosition(); // for debug 

		bool connected();

		Connector(Primitive* prim0, Primitive* prim1, NormalId connectedAt) : prim0(prim0), prim1(prim1), connectedAt(connectedAt)
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

		class Experiment
		{
		public:
			static void getKernelWorldContacts();
			static void buildGlobalJoints();
		};

		void addConnector(Connector* connector);

		void buildConnectors();

		void buildGlobalJoints();

		/* before build() version, looks through children */

		bool areConnectedUnbuiltPrimitives(Primitive* prim0, Primitive* prim1);

		/* static, after build() version */

		static bool areConnectedPrimitives(Primitive* prim0, Primitive* prim1);

		static bool areConnectedBodies(Body* body0, Body* body1);

		static bool bodyIsConnector(Body* body);

		static Linkage makeLinkage(SurfaceType s0, SurfaceType s1);

		static NormalId fromNormal(Vector3 normal);

		static Connector* fromLinkageAndPrimitives(Linkage linkage, Primitive* prim0, Primitive* prim1, NormalId surface);

		static JointsService* singleton();

		JointsService()
		{
			setName("JointsService");
			joints = dJointGroupCreate(0);
		}
	};
}