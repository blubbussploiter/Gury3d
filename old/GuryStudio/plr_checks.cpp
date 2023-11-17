#include "players.h"
#include "ray.h"

bool Services::Players::PlayerController::isGrounded()
{
	Rendering::Renderables::PVInstance* pv1;
	Rendering::Renderables::PVInstance* pv2;

	Rendering::Renderables::PVInstance* touched1;
	Rendering::Renderables::PVInstance* touched2;

	pv1 = (Rendering::Renderables::PVInstance*)plr->character->findFirstChild("Right Leg");
	pv2 = (Rendering::Renderables::PVInstance*)plr->character->findFirstChild("Left Leg");

	if (!pv1 || !pv2)
		return 0;

	touched1 = pv1->body->touchedPV;
	touched2 = pv2->body->touchedPV;

	if (!touched1 || !touched2)
		return 0;

	return (touched1 && touched2 && (touched1->getParent() != plr->character && touched2->getParent() != plr->character));
}

bool Services::Players::PlayerController::isTipped()
{
	return 0;
}


bool Services::Players::PlayerController::isInAir()
{
	Rendering::Renderables::PVInstance* primaryPart;
	const dReal* vel;

	primaryPart = plr->character->primaryPart;
	vel = dBodyGetLinearVel(primaryPart->body->body);

	return (vel[1] >= 0.1);
}

bool Services::Players::PlayerController::isFalling()
{
	const dReal* vel;
	vel = getFallVelocity();
	return -0.1 > vel[1];
}

const float* Services::Players::PlayerController::getFallVelocity()
{
	return dBodyGetLinearVel(plr->character->primaryPart->body->body);
}