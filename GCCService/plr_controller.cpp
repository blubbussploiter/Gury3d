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
	Vector3 mov;
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

	switch (dir())
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
	/* vertical + horizontal.. */
	case RBX::ForwardRight:
	{
		mov += o.getLookVector() + o.rightVector();
		break;
	}
	case RBX::ForwardLeft:
	{
		mov += o.getLookVector() - o.rightVector();
		break;
	}
	case RBX::BackwardsRight:
	{
		mov -= o.getLookVector() - o.rightVector();
		break;
	}
	case RBX::BackwardsLeft:
	{
		mov -= o.getLookVector() + o.rightVector();
		break;
	}
	case RBX::Jump: { humanoid->setJump(); setdir(Idle); return; }
	case RBX::Idle: { return; }
	}

	humanoid->setWalkDirection(mov);
}