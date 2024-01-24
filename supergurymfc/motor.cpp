#include "motor.h"

void RBX::MotorConnector::build()
{
	if (!body0 || !body1 
		|| (body0 && !body0->body) || (body1 && !body1->body)) return;

	motor = dJointCreateAMotor(Kernel::get()->world, JointsService::singleton()->joints);

	dJointAttach(motor, body0->body, body1->body);
}