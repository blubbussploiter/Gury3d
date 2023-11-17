
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
}

void Player::loadCharacter()
{
	RBX::Instances* load;
	RBX::Humanoid* h = 0;

	if (character) return;

	load = RBX::Serializer::loadInstances(GetFileInPath("/content/font/character.rbxm"));

	if (load && load->size() > 0)
	{
		RBX::Instance* possibleCharacter = load->at(0);
		if (possibleCharacter->getClassName() == "Model")
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

	if (head->getClassName() != "PVInstance" || humanoidRootPart->getClassName() != "PVInstance") return;

	controller = new PlayerController();
	controller->init(this);

	Camera::singleton()->focusPart = head;
	Camera::singleton()->cameraType = Follow;

	character->setName(getName());
	character->buildJoints();

	character->setParent(RBX::Workspace::singleton());

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
	if (!RBX::RunService::singleton()->isRunning) return;
	RBX::ControllerService::singleton()->addController(controller);
	RBX::Camera::singleton()->disable(1);
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

void RBX::Network::Players::setPlayerList(RBX::Gui::GuiList* _playerList)
{
	playerList = _playerList;
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

	playerList->visible = true;
	playerList->addChild(lbl);
}

void Players::updatePlayerList()
{
	for (unsigned int i = 0; i < players.size(); i++)
	{
		Player* p = players.at(i);
		RBX::Gui::GuiLabel* lbl = p->getGuiName();

		if (lbl && lbl->title != p->getName())
		{
			lbl->title = p->getName();
		}
	}
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
