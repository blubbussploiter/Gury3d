
#include <thread>
#include <G3DAll.h>

#include "replicator.h"

#include "rbx_identifiers.h"
#include "idserializer.h"

#include "classes.h"
#include "stdout.h"

#include "scene.h"

RBX::Network::Replicator* r_replicator;

void RBX::Network::Replicator::QueueThinker()
{
    while (1)
    {
        if (!send_queue.empty())
        {
            SendTopQueue();
        }
        Sleep(sendRate * 5);
    }
}

PluginReceiveResult RBX::Network::Replicator::OnReceive(Packet* p)
{
    BitStream data(p->data, p->length, false);

    switch (p->data[0])
    {
        case ID_SEND_INSTANCE:
        {
            RBX::Instance* instance;
            RBX::GuidItem* item;

            data.IgnoreBits(8);

            IdSerializer::NetworkId id;
            id = IdSerializer::deserializeId(data);

            instance = RBX::fromName(id.className);

            item = new RBX::GuidItem(instance, id.guid);
            item->assign();

            DeserializeProperties(data, instance);

            RBX::StandardOut::print(RBX::MESSAGE_INFO, "Replication: %s >> %s-%d", server.ToString(), instance->getName().c_str(), instance->id->guid->data.index);

            break;
        }
        case ID_SEND_PROPERTY:
        {
            RBX::Instance* instance;

            data.IgnoreBits(8);

            int index;
            data >> index;

            instance = Guid::fromIndex(index);
            DeserializeProperty(data, instance);

            break;
        }
        case ID_BRICK_COUNT:
        {

            data.IgnoreBits(8);
            data >> brickCount;

            break;
        }
    }
    return RR_CONTINUE_PROCESSING;
}

void RBX::Network::Replicator::SendSceneInfo()
{
    int inScene;
    BitStream stream;

    RBX::Scene* scene = RBX::Scene::singleton();
    inScene = scene->renderObjects.size();

    stream << (unsigned char)ID_BRICK_COUNT;
    stream << inScene;

    peer->Send(&stream,
        MEDIUM_PRIORITY,
        RELIABLE_ORDERED,
        0,
        server,
        0);
}

void RBX::Network::Replicator::SendTopQueue()
{
    BitStream stream;
    RBX::Instance* instance;

    instance = send_queue.front();
    send_queue.pop();

    stream << (unsigned char)ID_SEND_INSTANCE;
    RBX::Network::IdSerializer::serializeId(stream, instance);
    SerializeProperties(stream, instance);

    RBX::StandardOut::print(RBX::MESSAGE_INFO, "Replication: %s << %s-%d", server.ToString(), instance->getName().c_str(), instance->id->guid->data.index);

    peer->Send(
        &stream,
        MEDIUM_PRIORITY,
        RELIABLE_ORDERED,
        0,
        server,
        0);

}

void RBX::Network::Replicator::SendInstance(RBX::Instance* instance, bool sendChildren)
{
    send_queue.push(instance);

    if (!sendChildren) return;

    RBX::Instances* children = instance->getChildren();

    for (int i = 0; i < children->size(); i++)
    {
        RBX::Instance* c = children->at(i);
        if (c)
        {
            SendInstance(c);
        }
    }

}

void RBX::Network::Replicator::SendProperties(RBX::Instance* instance)
{
    rttr::type instanceType = rttr::type::get_by_name(instance->getClassName());
    if (instanceType)
    {
        for (const auto& prop : instanceType.get_properties())
        {
            SendProperty(instance, prop);
        }
    }
}

void RBX::Network::Replicator::SendProperty(RBX::Instance* instance, const rttr::property property)
{

    BitStream stream;

    stream << (unsigned char)ID_SEND_PROPERTY;
    stream << instance->id->guid->data.index;
    SerializeProperty(stream, instance, property);

    peer->Send(
        &stream,
        MEDIUM_PRIORITY,
        RELIABLE_ORDERED,
        0,
        server,
        0);
}

void RBX::Network::Replicator::SendPlayer(Player* _player)
{
    BitStream data;

    if (!_player) return; /* no player to send */
    player = _player;

    SendInstance(player);

    data << (unsigned char)ID_SEND_REMOTEPLAYER;
    data << player->id->guid->data.index;

    peer->Send(&data,
        MEDIUM_PRIORITY,
        RELIABLE_ORDERED,
        0,
        server, 0);
}

void RBX::Network::Replicator::SerializeProperties(BitStream& stream, RBX::Instance* instance)
{
    rttr::type instanceType = rttr::type::get_by_name(instance->getClassName());

    if (instanceType)
    {
        auto props = instanceType.get_properties();
        stream << props.size();

        for (const auto& prop : props)
        {
            SerializeProperty(stream, instance, prop);
        }
    }
    else
    {
        stream << 0;
    }
}

void RBX::Network::Replicator::SerializeProperty(BitStream& stream, RBX::Instance* instance, rttr::property property)
{
    std::string propertyName;
    propertyName = property.get_name().to_string();

    stream << propertyName.size();
    stream << propertyName.c_str();

    rttr::variant var = property.get_value(instance);

    if (var.get_type() == rttr::type::get<std::string>())
    {
        serialize<std::string>(stream, var.to_string());
    }

    if (var.can_convert<RBX::Instance*>())
    {
        RBX::Instance* value = var.get_value<RBX::Instance*>();
        RBX::GuidItem* item;

        if (value && value->id)
        {
            item = value->id;
            stream << item->guid->data.index; /* ezpz, just write the index of the guid */
        }
        else
        {
            stream << -1; /* null parent */
        }
    }

    if (var.get_type() == rttr::type::get<float>())
    {
        serialize<float>(stream, var.to_float());
    }

    if (var.get_type() == rttr::type::get<double>())
    {
        serialize<double>(stream, var.to_double());
    }

    if (var.get_type() == rttr::type::get<Vector3>())
    {
        serialize<Vector3>(stream, var.convert<Vector3>());
    }

    if (var.get_type() == rttr::type::get<CoordinateFrame>())
    {
        serialize<CoordinateFrame>(stream, var.convert<CoordinateFrame>());
    }

    if (var.get_type() == rttr::type::get<Color3>())
    {
        serialize<Color3>(stream, var.convert<Color3>());
    }

    if (var.get_type() == rttr::type::get<int>())
    {
        stream << var.to_int();
    }
}

void RBX::Network::Replicator::DeserializeProperties(BitStream& stream, RBX::Instance* instance)
{
    size_t numOfProperties;
    stream >> numOfProperties;

    for (size_t i = 0; i < numOfProperties; i++)
    {
        DeserializeProperty(stream, instance);
    }
}

void RBX::Network::Replicator::DeserializeProperty(BitStream& stream, RBX::Instance* instance)
{
    if (!instance) return;

    size_t sz;
    stream >> sz;

    if (!sz) return;

    std::string propertyName;
    propertyName = Help::read(stream, sz);

    rttr::type instanceType = rttr::type::get_by_name(instance->getClassName());

    if (instanceType)
    {
        rttr::property property = instanceType.get_property(propertyName);

        if (property)
        {

            if (property.get_type() == rttr::type::get<std::string>())
            {
                size_t string_size;
                std::string str;

                stream >> string_size;

                if (string_size > 0)
                {
                    str = Help::read(stream, string_size);
                    property.set_value(instance,str);
                }
            }

            if (property.get_type() == rttr::type::get<Instance*>())
            {
                RBX::Instance* v = 0;

                int index;
                stream >> index;

                if (index != -1)
                {
                    v = Guid::fromIndex(index);
                }

                property.set_value(instance, v);
            }

            if (property.get_type() == rttr::type::get<float>())
            {
                deserialize<float>(instance, stream, property);
            }

            if (property.get_type() == rttr::type::get<double>())
            {
                deserialize<double>(instance, stream, property);
            }

            if (property.get_type() == rttr::type::get<Vector3>())
            {
                deserialize<Vector3>(instance, stream, property);
            }

            if (property.get_type() == rttr::type::get<CoordinateFrame>())
            {
                deserialize<CoordinateFrame>(instance, stream, property);
            }

            if (property.get_type() == rttr::type::get<Color3>())
            {
                deserialize<Color3>(instance, stream, property);
            }

            if (property.get_type() == rttr::type::get<int>())
            {
                int value;
                stream >> value;
                property.set_value(instance, value);
            }
        }
    }
}

RBX::Network::Replicator* RBX::Network::Replicator::replicator()
{
    return r_replicator;
}

RBX::Network::Replicator::Replicator(SystemAddress server, RakPeerInterface* peer, int sendRate) : server(server), peer(peer), sendRate(sendRate)
{
    peer->AttachPlugin(this);

    if (!r_replicator)
    {
        r_replicator = this;
    }

    setClassName("Replicator");

    std::thread{
        &RBX::Network::Replicator::QueueThinker,
        this }
    .detach();
}
