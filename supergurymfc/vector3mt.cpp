
#include <G3D/Vector3.h>

#include "rbx.h"
#include "vector3bridge.h"

RBX_LUA_REGISTER_NAME(G3D::Vector3, "Vector3");

const luaL_reg RBX::Lua::Vector3Bridge::classLibrary[] =
{
	{"new", &RBX::Lua::Vector3Bridge::newVector3},
	{NULL, NULL}
};

int RBX::Lua::Vector3Bridge::newVector3(lua_State* L)
{
	float x = luaL_checknumber(L, 1), y = luaL_checknumber(L, 2), z = luaL_checknumber(L, 3);
	RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, G3D::Vector3(x, y, z));
	return 1;
}

int RBX::Lua::Vector3Bridge::lerp(lua_State* L)
{
	G3D::Vector3* vector1 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
	G3D::Vector3* vector2 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 2);
	G3D::Vector3 lerpd;

	float alpha;

	alpha = luaL_checknumber(L, 3);
	lerpd = vector1->lerp(*vector2, alpha);

	RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, lerpd);
	return 1;
}

int RBX::Lua::Bridge<G3D::Vector3>::on_tostring(G3D::Vector3* object, lua_State* L)
{
	lua_pushfstring(L, "%f, %f, %f", object->x,object->y,object->z);
	return 1;
}


int RBX::Lua::Bridge<G3D::Vector3>::on_newindex(G3D::Vector3* object, const char* name, lua_State* L)
{
	throw std::runtime_error(RBX::Format("%s cannot be assigned to", name));
}

int RBX::Lua::Bridge<G3D::Vector3>::on_index(G3D::Vector3* object, const char* name, lua_State* L)
{

	if (!strcmp(name, "x"))
	{
		lua_pushnumber(L, object->x);
		return 1;
	}
	if (!strcmp(name, "y"))
	{
		lua_pushnumber(L, object->y);
		return 1;
	}
	if (!strcmp(name, "z"))
	{
		lua_pushnumber(L, object->z);
		return 1;
	}
	if (!strcmp(name, "unit"))
	{
		G3D::Vector3 unit = object->unit();
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, unit);
		return 1;
	}
	if (!strcmp(name, "magnitude"))
	{
		lua_pushnumber(L, object->magnitude());
		return 1;
	}
	if (!strcmp(name, "lerp"))
	{
		lua_pushcfunction(L, RBX::Lua::Vector3Bridge::lerp);
		return 1;
	}

	throw std::runtime_error(RBX::Format("%s is not a valid member", name));
}

/* vector3 math stuff */
/* just repeat functions, MY CODE IS GOOD I SWEAR :sob::sob::sob: */

int RBX::Lua::Bridge<G3D::Vector3>::on_div(lua_State* L)
{
	G3D::Vector3* v1, * v2;
	v1 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
	v2 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 2);

	if (v1 && v2)
	{

		G3D::Vector3 v3;
		v3.x = v1->x / v2->x;
		v3.y = v1->y / v2->y;
		v3.z = v1->z / v2->z;
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, v3);
		return 1;
	}

	return 0;
}

int RBX::Lua::Bridge<G3D::Vector3>::on_mul(lua_State* L)
{
	G3D::Vector3* v1, * v2;
	v1 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
	v2 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 2);

	if (v1 && v2)
	{

		G3D::Vector3 v3;
		v3.x = v1->x * v2->x;
		v3.y = v1->y * v2->y;
		v3.z = v1->z * v2->z;
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, v3);
		return 1;
	}

	return 0;
}

int RBX::Lua::Bridge<G3D::Vector3>::on_sub(lua_State* L)
{
	G3D::Vector3* v1, * v2;
	v1 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
	v2 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 2);

	if (v1 && v2)
	{

		G3D::Vector3 v3;
		v3.x = v1->x - v2->x;
		v3.y = v1->y - v2->y;
		v3.z = v1->z - v2->z;
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, v3);
		return 1;
	}

	return 0;
}

int RBX::Lua::Bridge<G3D::Vector3>::on_add(lua_State* L)
{
	G3D::Vector3* v1, * v2;
	v1 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
	v2 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 2);

	if (v1 && v2)
	{

		G3D::Vector3 v3;
		v3.x = v1->x + v2->x;
		v3.y = v1->y + v2->y;
		v3.z = v1->z + v2->z;
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, v3);
		return 1;
	}

	return 0;
}

/* this isnt a repeat though :) */

int RBX::Lua::Bridge<G3D::Vector3>::on_unm(lua_State* L)
{
	G3D::Vector3* v1;
	v1 = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);

	if (v1)
	{

		G3D::Vector3 v3;
		v3.x = -v1->x;
		v3.y = -v1->y;
		v3.z = -v1->z;
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, v3);
		return 1;
	}

	return 0;
}