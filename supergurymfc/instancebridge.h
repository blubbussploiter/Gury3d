#pragma once

#include "luabridge.h"
#include "instance.h"

namespace RBX
{
	class LuaArguments
	{
	private:
		rttr::method method;
		std::vector<rttr::argument> arguments;
	public:

		template <typename instance>
		rttr::variant invoke(instance s)
		{
			return method.invoke_variadic(s, arguments);
		}

		void thunk(lua_State* L, int idx, rttr::parameter_info info);
		void parse(lua_State* L, int idx);
		LuaArguments(lua_State* L, rttr::method method);

	};

	extern void pushVariant(lua_State* L, rttr::variant var);

	extern void pushLuaValue(lua_State* L, RBX::Instance* obj, rttr::property prop);
	extern void assignLuaValue(lua_State* L, RBX::Instance* obj, rttr::property prop, int idx);
}