#pragma once

namespace RBX
{
	enum MessageIdentifiers
	{
		ID_SEND_GLOBALS = 0x4b,
		ID_SEND_INSTANCE = 0x8f,
		ID_SEND_PROPERTY = 0x8c,
		ID_SEND_REMOTEPLAYER = 0x86,
		ID_BRICK_COUNT = 0x8b,
	};

	enum PhysicsMessageIdentifiers
	{
		ID_SEND_PHYSICS = 0x8e
	};
}