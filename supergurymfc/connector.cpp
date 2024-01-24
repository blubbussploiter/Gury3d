#include "jointsservice.h"

bool RBX::Connector::connected()
{
	if (JointsService::areConnectedPrimitives(prim0, prim1)) return 1;
	if (prim0->body && prim1->body)
	{
		return JointsService::areConnectedBodies(prim0->body, prim1->body);
	}
	return 0;
}