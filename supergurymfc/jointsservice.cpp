#include "jointsservice.h"

#include "snap.h"
#include "motor.h"

#include "scene.h"

RBX::Connector* RBX::JointsService::fromLinkageAndPrimitives(Linkage linkage, Primitive* prim0, Primitive* prim1, NormalId surface)
{
	switch (linkage)
	{
	case Motored:
	{
		return new MotorConnector(prim0, prim1, surface);
	}
	case Welded:
	case Glued:
	case Snapped:
	{
		return new SnapConnector(prim0, prim1, surface);
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


bool RBX::JointsService::areConnectedUnbuiltPrimitives(Primitive* prim0, Primitive* prim1)
{

	/* before build(), compare prim0 and prim1 */

	for (unsigned int i = 0; i < connectors.size(); i++)
	{
		Connector* connector = connectors[i];
		if (connector)
		{
			if (connector->prim0 == prim0 && connector->prim1 == prim1 ||
				connector->prim0 == prim1 && connector->prim1 == prim0)
				return true;
		}
	}

	return false;

}

bool RBX::JointsService::areConnectedBodies(Body* body0, Body* body1)
{
	if (!body0->body || !body1->body) return false;
	return dAreConnectedExcluding(body0->body, body1->body, dJointTypeContact);
}

bool RBX::JointsService::bodyIsConnector(Body* body)
{
	if (!body) return 0;
	Connector* data = (Connector*)dBodyGetData(body->body);
	return data != 0;
}

RBX::Linkage RBX::JointsService::makeLinkage(SurfaceType s0, SurfaceType s1)
{
	if ((s0 == Studs || s1 == Studs) && (s0 == Inlet || s1 == Inlet)) return Snapped;
	if (s0 == Motor || s1 == Motor) return Motored;
	if (s0 == Weld || s1 == Weld) return Welded;
	if (s0 == Glue || s1 == Glue) return Glued;
	return NotLinked;
}

void RBX::JointsService::addConnector(Connector* connector)
{
	if (!connector) return;
	if (areConnectedUnbuiltPrimitives(connector->prim0, connector->prim1)) return;
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
	Experiment::buildGlobalJoints();
}

RBX::JointsService* RBX::JointsService::singleton()
{
    return RBX::Datamodel::getDatamodel()->jointService;
}