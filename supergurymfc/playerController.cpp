#include <math.h>

#include "players.h"
#include "camera.h"

#include "runservice.h"
#include "sounds.h"

#include "humanoid.h"

void RBX::Network::PlayerController::move()
{
	RBX::ModelInstance* character = player->character;
	RBX::Humanoid* humanoid;
	
	if (character)
	{
		humanoid = character->findFirstChildOfClass<Humanoid>("Humanoid");

		if (!moving())
		{
			humanoid->setWalkDirection(Vector3::zero());
			return;
		}
			
		CoordinateFrame cameraCoord = Camera::get()->cframe;
		Vector3 movementHorizontal, movementVertical;
		
		movementHorizontal = cameraCoord.rightVector() * horizontal;
		movementVertical = cameraCoord.lookVector() * vertical;
		
		humanoid->setWalkDirection(normalize(movementHorizontal + movementVertical));
	}
}