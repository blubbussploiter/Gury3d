#include <math.h>

#include "players.h"
#include "camera.h"

#include "runservice.h"
#include "sounds.h"

#include "humanoid.h"

void RBX::Network::PlayerController::mv_update()
{
	PVInstance* primaryPart;
	RBX::ModelInstance* c;
	RBX::Humanoid* h;

	c = RBX::Network::Players::findLocalCharacter();

	if (!c) return;
	h = c->findFirstChildOfClass<RBX::Humanoid>("Humanoid");
	if (!h) return;

	primaryPart = plr->character->getPrimaryPart();

	if (!primaryPart)
		return;

	if (!moving())
	{
		h->setWalkDirection(Vector3::zero());
	}

}

void RBX::Network::PlayerController::move()
{
	CoordinateFrame o;
	Vector3 mov = Vector3::zero();
	Vector3 look;
	RBX::Camera* camera;
	RBX::ModelInstance* character;
	RBX::Humanoid* humanoid;

	character = RBX::Network::Players::findLocalCharacter();

	if (!character || (character && !RBX::Humanoid::modelIsCharacter(character))) return;

	humanoid = RBX::Humanoid::modelIsCharacter(character);

	camera = RBX::Camera::singleton();
	o = camera->cframe;
	look = o.lookVector();

	for (unsigned int i = 0; i < directions.size(); i++)
	{
		MovementDirections dir = directions.at(i);
		switch (dir)
		{
			/* vertical */
		case RBX::Forward:
		{
			mov = normalize(o.lookVector());
			break;
		}
		case RBX::Backwards:
		{
			mov = normalize(-o.lookVector());
			break;
		}
		/* horizontal */
		case RBX::Right:
		{
			mov = normalize(o.rightVector());
			break;
		}
		case RBX::Left:
		{
			mov = normalize(-o.rightVector());
			break;
		}
		case RBX::Jump:
		{
			setdir(Idle);
			humanoid->setJump();
			break;
		}
		case RBX::Idle: 
		{
			directions.clear();
			break; }
		}
	}

	humanoid->setWalkDirection(mov);
}