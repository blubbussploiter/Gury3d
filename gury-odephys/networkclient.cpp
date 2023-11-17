#include <thread>

#include "replicator.h"
#include "networkclient.h"
#include "stdout.h"

RBX::Network::NetworkClient* s_networkClient;

#pragma comment(lib, "SLikeNet_LibStatic_Release_Win32.lib")

void RBX::Network::NetworkClient::doThreadLogic()
{
    while (1)
    {
        Packet* p = client->Receive();
        if (p)
        {
            client->DeallocatePacket(p);
            p = client->Receive();
        }

        Sleep(threadSleep);
    }

    RakPeerInterface::DestroyInstance(client);
}

PluginReceiveResult RBX::Network::NetworkClient::OnReceive(Packet* p)
{
    switch (p->data[0])
    {
        case ID_CONNECTION_REQUEST_ACCEPTED:
        {
            Replicator* proxy;

            RBX::StandardOut::print(
                RBX::MESSAGE_INFO,
                "Connection accepted from %s\n",
                p->systemAddress.ToString(1));

            proxy = new Replicator
            (
                p->systemAddress,
                client
            );

            proxy->setParent(this);

            break;
        }
        case ID_CONNECTION_BANNED:
        case ID_CONNECTION_ATTEMPT_FAILED:
        {
            RBX::StandardOut::print(
                RBX::MESSAGE_WARNING,
                "Failed to connect to %s, %d",
                p->systemAddress.ToString(1), p->data[0]);
            break;
        }
    }           
    return RR_CONTINUE_PROCESSING;
}

void RBX::Network::NetworkClient::connect(std::string server, int serverPort, int clientPort, int _threadSleep)
{
    client = RakPeerInterface::GetInstance();

    SocketDescriptor socketDescriptor(clientPort, 0);
    socketDescriptor.socketFamily = AF_INET;

    client->Startup(1, &socketDescriptor, 1);
    threadSleep = _threadSleep;

    ConnectionAttemptResult r = client->Connect(server.c_str(), serverPort, 0, 0);

    if (r)
    {
        throw std::runtime_error("Failed to create connection attempt");
    }

    client->SetOccasionalPing(true);
    client->AttachPlugin(this);

    std::thread
    {
    &NetworkClient::doThreadLogic,
    this
    }.detach();
}

RBX::Network::NetworkClient* RBX::Network::NetworkClient::networkClient()
{
    if (!s_networkClient) s_networkClient = new RBX::Network::NetworkClient();
    return s_networkClient;
}
