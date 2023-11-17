#pragma once

#include "slikenet/PluginInterface2.h"
#include "slikenet/RakPeerInterface.h"
#include "slikenet/MessageIdentifiers.h"

#include "rbx_identifiers.h"
#include "replicator.h"

#include "instance.h"
#include "datamodel.h"

using namespace SLNet;

namespace RBX
{
	namespace Network
	{

		class NetworkServer : public RBX::Instance, public PluginInterface2
		{
			class ClientProxy;
		private:
			RakPeerInterface* server;
			unsigned short maxPlayers, numPlayers;
			int port, threadSleep;
		public:

			RakPeerInterface* getPeer() { return server; }

			void doThreadLogic();
			void start(int port, int threadSleep=30);

			PluginReceiveResult OnReceive(Packet* packet); /* handles base raknet stuff */

			static NetworkServer* networkServer();

			class ClientProxy : public Replicator
			{
			private:
				RBX::Network::Player* remotePlayer; /* first player object replicated from client? */
				RBX::Network::NetworkServer* server;
				SystemAddress client;
			public:

				PluginReceiveResult OnReceive(Packet* p);

				void sendTop();
				void sendTopLevelInstances(); /* set uuids for things like workspace, lighting, etc. */

				ClientProxy(SystemAddress address, RBX::Network::NetworkServer* _server, int sendRate=5) : Replicator(address, _server->server, sendRate)
				{
					server = _server;
					client = address;
					remotePlayer = 0;
				}
			};

			NetworkServer()
			{
				numPlayers = 0;
				maxPlayers = 10;
				port = 0;
				server = 0;
				threadSleep = 30;
				setClassName("NetworkServer");
				setParent(Datamodel::getDatamodel());
			}
		};
	}
}