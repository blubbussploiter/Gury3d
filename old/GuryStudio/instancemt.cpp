
#include <iostream>
#include "instancebridge.h"

RBX_LUA_REGISTER_NAME(RBX::Instance, "Instance");

int funcProxy(lua_State* L)
{
	RBX::Instance* instance = RBX::Reflection::LuaMetatable<RBX::Instance>::getObject(L, 1);
	
	if (instance)
	{
		rttr::type base = rttr::detail::get_type_from_instance(instance);
		rttr::type type = rttr::type::get_by_name(instance->getClassName().c_str());

		if (!type)
		{
			type = base;
		}

		if (type)
		{
			const char* index = lua_tostring(L, lua_upvalueindex(1));
			rttr::method m = type.get_method(index);

			if (m)
			{

				RBX::LuaArguments args(L, m);
				rttr::variant ret = args.invoke(instance);

				if (ret.is_valid())
				{
					RBX::pushVariant(L, ret);
					return 1;
				}

			}

		}
	}

	return 0;
}

int RBX::Reflection::LuaMetatable<RBX::Instance>::on_tostring(lua_State* L)
{
	Instance* instance = RBX::Reflection::LuaMetatable<RBX::Instance>::getObject(L, 1);
	lua_pushstring(L, instance->getName().c_str());
	return 1;
}

int RBX::Reflection::LuaMetatable<RBX::Instance>::on_index(lua_State* L)
{
	Instance* instance = RBX::Reflection::LuaMetatable<RBX::Instance>::getObject(L, 1);
	const char* index = lua_tostring(L, 2);

	rttr::type base = rttr::detail::get_type_from_instance(instance);
	rttr::type type = rttr::type::get_by_name(instance->getClassName());

	if (!type)
	{
		type = base;
	}

	if (type)
	{
		rttr::property prop = type.get_property(index);

		if (prop)
		{
			pushLuaValue(L, instance, prop);
			return 1;
		}

		rttr::method method = type.get_method(index);
		if (method)
		{
			lua_pushvalue(L, 2);
			lua_pushcclosure(L, funcProxy, 1);
			return 1;
		}
	}

	Instance* child;
	child = instance->findFirstChild(index);

	if (child)
	{
		RBX::Reflection::LuaMetatable<RBX::Instance>::pushObject(L, child);
		return 1;
	}

	return luaL_error(L, "'%s' not valid member of %s", index, instance->getName().c_str());
}

int RBX::Reflection::LuaMetatable<RBX::Instance>::on_newindex(lua_State* L)
{
	Instance* instance = RBX::Reflection::LuaMetatable<RBX::Instance>::getObject(L, 1);
	const char* index = lua_tostring(L, 2);

	rttr::type base = rttr::detail::get_type_from_instance(instance);
	rttr::type type = rttr::type::get_by_name(instance->getClassName());

	if (!type)
	{
		type = base;
	}

	if (type)
	{
		rttr::property prop = type.get_property(index);
		if (prop)
		{
			assignLuaValue(L, instance, prop, 3);
			return 0;
		}
	}

	return luaL_error(L, "'%s' not valid member of %s '%s'", index, instance->getClassName().c_str(), instance->getName().c_str());
}