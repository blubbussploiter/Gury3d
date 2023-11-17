
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
	rttr::registration::class_<RBX::Network::Players>("Players")
		.constructor<>()
		.property_readonly("LocalPlayer", &RBX::Network::Players::getLocalPlayer)
			.method("createLocalPlayer", &RBX::Network::Players::createLocalPlayer);
	rttr::registration::class_<RBX::Network::Player>("Player")
	.constructor<>()
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
		RBX::ModelInstance* possibleCharacter = dynamic_cast<RBX::ModelInstance*>(load->at(0));
		if (possibleCharacter)
		{
			h = RBX::Humanoid::modelIsCharacter(possibleCharacter);
			if(h)
				character = (RBX::ModelInstance*)possibleCharacter;
		}
	}
	
	if (!character) return;

	RBX::PartInstance* head, *humanoidRootPart;

	head = (RBX::PartInstance*)character->findFirstChild("Head");
	humanoidRootPart = (RBX::PartInstance*)character->findFirstChild("Torso");

	if (!head || !humanoidRootPart) return;

	if (!IsA<RBX::PartInstance>(head) || !IsA<RBX::PartInstance>(humanoidRootPart)) return;

	controller = new PlayerController();
	controller->init(this);

	Camera::singleton()->focusPart = head;
	Camera::singleton()->cameraType = Follow;

	character->setName(getName());
	character->setParent(RBX::Workspace::singleton());

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

void Players::createLocalPlayer(int userId)
{
	Player* player;

	if (localPlayer) return; /* 'localplayer already exists!' */

	player = new Player();

	if (userId == 0)
		userId++;

	player->userId = userId;
	player->setName("Player");

	localPlayer = player;
	addPlayer(player);

}

void Players::destroyPlayer(Player* plr)
{
	players.erase(std::remove(players.begin(), players.end(), plr));
	delete& plr;
}

void Players::addPlayer(Player* player)
{
	RBX::Gui::GuiLabel* lbl;

	lbl = new RBX::Gui::GuiLabel();

	lbl->textColor = Color3::wheelRandom();
	lbl->title = player->getName();
	lbl->sz = 10;

	players.push_back(player);
}

RBX::ModelInstance* Players::findLocalCharacter()
{
	Players* players = getPlayers();
	Player* player;

	player = players->localPlayer;

	if (player && (player->character))
		return player->character;

	return 0;
}

Players* RBX::Network::getPlayers()
{
	return RBX::Datamodel::getDatamodel()->players;
}
