#include "scriptcontext.h"
#include "yieldingthreads.h"

#include "datamodel.h"
#include "workspace.h"

using namespace RBX::Reflection;

void RBX::ScriptContext::openState()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "ScriptContext::openState() opening");
	if (!globalState)
	{
		globalState = luaL_newstate();

		lua_pushcclosure(globalState, luaopen_base, 0);
		lua_pushstring(globalState, "");
		lua_pcall(globalState, 1, 0, 0);

		RBX_LUA_REGISTER(globalState, RBX::Instance);

		LuaMetatable<Instance>::pushObject(globalState, Datamodel::getDatamodel());
		lua_setglobal(globalState, "game");

		LuaMetatable<Instance>::pushObject(globalState, Workspace::singleton());
		lua_setglobal(globalState, "workspace");

		lua_register(globalState, "wait", & ScriptContext::wait);
		lua_register(globalState, "print", &ScriptContext::print);

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

int RBX::ScriptContext::print(lua_State* L)
{
	int n = lua_gettop(L);  /* number of arguments */
	int i;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char* s;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);  /* get result */
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1)
			RBX::Log::writeEntry(MESSAGE_PRINT , "\t");
		RBX::Log::writeEntry(MESSAGE_PRINT, s);
		lua_pop(L, 1);  /* pop result */
	}
	RBX::Log::writeEntry(MESSAGE_PRINT, "\n");
	return 0;
}

int RBX::ScriptContext::wait(lua_State* L)
{
	double timeout;

	timeout = lua_tonumber(L, 1);
	RBX::Datamodel::getDatamodel()->yieldingThreads->queueWaiter(L, timeout);

	return lua_yield(L, 0);
}
