#include "instancebridge.h"

inline void RBX::LuaArguments::parse(lua_State* L, int idx)
{
	rttr::array_range<rttr::parameter_info> params = method.get_parameter_infos();

	for (auto& info : params)
	{
		if (info.get_type() == rttr::type::get<int>())
		{
			int i = static_cast<int>(lua_tonumber(L, idx));
			arguments.push_back(rttr::argument(i));
		}
		if (info.get_type() == rttr::type::get<std::string>())
		{
			const char* s = lua_tostring(L, idx);
			
		}
	}
}

RBX::LuaArguments::LuaArguments(lua_State* L, rttr::method method) : method(method)
{
	int args = lua_gettop(L);
	for (int i = 2; i <= args; i++)
	{
		parse(L, i);
	}
}