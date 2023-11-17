#pragma once

#include "slikenet/PluginInterface2.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/RakPeerInterface.h"

#include "datamodel.h"
#include "service.h"

using namespace SLNet;

namespace RBX
{
	namespace Network
	{
		class NetworkClient :
			public RBX::Instance, RBX::Service<NetworkClient>, public PluginInterface2
		{
		private:
			RakPeerInterface* client;
			int threadSleep;
		public:
			void doThreadLogic();
			void connect(std::string server, int serverPort, int clientPort=0, int threadSleep = 30);
			PluginReceiveResult OnReceive(Packet* p);
			static NetworkClient* networkClient();
			NetworkClient()
			{
				setName("NetworkClient");
				setClassName("NetworkClient");
				Datamodel::getDatamodel()->addToExplorerWindow(this);
			}
		};
	}
}