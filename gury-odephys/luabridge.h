
#ifndef LUABRIDGE_H
#define LUABRIDGE_H

#include <string>

extern "C"
{
#include "lapi.h"
#include "ldo.h"
#include "lauxlib.h"
#include "lstate.h"
#include "lualib.h"
}

#define RBX_LUA_REGISTER(L, c) \
{ \
	RBX::Reflection::LuaMetatable<c>::registerClass(L); \
}

#define RBX_LUA_REGISTER_NAME(c, n) \
	std::string RBX::Reflection::LuaMetatable<c>::className = n; \


namespace RBX
{
	namespace Reflection
	{
		/* For interaction between C++ classes and lua */

		template <typename Class>
		class LuaMetatable
		{
		public:

			static std::string className;

			/* Implemented by template class */

			static int on_index(lua_State* L);
			static int on_newindex(lua_State* L);
			static int on_tostring(lua_State* L);

			static Class* getObject(lua_State* L, int idx)
			{
				return (Class*)luaL_checkudata(L, idx, className.c_str());
			}

			static int pushConObject(lua_State* L, Class* c)
			{
				lua_pushlightuserdata(L, c);

				luaL_getmetatable(L, className.c_str());
				lua_setmetatable(L, -2);

				return 1;
			}

			/* Push the C++ class onto lua stack */
			static int pushObject(lua_State* L, Class* c)
			{
				luaL_getmetatable(L, className.c_str());

				if (lua_isnil(L, -1))
					return 0;

				return pushConObject(L, c);
			}

			/* Register the class onto lua stack */
			static void registerClass(lua_State* L)
			{
				luaL_newmetatable(L, className.c_str());

				lua_pushcfunction(L, on_index);
				lua_setfield(L, -2, "__index");

				lua_pushcfunction(L, on_newindex);
				lua_setfield(L, -2, "__newindex");

				lua_pushcfunction(L, on_tostring);
				lua_setfield(L, -2, "__tostring");
			}

		};


	}
}

#endif