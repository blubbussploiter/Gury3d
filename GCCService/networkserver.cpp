#include <thread>

#include "guid.h"
#include "networkserver.h"
#include "stdout.h"

RBX::Network::NetworkServer* s_networkServer;

#pragma comment(lib, "SLikeNet_LibStatic_Release_Win32.lib")

void RBX::Network::NetworkServer::doThreadLogic()
{
	RBX::StandardOut::print(MESSAGE_INFO, "Starting network server on port %d", port);
	RBX::StandardOut::print(MESSAGE_INFO, "My IP Addresses are:");

	for (unsigned int i = 0; i < server->GetNumberOfAddresses(); i++)
	{
		const char* ip = server->GetLocalIP(i);
		RBX::StandardOut::print(MESSAGE_INFO, "%s", ip);
	}

	while (1)
	{
		Packet* p = server->Receive();
		if (p)
		{
			server->DeallocatePacket(p);
			p = server->Receive();
		}
		Sleep(threadSleep);
	}

	RakPeerInterface::DestroyInstance(server);
}

PluginReceiveResult RBX::Network::NetworkServer::OnReceive(Packet* packet)
{
	BitStream data(packet->data, packet->length, false);

	switch (packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
		{
			ClientProxy* proxy;

			RBX::StandardOut::print(
				RBX::MESSAGE_INFO,
				"New connection from %s", 
				packet->systemAddress.ToString(1));

			proxy = new ClientProxy(packet->systemAddress, this);

			proxy->setParent(this);

			proxy->sendTop();
			proxy->sendTopLevelInstances();
			
			break;
		}
	}
	return RR_CONTINUE_PROCESSING;
}

void RBX::Network::NetworkServer::start(int _port, int _threadSleep)
{

	port = _port;

	SocketDescriptor sd;

	server = RakPeerInterface::GetInstance();
	threadSleep = _threadSleep;

	sd.port = port;
	sd.socketFamily = AF_INET; // Test out IPV4				

	StartupResult b = server->Startup(maxPlayers, &sd, 1);

	if (b)
	{
		throw std::runtime_error("Failed to start NetworkServer");
		return;
	}

	server->SetMaximumIncomingConnections(maxPlayers);
	server->SetOccasionalPing(true);
	server->AttachPlugin(this);

	std::thread
	{
	&NetworkServer::doThreadLogic, 
	this 
	}.detach();

}

RBX::Network::NetworkServer* RBX::Network::NetworkServer::networkServer()
{
	if (!s_networkServer) s_networkServer = new RBX::Network::NetworkServer();
	return s_networkServer;
}
