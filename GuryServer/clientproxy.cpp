#include "idserializer.h"
#include "networkserver.h"
#include "guid.h"

#include "slikenet/BitStream.h"

#include "datamodel.h"
#include "lighting.h"

PluginReceiveResult RBX::Network::NetworkServer::ClientProxy::OnReceive(Packet* p)
{

    BitStream data(p->data, p->length, false);

    switch (p->data[0])
    {
        case ID_SEND_REMOTEPLAYER:
        {

            data.IgnoreBits(8);

            int id;
            data >> id;

            printf("data = %d\n", id);

            break;
        }
    }

    return RR_CONTINUE_PROCESSING;
}

void RBX::Network::NetworkServer::ClientProxy::sendTop()
{
    BitStream stream;

    stream << (unsigned char)ID_SEND_GLOBALS;
    stream << RBX::Guid::getLocalScope().c_str();

    uint32_t id = server->server->Send(
        &stream,
        MEDIUM_PRIORITY,
        RELIABLE_ORDERED,
        0,
        client,
        0);

}

void RBX::Network::NetworkServer::ClientProxy::sendTopLevelInstances()
{
    RBX::Datamodel* datamodel = RBX::Datamodel::getDatamodel();

    SendSceneInfo();

    SendInstance(datamodel->lighting);
    SendInstance(datamodel->workspace);

}
