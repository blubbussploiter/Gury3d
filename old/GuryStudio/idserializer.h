#pragma once

#include "instance.h"
#include "slikenet/BitStream.h"

using namespace SLNet;

namespace RBX
{
	namespace Network
	{
		class IdSerializer : public RBX::Instance
		{
		public:

			class NetworkId
			{
			public:
				std::string guid;
				std::string className;
			};

			static void serializeId(BitStream& stream, RBX::Instance* instance);
			static NetworkId deserializeId(BitStream& stream);
		};
	}
}