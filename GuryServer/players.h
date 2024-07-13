#ifndef PLAYERS_H
#define PLAYERS_H

#include "GuiRoot.h"

#include "model.h"
#include "controller.h"

#include "backpack.h"
#include "service.h"

namespace RBX
{
	namespace Network
	{
		class Players;
		class Player;

		Players* getPlayers();

		class Players :
			public Instance,
			public RBX::Service<Players>
		{
		private:

			int maxPlayers;

			std::vector<Player*> players;
			RBX::Gui::GuiList* playerList;
		public:

			Player* localPlayer;
			Player* getLocalPlayer() { return localPlayer; }

			Player* createLocalPlayer(int userId);

			void setPlayerList(RBX::Gui::GuiList* playerList);
			void destroyPlayer(Player* plr);

			void addPlayer(Player* plr);
			void updatePlayerList();

			void onStep();

			int getNumPlayers()
			{
				return players.size();
			}

			void setMaxPlayers(int newMaxPlayers) 
			{ 
				maxPlayers = newMaxPlayers;
			}
			int getMaxPlayers()
			{
				return maxPlayers; 
			}

			/* unlike actual function, no arguments, instead dependant on there being a global Players class */

			static RBX::ModelInstance* findLocalCharacter(); 
			Players()
			{
				setClassName("Players");
				setName("Players");

				isParentLocked = 1;
				localPlayer = 0;
			}

			RTTR_ENABLE(RBX::Instance)
		};

		class PlayerController : public RBX::Controller
		{
		private:
			Player* plr;

			float jmpPower;
			float camYaw;

		public:

			void mv_update();
			void move();

			void init(Player* _plr) { if (!plr) plr = _plr; jmpPower = 30; }
		};

		class Player : 
			public RBX::Instance
		{
		private:
			RBX::Gui::GuiLabel* guiName; /* player name in list */
			PlayerController* controller;
		public:

			int userId;
			bool adminMode;

			RBX::Backpack* backpack;
			RBX::HopperBin* activeBin;

			RBX::ModelInstance* character;

			RBX::Gui::GuiLabel* getGuiName() { return guiName; }
			void setGuiName(RBX::Gui::GuiLabel* lbl) { guiName = lbl; }

			RBX::Backpack* getBackpack() { return backpack; }
			RBX::ModelInstance* getCharacter() { return character; }

			int getUserId() { return userId; }
			void setUserId(int newUserId) { userId = newUserId; }

			bool getAdminMode() { return adminMode; }
			void setAdminMode(bool newAdminMode) { adminMode = newAdminMode; }

			void loadCharacter();
			void disposeActiveBin();

			void setAsController();
			void render(RenderDevice* rd);

			Player()
			{
				setClassName("Player");
				setName("Player");
				setParent(getPlayers());
				backpack = new RBX::Backpack();
				backpack->setParent(this);
				activeBin = 0;
				controller = 0;
				character = 0;
				guiName = 0;
				userId = 0;
			}
			~Player()
			{
				getPlayers()->destroyPlayer(this);
			}
			RTTR_ENABLE(RBX::Instance)
		};

	}
}

#endif PLAYERS_H