#pragma once

#include "luabridge.h"

namespace RBX
{
	namespace Lua
	{
		class Vector3Bridge
		{
		public:
			static const luaL_reg classLibrary[];
			static int newVector3(lua_State* L);
			static int lerp(lua_State* L);
		};

	}
}