
#include <G3D/CoordinateFrame.h>
#include <G3D/Quat.h>

#include "rbx.h"
#include "cframebridge.h"

#include "stdout.h"

RBX_LUA_REGISTER_NAME(G3D::CoordinateFrame, "CoordinateFrame");

const luaL_reg RBX::Lua::CoordinateFrameBridge::classLibrary[] =
{
	{"new", &RBX::Lua::CoordinateFrameBridge::newCoordinateFrame},
	{"fromEulerAnglesXYZ", &RBX::Lua::CoordinateFrameBridge::fromEulerAnglesXYZ},
	{"fromAxisAngle", &RBX::Lua::CoordinateFrameBridge::fromAxisAngle},
	{NULL, NULL}
};

int RBX::Lua::CoordinateFrameBridge::fromAxisAngle(lua_State* L)
{
	G3D::Vector3* object = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
	G3D::Vector3 vector3 = G3D::Vector3(object->x, object->y, object->z);
	G3D::CoordinateFrame cframe;

	float axis = luaL_checknumber(L, 2);

	cframe.rotation = G3D::Matrix3::fromAxisAngle(vector3, axis);
	RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, cframe);

	return 1;
}

int RBX::Lua::CoordinateFrameBridge::fromEulerAnglesXYZ(lua_State* L)
{
	float x, y, z;
	G3D::CoordinateFrame cframe;

	x = luaL_checknumber(L, 1);
	y = luaL_checknumber(L, 2);
	z = luaL_checknumber(L, 3);

	cframe.rotation = G3D::Matrix3::fromEulerAnglesXYZ(x, y, z);
	RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, cframe);

	return 1;
}

int RBX::Lua::CoordinateFrameBridge::newCoordinateFrame(lua_State* L)
{
	G3D::CoordinateFrame cframe;
	int narg = lua_gettop(L);

	switch (narg)
	{
	case 0: break;
	case 1:
	{
		G3D::Vector3* object = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
		cframe = G3D::Vector3(object->x, object->y, object->z);
		break;
	}
	case 2:
	{
		G3D::Vector3* object = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 1);
		G3D::Vector3* lookAt = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 2);
		cframe.translation = G3D::Vector3(object->x, object->y, object->z);
		cframe.lookAt(G3D::Vector3(lookAt->x, lookAt->y,lookAt->z));
		break;
	}
	case 3:
	{
		for (int i = 1; i <= 3; i++)
			cframe.translation[i - 1] = lua_tonumber(L, i);
		break;
	}
	case 7:
	{
		G3D::Quat quat;
		for (int i = 1; i <= 3; i++)
			cframe.translation[i - 1] = lua_tonumber(L, i);
		quat.x = lua_tonumber(L, 4);
		quat.y = lua_tonumber(L, 5);
		quat.z = lua_tonumber(L, 6);
		quat.w = lua_tonumber(L, 7);
		cframe.rotation = G3D::Matrix3(quat);
		break;
	}
	case 12:
	{
		G3D::Matrix3 matrix;
		int i;

		for (int i = 1; i <= 3; i++)
			cframe.translation[i-1] = lua_tonumber(L, i);

		i = 4;
		do
		{
			for (int r = 0; r < 3; r++)
			{
				for (int c = 0; c < 3; c++)
				{
					matrix.elt[r][c] = lua_tonumber(L, c + i);
				}
			}
			i += 3;
		} while (i < 13);

		cframe.rotation = matrix;
		break;
	}
	default:
		throw std::runtime_error(RBX::Format("invalid number of arguments: %d", narg));
	}

	RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, cframe);
	return 1;
}

int RBX::Lua::CoordinateFrameBridge::on_inverse(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, object->inverse()); 
	return 1;
}

int RBX::Lua::CoordinateFrameBridge::on_vectorToWorldSpace(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	G3D::Vector3 worldSpace;

	int result = lua_gettop(L) - 1;

	if (result == 1)
	{
		worldSpace = object->vectorToWorldSpace(G3D::Vector3::zero());
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, worldSpace);
		return 1;
	}
	else
	{
		if (result > 0)
		{
			int i;
			for (i = 2; i <= result; i++)
			{
				G3D::Vector3* vp = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, i);
				G3D::Vector3 v(vp->x, vp->y, vp->z);
				worldSpace = object->vectorToWorldSpace(v);
				RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, worldSpace);
			}
			return i - 2;
		}
	}
	return 0;
}

int RBX::Lua::CoordinateFrameBridge::on_vectorToObjectSpace(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	G3D::Vector3 objectSpace;

	int result = lua_gettop(L) - 1;

	if (result == 1)
	{
		objectSpace = object->vectorToObjectSpace(G3D::Vector3::zero());
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, objectSpace);
		return 1;
	}
	else
	{
		if (result > 0)
		{
			int i;
			for (i = 2; i <= result; i++)
			{
				G3D::Vector3* vp = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, i);
				G3D::Vector3 v(vp->x, vp->y, vp->z);
				objectSpace = object->vectorToObjectSpace(v);
				RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, objectSpace);
			}
			return i - 2;
		}
	}
	return 0;
}

int RBX::Lua::CoordinateFrameBridge::on_pointToObjectSpace(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	G3D::CoordinateFrame objectSpace;

	int result = lua_gettop(L) - 1;

	if (result == 1)
	{
		objectSpace = object->pointToObjectSpace(G3D::Vector3::zero());
		RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, objectSpace);
		return 1;
	}
	else
	{
		if (result > 0)
		{
			int i;
			for (i = 2; i <= result; i++)
			{
				G3D::Vector3* vp = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, i);
				G3D::Vector3 v(vp->x, vp->y, vp->z);
				objectSpace = object->pointToObjectSpace(v);
				RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, objectSpace);
			}
			return i - 2;
		}
	}
	return 0;
}

int RBX::Lua::CoordinateFrameBridge::on_pointToWorldSpace(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	G3D::CoordinateFrame worldSpace;

	int result = lua_gettop(L) - 1;

	if (result == 1)
	{
		worldSpace = object->pointToWorldSpace(G3D::Vector3::zero());
		RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, worldSpace);
		return 1;
	}
	else
	{
		if (result > 0)
		{
			int i;
			for (i = 2; i <= result; i++)
			{
				G3D::Vector3* vp = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, i);
				G3D::Vector3 v(vp->x, vp->y, vp->z);
				worldSpace = object->pointToWorldSpace(v);
				RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, worldSpace);
			}
			return i - 2;
		}
	}
	return 0;
}

int RBX::Lua::CoordinateFrameBridge::on_toObjectSpace(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	G3D::CoordinateFrame objectSpace;

	int result = lua_gettop(L) - 1;

	if (result - 1 == 1)
	{
		RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, object->inverse());
		return 1;
	}
	else
	{
		if (result > 0)
		{
			int i;
			for (i = 2; i <= result; i++)
			{
				G3D::CoordinateFrame* c = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, i);
				G3D::CoordinateFrame cf(c->rotation, c->translation);
				objectSpace = object->toObjectSpace(cf);
				RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, objectSpace);
			}
			return i - 2;
		}
	}

	return 0;
}

int RBX::Lua::CoordinateFrameBridge::on_toWorldSpace(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	G3D::CoordinateFrame worldSpace;

	int result = lua_gettop(L) - 1;

	if (result > 0)
	{
		int i;
		for (i = 2; i <= result; i++)
		{
			G3D::CoordinateFrame* c = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, i);
			G3D::Matrix3 rotation; G3D::Vector3 position;
			rotation = object->rotation * c->rotation;
			position = object->translation * c->translation;
			worldSpace = G3D::CoordinateFrame(rotation, position);
			RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, worldSpace);
		}
		return i-2;
	}
	else
	{
		worldSpace = G3D::CoordinateFrame(object->rotation, object->translation);
		RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, worldSpace);
		return 1;
	}

	return 0;
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_tostring(G3D::CoordinateFrame* object, lua_State* L)
{
	G3D::Vector3 origin = object->translation;
	G3D::Matrix3 matrix = object->rotation;
	lua_pushfstring(L, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", origin.x, origin.y, origin.z,
		matrix[0][0], matrix[0][1], matrix[0][2],
		matrix[1][0], matrix[1][1], matrix[1][2],
		matrix[2][0], matrix[2][1], matrix[2][2]);
	return 1;
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_index(G3D::CoordinateFrame* object, const char* name, lua_State* L)
{
	if (!strcmp(name, "p"))
	{
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, object->translation);
		return 1;
	}

	if (!strcmp(name, "lookVector"))
	{
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, object->lookVector());
		return 1;
	}

	if (!strcmp(name, "toWorldSpace"))
	{
		lua_pushcfunction(L, CoordinateFrameBridge::on_toWorldSpace);
		return 1;
	}

	if (!strcmp(name, "toObjectSpace"))
	{
		lua_pushcfunction(L, CoordinateFrameBridge::on_toObjectSpace);
		return 1;
	}

	if (!strcmp(name, "pointToWorldSpace"))
	{
		lua_pushcfunction(L, CoordinateFrameBridge::on_pointToWorldSpace);
		return 1;
	}

	if (!strcmp(name, "pointToObjectSpace"))
	{
		lua_pushcfunction(L, CoordinateFrameBridge::on_pointToObjectSpace);
		return 1;
	}

	if (!strcmp(name, "vectorToWorldSpace"))
	{
		lua_pushcfunction(L, CoordinateFrameBridge::on_vectorToWorldSpace);
		return 1;
	}

	if (!strcmp(name, "vectorToObjectSpace"))
	{
		lua_pushcfunction(L, CoordinateFrameBridge::on_vectorToObjectSpace);
		return 1;
	}

	if (!strcmp(name, "x"))
	{
		lua_pushnumber(L, object->translation.x);
		return 1;
	}

	if (!strcmp(name, "y"))
	{
		lua_pushnumber(L, object->translation.y);
		return 1;
	}

	if (!strcmp(name, "z"))
	{
		lua_pushnumber(L, object->translation.z);
		return 1;
	}



	return 0;
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_newindex(G3D::CoordinateFrame* object, const char* name, lua_State* L)
{
	throw std::runtime_error(RBX::Format("%s cannot be assigned to", name));
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_mul(lua_State* L)
{
	G3D::CoordinateFrame* object = RBX::Lua::Bridge<G3D::CoordinateFrame>::getObject(L, 1);
	G3D::CoordinateFrame* object2 = RBX::Lua::Bridge<G3D::CoordinateFrame>::getValue(L, 2);

	if (object2)
	{
		RBX::Lua::Bridge<G3D::CoordinateFrame>::pushNewObject(L, G3D::CoordinateFrame(object->rotation * object2->rotation, object->pointToWorldSpace(object2->translation)));
		return 1;
	}
	else
	{
		G3D::Vector3* vector = RBX::Lua::Bridge<G3D::Vector3>::getObject(L, 2);
		RBX::Lua::Bridge<G3D::Vector3>::pushNewObject(L, object->pointToWorldSpace(G3D::Vector3(vector->x, vector->y, vector->z)));
		return 1;
	}

	return 0;
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_add(lua_State* L)
{
	return 0;
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_sub(lua_State* L)
{
	return 0;
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_div(lua_State* L)
{
	return 0;
}

int RBX::Lua::Bridge<G3D::CoordinateFrame>::on_unm(lua_State* L)
{
	return 0;
}