#pragma once

#include "luabridge.h"

namespace RBX
{
	namespace Lua
	{

		class CoordinateFrameBridge
		{
		public:
			static const luaL_reg classLibrary[];

			static int fromEulerAnglesXYZ(lua_State* L);
			static int fromAxisAngle(lua_State* L);

			static int newCoordinateFrame(lua_State* L);

			static int on_inverse(lua_State* L);
			static int on_vectorToWorldSpace(lua_State* L);
			static int on_vectorToObjectSpace(lua_State* L);
			static int on_pointToObjectSpace(lua_State* L);
			static int on_pointToWorldSpace(lua_State* L);
			static int on_toObjectSpace(lua_State* L);
			static int on_toWorldSpace(lua_State* L);
		};
	}
}