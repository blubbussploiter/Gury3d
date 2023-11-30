
#include <iostream>
#include "instancebridge.h"

RBX_PTR_LUA_REGISTER_NAME(RBX::Instance, "Instance");

int funcProxy(lua_State* L)
{
	RBX::Instance* instance = RBX::Lua::SharedPtrBridge<RBX::Instance>::getPtr(L, 1);
	
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

int RBX::Lua::SharedPtrBridge<RBX::Instance>::on_tostring(RBX::Instance* object, lua_State* L)
{
	lua_pushstring(L, object->getName().c_str());
	return 1;
}

int RBX::Lua::SharedPtrBridge<RBX::Instance>::on_index(RBX::Instance* object, const char* name, lua_State* L)
{

	rttr::type base = rttr::detail::get_type_from_instance(object);
	rttr::type type = rttr::type::get_by_name(object->getClassName());

	if (!type)
	{
		type = base;
	}

	if (type)
	{
		rttr::property prop = type.get_property(name);

		if (prop)
		{
			pushLuaValue(L, object, prop);
			return 1;
		}

		rttr::method method = type.get_method(name);
		if (method)
		{
			lua_pushvalue(L, 2);
			lua_pushcclosure(L, funcProxy, 1);
			return 1;
		}
	}

	Instance* child;
	child = object->findFirstChild(name);

	if (child)
	{
		RBX::Lua::SharedPtrBridge<RBX::Instance>::push(L, child);
		return 1;
	}

	return luaL_error(L, "'%s' not valid member of %s", name, object->getName().c_str());
}

int RBX::Lua::SharedPtrBridge<RBX::Instance>::on_newindex(RBX::Instance* object, const char* name, lua_State* L)
{
	rttr::type base = rttr::detail::get_type_from_instance(object);
	rttr::type type = rttr::type::get_by_name(object->getClassName());

	if (!type)
	{
		type = base;
	}

	if (type)
	{
		rttr::property prop = type.get_property(name);
		if (prop)
		{
			assignLuaValue(L, object, prop, 3);
			return 0;
		}
	}

	return luaL_error(L, "'%s' not valid member of %s '%s'", name, object->getClassName().c_str(), object->getName().c_str());
}