#include "jointsservice.h"

#include "snap.h"
#include "motor.h"

#include "scene.h"

void RBX::JointsService::collisionCallback(void* data, dGeomID o1, dGeomID o2)
{
	dContact contact[4];

	int i;
	int numc = dCollide(o1, o2, 4, &contact[0].geom, sizeof(dContactGeom));

	RBX::PVInstance* pv0, * pv1;

	pv0 = static_cast<PVInstance*>(dGeomGetData(o1));
	pv1 = static_cast<PVInstance*>(dGeomGetData(o2));

	if (!pv0 || !pv1)
	{
		return;
	}

	if (numc > 0)
	{
		for (i = 0; i < numc; i++)
		{
			NormalId n0, n1;

			dReal* normal = contact[i].geom.normal;

			n0 = fromNormal(Vector3(normal[0], normal[1], normal[2]));
			n1 = fromNormal(Vector3(-normal[0], -normal[1], -normal[2]));

			SurfaceType s0, s1;
			s0 = pv0->getSurface(n0);
			s1 = pv1->getSurface(n1);

			Linkage link = makeLinkage(s0, s1);

			if(link != NotLinked)
			{
				JointsService::singleton()->addConnector(fromLinkageAndPrimitives(link, pv0->primitive, pv1->primitive));
			}
		}
	}

}

RBX::Connector* RBX::JointsService::fromLinkageAndPrimitives(Linkage linkage, Primitive* prim0, Primitive* prim1)
{
	switch (linkage)
	{
	case Motored:
	{
		return new MotorConnector(prim0, prim1);
	}
	case Snapped:
	{
		return new SnapConnector(prim0, prim1);
	}
	}
	return 0;
}

RBX::NormalId RBX::JointsService::fromNormal(Vector3 normal)
{
	if (normal.y == 1)
		return TOP;
	if (normal.y == -1)
		return BOTTOM;
	if (normal.x == 1)
		return RIGHT;
	if (normal.x == -1)
		return LEFT;
	if (normal.z == 1)
		return FRONT;
	if (normal.z == -1)
		return BACK;
	return UNDEFINED;
}

bool RBX::JointsService::areConnectedPrimitives(Primitive* prim0, Primitive* prim1)
{
	if (!prim0->geom[0] || !prim1->geom[0]) return false;
	if (!bodyIsConnector(prim0->body) || !bodyIsConnector(prim1->body)) return false;
	return dGeomGetBody(prim0->geom[0]) == dGeomGetBody(prim1->geom[0]);
}

bool RBX::JointsService::areConnectedBodies(Body* body0, Body* body1)
{
	if (!body0->body || !body1->body) return false;
	return dAreConnectedExcluding(body0->body, body1->body, dJointTypeContact);
}

bool RBX::JointsService::bodyIsConnector(Body* body)
{
	if (!body) return 0;
	SnapConnector* data = (SnapConnector*)dBodyGetData(body->body);
	return data != 0;
}

RBX::Linkage RBX::JointsService::makeLinkage(SurfaceType s0, SurfaceType s1)
{
	if ((s0 == Studs || s1 == Studs) && (s0 == Inlet || s1 == Inlet)) return Snapped;
	if (s0 == Motor || s1 == Motor) return Motored;
	return NotLinked;
}

void RBX::JointsService::addConnector(Connector* connector)
{
	if (!connector) return;
	if (areConnectedPrimitives(connector->prim0, connector->prim1)) return;
	connectors.push_back(connector);
}

void RBX::JointsService::buildConnectors()
{
	for (int i = 0; i < connectors.size(); i++)
	{
		Connector* connector = connectors[i];
		if (!connector->connected())
		{
			connector->build();
			connector->setParent(this);
		}
	}
}

void RBX::JointsService::buildGlobalJoints()
{
	dSpaceCollide(Kernel::get()->space, 0, &JointsService::collisionCallback);
}

RBX::JointsService* RBX::JointsService::singleton()
{
    return RBX::Datamodel::getDatamodel()->jointService;
}
