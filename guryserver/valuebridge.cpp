#include <iostream>
#include <G3D/Vector3.h>
#include <G3D/CoordinateFrame.h>

#include "instancebridge.h"

void RBX::pushVariant(lua_State* L, rttr::variant var)
{

	rttr::type type = var.get_type();

	if (type == rttr::type::get<std::string>())
	{
		lua_pushstring(L, var.to_string().c_str());
		return;
	}

	if (type == rttr::type::get<float>())
	{
		lua_pushnumber(L, var.to_float());
		return;
	}

	if (type == rttr::type::get<double>())
	{
		lua_pushnumber(L, var.to_double());
		return;
	}

	if (type == rttr::type::get<bool>())
	{
		lua_pushboolean(L, var.to_bool());
		return;
	}

	if (var.can_convert<RBX::Instance*>())
	{
		RBX::Instance* instance = var.convert<RBX::Instance*>();
		RBX::Lua::SharedPtrBridge<RBX::Instance>::push(L, instance);
		return;
	}

	if (var.can_convert<G3D::Vector3>())
	{
		G3D::Vector3 vector3 = var.convert<G3D::Vector3>(); 
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, vector3);
		return;
	}

	if (var.can_convert<G3D::CoordinateFrame>())
	{
		G3D::CoordinateFrame cframe = var.convert<G3D::CoordinateFrame>();
		RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, cframe);
		return;
	}

	if (type == rttr::type::get<RBX::Instances*>())
	{
		RBX::Instances* objects = var.convert<RBX::Instances*>();
		if (objects)
		{
			RBX::Instances::const_iterator iter = objects->begin();
			RBX::Instances::const_iterator end = objects->end();

			lua_createtable(L, end - iter, 0);
			for (int i = 0; iter != end; ++iter, ++i)
			{
				RBX::Lua::SharedPtrBridge<RBX::Instance>::push(L, *iter);
				lua_rawseti(L, -2, i);
			}

		}
		else
		{
			lua_createtable(L, 0, 0);
		}

		return;
	}

	/* special-wecial, enums! */

	if (type.is_enumeration())
	{
		int integer = var.convert<int>();
		lua_pushinteger(L, integer);
		return;
	}

	lua_pushnil(L);
}

void assignVariant(lua_State* L, RBX::Instance* instance, rttr::property& prop, rttr::variant var, int idx)
{
	rttr::type type = var.get_type();

	if (type == rttr::type::get<std::string>())
	{
		prop.set_value(instance, std::string(lua_tostring(L, idx)));
	}

	if (type == rttr::type::get<int>())
	{
		prop.set_value(instance, lua_tointeger(L, idx));
	}

	if (type == rttr::type::get<double>() || type == rttr::type::get<float>())
	{
		prop.set_value(instance, lua_tonumber(L, idx));
	}

	if (type == rttr::type::get<bool>())
	{
		prop.set_value(instance, (bool)lua_toboolean(L, idx));
	}

	if (type == rttr::type::get<RBX::Instance*>())
	{
		RBX::Instance* _instance = RBX::Lua::SharedPtrBridge<RBX::Instance>::getPtr(L, idx);
		if (_instance)
		{
			prop.set_value(instance, _instance);
		}
	}

	if (type == rttr::type::get<G3D::Vector3>())
	{
		G3D::Vector3* vector3 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, idx);
		prop.set_value(instance, G3D::Vector3(vector3->x, vector3->y, vector3->z));
	}

	if (type == rttr::type::get<G3D::CoordinateFrame>())
	{
		G3D::CoordinateFrame* cframe = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, idx);
		prop.set_value(instance, G3D::CoordinateFrame(cframe->rotation, cframe->translation));
	}

	else
	{
		prop.set_value(instance, nullptr);
	}


	if (type.is_enumeration())
	{
		rttr::enumeration enumeration = type.get_enumeration();
		const rttr::type enumType = enumeration.get_type();

		rttr::variant value;

		switch (lua_type(L, idx))
		{
		case LUA_TNUMBER: value = lua_tointeger(L, idx);
		case LUA_TSTRING:
		{
			value = enumeration.name_to_value(lua_tostring(L, idx));
			break;
		}
		default: return;
		}

		if (value.convert(enumType))
		{
			prop.set_value(instance, value);
		}
	}
}

void RBX::pushLuaValue(lua_State* L, RBX::Instance* instance, rttr::property prop)
{
	rttr::variant var = prop.get_value(instance);
	pushVariant(L, var);
}

void RBX::assignLuaValue(lua_State* L, RBX::Instance* instance, rttr::property& prop, int idx)
{
	rttr::variant var = prop.get_value(instance);
	assignVariant(L, instance, prop, var, idx);
}