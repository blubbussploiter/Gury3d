#include "scriptcontext.h"
#include "yieldingthreads.h"

#include "datamodel.h"
#include "workspace.h"

using namespace RBX::Reflection;

void RBX::ScriptContext::openState()
{
	if (!globalState)
	{
		globalState = luaL_newstate();

		RBX_LUA_REGISTER(globalState, RBX::Instance);

		LuaMetatable<Instance>::pushObject(globalState, Datamodel::getDatamodel());
		lua_setglobal(globalState, "game");

		LuaMetatable<Instance>::pushObject(globalState, Workspace::singleton());
		lua_setglobal(globalState, "workspace");

		lua_register(globalState, "wait", & ScriptContext::wait);

		lua_pushcclosure(globalState, luaopen_base, 0);
		lua_pushstring(globalState, "");
		lua_pcall(globalState, 1, 0, 0);

	}
}

void RBX::ScriptContext::runScript(std::string script, std::string name)
{
	if (luaL_loadbuffer(globalState, script.c_str(), script.size(), name.c_str()))
	{
		throw std::runtime_error(lua_tostring(globalState, -1));
	}

	if (lua_resume(globalState, 0))
	{
		throw std::runtime_error(lua_tostring(globalState, -1));
	}
}

int RBX::ScriptContext::resume(lua_State* L, int narg)
{
	return lua_resume(L, narg);
}

int RBX::ScriptContext::wait(lua_State* L)
{
	double timeout;

	timeout = lua_tonumber(L, 1);
	RBX::Datamodel::getDatamodel()->yieldingThreads->queueWaiter(L, timeout);

	return lua_yield(L, 0);
}
