#include "hinge.h"

void RBX::HingeConnector::build()
{
	if (!body0 || !body1
		|| (body0 && !body0->body) || (body1 && !body1->body) || (body0 && body1 && dAreConnected(body0->body, body1->body))) return;

	void* data = prim0->getUserdata();
	Instance* instance = static_cast<Instance*>(data);
	PVInstance* pvInstance = dynamic_cast<PVInstance*>(instance);

	if (!pvInstance) return;

	Vector3 axis, anchor;
	axis = RBX::getNormalFromNormalId(pvInstance, connectedAt);
	anchor = (body0->getPosition() + body1->getPosition()) / 2;

	hinge = dJointCreateHinge(Kernel::get()->world, JointsService::get()->joints);
	dJointAttach(hinge, body0->body, body1->body);

	dJointSetHingeAnchor(hinge, anchor.x, anchor.y, anchor.z);
	dJointSetHingeAxis(hinge, axis.x, axis.y, axis.z);

}
