#pragma once

#include <queue>

#include "slikenet/PluginInterface2.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/RakPeerInterface.h"
#include "slikenet/Bitstream.h"

#include "players.h"
#include "workspace.h"

#include "bitstreamHelp.h"
#include "guid.h"

using namespace SLNet;

namespace RBX
{
	namespace Network
	{
		class Replicator : public RBX::Instance, public PluginInterface2
		{
			friend class PhysicsSender;
		private:
			std::string serverScope;

			SystemAddress server;
			SystemAddress remotePlayerId;

			RakPeerInterface* peer;
			std::queue<RBX::Instance*> send_queue;

			Player* player;

			int sendRate;
			int brickCount, receivedBrickCount;
		public:

			PluginReceiveResult OnReceive(Packet* p);

			void SendInstance(RBX::Instance* instance, bool sendChildren = true);

			void SerializeProperties(BitStream& stream, RBX::Instance* instance);
			void DeserializeProperties(BitStream& stream, RBX::Instance* instance);

			void SerializeProperty(BitStream& stream, RBX::Instance* instance, rttr::property property);
			void DeserializeProperty(BitStream& stream,
				RBX::Instance* instance);

			void SendProperties(RBX::Instance* instance);
			void SendProperty(RBX::Instance* instance, const rttr::property property);

			void SendPlayer(Player* player);

			void SendSceneInfo();

			void SendTopQueue();
			void QueueThinker();

			static Replicator* replicator();

			bool connected() 
			{
				if (!peer) return 0;
				return peer->GetConnectionState(server) == IS_CONNECTED;
			}

			Player* getPlayer()
			{
				return player;
			}

			/* serialize */

			template<typename T>
			void serialize(BitStream& stream, T value) {}

			template<typename T>
			void serialize(BitStream& stream, double value)
			{
				stream.WriteBits((unsigned char*)&value, 32);
			}

			template<typename T>
			void serialize(BitStream& stream, float value)
			{
				stream.WriteBits((unsigned char*)&value, 32);
			}

			template<typename T>
			void serialize(BitStream& stream, int value)
			{
				stream << value;
			}

			template<typename T>
			void serialize(BitStream& stream, std::string value)
			{
				stream << value.size();
				stream << value.c_str();
			}

			template<typename T>
			void serialize(BitStream& stream, CoordinateFrame value)
			{
				Vector3 v = value.translation;
				Matrix3 m = value.rotation;

				serialize<Vector3>(stream, v);
				stream.WriteBits((unsigned char*)&m.elt[0][0], 32);
				stream.WriteBits((unsigned char*)&m.elt[0][1], 32);
				stream.WriteBits((unsigned char*)&m.elt[0][2], 32);
				stream.WriteBits((unsigned char*)&m.elt[1][0], 32);
				stream.WriteBits((unsigned char*)&m.elt[1][1], 32);
				stream.WriteBits((unsigned char*)&m.elt[1][2], 32);
				stream.WriteBits((unsigned char*)&m.elt[2][0], 32);
				stream.WriteBits((unsigned char*)&m.elt[2][1], 32);
				stream.WriteBits((unsigned char*)&m.elt[2][2], 32);
			}

			template<typename T>
			void serialize(BitStream& stream, Vector3 value)
			{
				stream.WriteBits((unsigned char*)&value.x, 32);
				stream.WriteBits((unsigned char*)&value.y, 32);
				stream.WriteBits((unsigned char*)&value.z, 32);
			}

			template<typename T>
			void serialize(BitStream& stream, Color3 value)
			{
				stream.WriteBits((unsigned char*)&value.r, 32);
				stream.WriteBits((unsigned char*)&value.g, 32);
				stream.WriteBits((unsigned char*)&value.b, 32);
			}

			template<typename T>
			void serialize(BitStream& stream, Instance* value)
			{
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

			/* deserialize */

			template <class T>
			void deserialize(RBX::Instance* instance, BitStream& stream, rttr::property& property) {}

			template<>
			void deserialize<std::string>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
			{
				size_t string_size;
				std::string str;

				stream >> string_size;
				if (string_size > 0)
				{
					property.set_value(instance, Help::read(stream, string_size));
				}
			}

			template<>
			void deserialize<RBX::Instance>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
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

			template <>
			void deserialize<float>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
			{
				float f = 0.0f;

				stream.ReadBits((unsigned char*)&f, 32);
				property.set_value(instance, f);

			}

			template<>
			void deserialize<double>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
			{
				double d = 0.0;

				stream.ReadBits((unsigned char*)&d, 32);
				property.set_value(instance, d);

			}

			template<>
			void deserialize<Vector3>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
			{
				float x, y, z;
				Vector3 v;

				stream.ReadBits((unsigned char*)&x, 32);
				stream.ReadBits((unsigned char*)&y, 32);
				stream.ReadBits((unsigned char*)&z, 32);

				v = Vector3(x, y, z);
				property.set_value(instance, v);
			}

			template<>
			void deserialize<CoordinateFrame>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
			{
				float x, y, z, r00, r01, r02, r10, r11, r12, r20, r21, r22;

				stream.ReadBits((unsigned char*)&x, 32);
				stream.ReadBits((unsigned char*)&y, 32);
				stream.ReadBits((unsigned char*)&z, 32);
				stream.ReadBits((unsigned char*)&r00, 32);
				stream.ReadBits((unsigned char*)&r01, 32);
				stream.ReadBits((unsigned char*)&r02, 32);
				stream.ReadBits((unsigned char*)&r10, 32);
				stream.ReadBits((unsigned char*)&r11, 32);
				stream.ReadBits((unsigned char*)&r12, 32);
				stream.ReadBits((unsigned char*)&r20, 32);
				stream.ReadBits((unsigned char*)&r21, 32);
				stream.ReadBits((unsigned char*)&r22, 32);

				CoordinateFrame cframe;

				cframe = CoordinateFrame(Matrix3(r00, r01, r02, r10, r11, r12, r20, r21, r22), Vector3(x, y, z));

				property.set_value(instance, cframe);
			}

			template<>
			void deserialize<Color3>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
			{
				float x, y, z;
				Color3 c;

				stream.ReadBits((unsigned char*)&x, 32);
				stream.ReadBits((unsigned char*)&y, 32);
				stream.ReadBits((unsigned char*)&z, 32);

				c = Color3(x, y, z);
				property.set_value(instance, c);
			}

			template<>
			void deserialize<int>(RBX::Instance* instance, BitStream& stream, rttr::property& property)
			{
				int value;
				stream >> value;
				property.set_value(instance, value);
			}

			Replicator(SystemAddress server, RakPeerInterface* peer, int sendRate = 5);
		};
	}
}