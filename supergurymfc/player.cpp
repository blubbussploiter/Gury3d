
#include "camera.h"
#include "part.h"

#include "mesh.h"

#include "humanoid.h"

#include "serializer.h"
#include "replicator.h"

#include "players.h"
#include "workspace.h"

using namespace RBX::Network;


RTTR_REGISTRATION
{
	rttr::registration::class_<RBX::Network::Player>("Player")
		.constructor<>()
		.property("userId", &RBX::Network::Player::getUserId, &RBX::Network::Player::setUserId)(rttr::metadata("Type", RBX::Data))
		.property("AdminMode", &RBX::Network::Player::getAdminMode, &RBX::Network::Player::setAdminMode)(rttr::metadata("Type", RBX::Data))
		.property_readonly("Character", &RBX::Network::Player::getCharacter)
		.property_readonly("Backpack", &RBX::Network::Player::getBackpack)
		.method("loadCharacter", &RBX::Network::Player::loadCharacter)
		.method("LoadCharacter", &RBX::Network::Player::loadCharacter);
}

void Player::loadCharacter()
{
	RBX::Instances* load;
	RBX::Humanoid* h = 0;

	if (character)
	{
		character->remove();
	}

	load = RBX::Serializer::loadInstances(GetFileInPath("/content/font/character.rbxm"));

	if (load && load->size() > 0)
	{
		RBX::ModelInstance* possibleCharacter = toInstance<ModelInstance>(load->at(0));
		if (possibleCharacter)
		{
			h = RBX::Humanoid::modelIsCharacter(possibleCharacter);
			if (h)
			{
				character = (RBX::ModelInstance*)possibleCharacter;
			}
		}
	}

	if (!character) return;

	RBX::PartInstance* head, * humanoidRootPart;

	head = (RBX::PartInstance*)character->findFirstChild("Head");
	humanoidRootPart = (RBX::PartInstance*)character->findFirstChild("Torso");

	if (!head || !humanoidRootPart) return;

	if (!IsA<RBX::PartInstance>(head) || !IsA<RBX::PartInstance>(humanoidRootPart)) return;

	Extents e, ce;
	Vector3 size, pos;

	Workspace* workspace;

	workspace = Workspace::singleton();
	e = workspace->computeVisibleExtents().toWorldSpace(CoordinateFrame());
	ce = character->computeVisibleExtents().toWorldSpace(CoordinateFrame());

	pos = Vector3(0, e.high.y + (ce.high.y * 2), 0);

	character->translate(pos);

	controller = new PlayerController();
	controller->init(this);

	Camera::singleton()->focusPart = head;
	Camera::singleton()->cameraType = Follow;

	character->setName(getName());
	character->setParent(workspace);

	if (RBX::RunService::singleton()->isRunning)
	{
		setAsController();
	}

}

void Player::disposeActiveBin()
{
	if (!activeBin)
		return;

	activeBin->remove();
	activeBin = 0;
}

void Player::setAsController()
{
	if (character)
	{
		RBX::ControllerService::singleton()->addController(controller);
		RBX::Camera::singleton()->disable(1);
	}
}

void RBX::Network::Player::render(RenderDevice* rd)
{
}
