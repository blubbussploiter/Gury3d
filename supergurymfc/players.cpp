
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
		.property("maxPlayers", &RBX::Network::Players::getMaxPlayers, &RBX::Network::Players::setMaxPlayers)(rttr::metadata("Type", RBX::Data))
		.property_readonly("numPlayers", &RBX::Network::Players::getNumPlayers)(rttr::metadata("Type", RBX::Data))
		.method("createLocalPlayer", &RBX::Network::Players::createLocalPlayer);
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

	RBX::PartInstance* head, *humanoidRootPart;

	head = (RBX::PartInstance*)character->findFirstChild("Head");
	humanoidRootPart = (RBX::PartInstance*)character->findFirstChild("Torso");

	if (!head || !humanoidRootPart) return;

	if (!IsA<RBX::PartInstance>(head) || !IsA<RBX::PartInstance>(humanoidRootPart)) return;

	Extents e;
	Vector3 size, pos;

	Workspace* workspace;

	workspace = Workspace::singleton();
	e = workspace->computeVisibleExtents();

	size = e.size();
	//pos = Vector3(0, size.magnitude() * 0.05f, 0);
	pos = Vector3(0, 100, 0);

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

void Players::createLocalPlayer(int userId)
{
	Player* player;

	if (localPlayer)
	{
		throw std::runtime_error("Local player already exists");
	}

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

void RBX::Network::Players::onStep()
{
	if (localPlayer)
	{
		localPlayer->backpack->updateGui();
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
