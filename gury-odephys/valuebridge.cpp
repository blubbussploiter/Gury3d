#include <iostream>
#include "instancebridge.h"

void RBX::pushVariant(lua_State* L, rttr::variant var)
{

	if (var.get_type() == rttr::type::get<std::string>())
	{
		lua_pushstring(L, var.to_string().c_str());
		return;
	}

	if (var.get_type() == rttr::type::get<float>())
	{
		lua_pushnumber(L, var.to_float());
		return;
	}

	if (var.get_type() == rttr::type::get<double>())
	{
		lua_pushnumber(L, var.to_double());
		return;
	}

	if (var.get_type() == rttr::type::get<bool>())
	{
		lua_pushboolean(L, var.to_bool());
		return;
	}

	if (var.can_convert<RBX::Instance*>())
	{
		RBX::Instance* instance = var.convert<RBX::Instance*>();
		RBX::Reflection::LuaMetatable<RBX::Instance>::pushObject(L, instance);
		return;
	}

	if (var.get_type() == rttr::type::get<RBX::Instances*>())
	{
		RBX::Instances* objects = var.convert<RBX::Instances*>();
		if (objects)
		{
			RBX::Instances::const_iterator iter = objects->begin();
			RBX::Instances::const_iterator end = objects->end();

			lua_createtable(L, end - iter, 0);
			for (int i = 0; iter != end; ++iter, ++i)
			{
				RBX::Reflection::LuaMetatable<RBX::Instance>::pushObject(L, *iter);
				lua_rawseti(L, -2, i);
			}

		}
		else
		{
			lua_createtable(L, 0, 0);
		}

		return;
	}

	lua_pushnil(L);
}

void RBX::pushLuaValue(lua_State* L, RBX::Instance* instance, rttr::property prop)
{
	rttr::variant var = prop.get_value(instance);
	pushVariant(L, var);
}

void RBX::assignLuaValue(lua_State* L, RBX::Instance* instance, rttr::property prop, int idx)
{
	switch (lua_type(L, idx))
	{
		case LUA_TNIL:
		{
			prop.set_value(instance, nullptr);
			break;
		}
		case LUA_TSTRING:
		{
			prop.set_value(instance, lua_tostring(L, idx));
			break;
		}
		case LUA_TNUMBER:
		{
			prop.set_value(instance, lua_tonumber(L, idx));
			break;
		}
		case LUA_TBOOLEAN:
		{
			printf("bool, %d\n",
				prop.set_value(instance, lua_toboolean(L, idx)));
			printf("%d\n", prop.get_value(instance).to_bool());
			break;
		}
		case LUA_TUSERDATA:
		{
			Instance* _instance = static_cast<RBX::Instance*>(luaL_checkudata(L, idx, "Instance"));
			if (_instance)
			{
				prop.set_value(instance, _instance);
			}
			break;
		}
	}
}