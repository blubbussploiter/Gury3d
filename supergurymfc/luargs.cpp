#include "instancebridge.h"
#include "stdout.h"

void RBX::LuaArguments::thunk(lua_State* L, int idx, rttr::parameter_info info)
{
	if (info.get_type() == rttr::type::get<int>())
	{
		int i = lua_tonumber(L, idx);
		rttr::variant var(i);
		arguments.push_back(rttr::argument(var));
	}
	if (info.get_type() == rttr::type::get<std::string>())
	{
		std::string str = std::string(lua_tostring(L, idx));
		rttr::variant var = rttr::variant(str);
		rttr::argument arg(var);
		arguments.push_back(arg);
	}
}

inline void RBX::LuaArguments::parse(lua_State* L, int idx)
{
	rttr::array_range<rttr::parameter_info> params = method.get_parameter_infos();
	for (auto& i : params)
	{
		thunk(L, idx, i);
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