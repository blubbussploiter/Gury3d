#pragma once

#include <string>
#include "slikenet/BitStream.h"

using namespace SLNet;

namespace RBX
{
	namespace Network
	{
		namespace Help
		{
			static const char* read(BitStream& stream, int len)
			{
				char* c = new char[len+1];
				stream.Read(c);
				c[len] = '\0';
				return c;
			}
		}
	}
}