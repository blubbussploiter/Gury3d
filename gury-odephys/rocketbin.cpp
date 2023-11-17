#include "Mouse.h"
#include "rocketbin.h"
#include "selection.h"
#include "workspace.h"
#include "humanoid.h"
#include "players.h"
#include "camera.h"
#include "part.h"

void RBX::RocketBin::update(RenderDevice* rd)
{
	RBX::Camera* camera = RBX::Camera::singleton();
	RBX::ModelInstance* character;
	RBX::Humanoid* humanoid;

	character = RBX::Network::getPlayers()->findLocalCharacter();
	if (!character) return;
	humanoid = RBX::Humanoid::modelIsCharacter(character);
	if (!humanoid)return;

	if (RBX::Selection::clicked)
	{

		RBX::PartInstance* part = new RBX::PartInstance();
		RBX::PVInstance* head = humanoid->humanoidRootPart;
		rocket = new RBX::Rocket();

		Vector3 origin, look, turn;

		if (!head) return;

		look = head->getCFrame().lookVector() * 2;
		turn = camera->getCoordinateFrame().lookVector();
		origin = head->getPosition() + look;

		part->lookAt(turn);

		part->setParent(Services::getWorkspace());
		part->setPosition(origin);
		part->setSize(Vector3(1, 1.2, 4));
		part->setColor(bcToRGB(23));

		part->setFace(FRONT, Studs);
		part->setFace(BACK, Studs);
		part->setFace(RIGHT, Studs);
		part->setFace(LEFT, Studs);

		Runtime::getRunService()->getPhysics()->createBody(part);
		rocket->setParent(part);

		RBX::Selection::clicked = 0;
	}

	if (active)
	{
		Rendering::cursor_custom = Texture::fromFile(GetFileInPath("/content/textures/GunCursor.png"));
		if (rocket)
			rocket->step();
	}
}

void RBX::RocketBin::deactivate()
{
	Rendering::cursor_custom = 0;
}