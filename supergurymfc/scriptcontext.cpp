#include "scriptcontext.h"
#include "yieldingthreads.h"

#include "datamodel.h"
#include "workspace.h"

#include "luabridge.h"
#include "vector3bridge.h"
#include "cframebridge.h"

#include "classes.h"

#include "runservice.h"

int instanceNew(lua_State* L)
{
	const char* instance = luaL_checkstring(L, 1);
	if (!instance) return 0;

	RBX::Instance* newInstance = RBX::fromName(instance);

	if (!lua_isnil(L, 2))
	{
		newInstance->setParent(RBX::Lua::SharedPtrBridge<RBX::Instance>::getPtr(L, 2));
	}

	RBX::Lua::SharedPtrBridge<RBX::Instance>::push(L, newInstance);
	return 1;
}

void RBX::ScriptContext::openState()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "ScriptContext::openState() opening");
	if (!globalState)
	{
		globalState = luaL_newstate();

		luaopen_base(globalState);
		luaopen_math(globalState);
		luaopen_string(globalState);

		luaL_register(globalState, "Vector3", RBX::Lua::Vector3Bridge::classLibrary);
		luaL_register(globalState, "CFrame", RBX::Lua::CoordinateFrameBridge::classLibrary);

		RBX_LUA_REGISTER(globalState, G3D::Vector3);
		RBX_LUA_REGISTER(globalState, G3D::CoordinateFrame);
		RBX_PTR_LUA_REGISTER(globalState, RBX::Instance);

		RBX::Lua::SharedPtrBridge<RBX::Instance>::push(globalState, Datamodel::getDatamodel());
		lua_setglobal(globalState, "game");

		RBX::Lua::SharedPtrBridge<RBX::Instance>::push(globalState, Workspace::singleton());
		lua_setglobal(globalState, "workspace");

		lua_register(globalState, "wait", &ScriptContext::wait);
		lua_register(globalState, "print", &ScriptContext::print);

		lua_newtable(globalState);
		lua_pushcfunction(globalState, instanceNew);
		lua_setfield(globalState, -2, "new");
		lua_setglobal(globalState, "Instance");

	}
}

void RBX::ScriptContext::executeInNewThread(std::string script)
{
	lua_State* thread = lua_newthread(globalState);
	if (!thread)
	{
		throw std::runtime_error("ScriptContext::execute: Unable to create a new thread");
	}

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "ScriptContext::executeInNewThread %.64s", script.c_str());
	if (luaL_loadbuffer(thread, script.c_str(), script.size(), "="))
	{
		throw std::runtime_error(lua_tostring(thread, -1));
	}
	else
	{
		resume(thread, 0);
	}
}

void RBX::ScriptContext::execute(std::string script)
{
	try
	{
		executeInNewThread(script);
	}
	catch (std::runtime_error e)
	{
		RBX::StandardOut::print(RBX::MESSAGE_ERROR, e.what());
	}

}

int RBX::ScriptContext::resume(lua_State* L, int narg)
{
	int status;
	status = lua_resume(L, narg);

	if (status == LUA_YIELD)
	{
		return 1;
	}

	if (status)
	{
		throw std::runtime_error(lua_tostring(L, -1));
	}

	return 0;
}

void RBX::ScriptContext::onWorkspaceDescendentAdded(RBX::Instance* descendent)
{
	RBX::BaseScript* script = dynamic_cast<RBX::BaseScript*>(descendent);
	if (script)
	{
		scripts.push_back(script);
	}
}

void RBX::ScriptContext::onWorkspaceDescendentRemoved(RBX::Instance* descendent)
{
	if (std::find(
		scripts.begin(),
		scripts.end(),
		descendent) != scripts.end())
	{
		scripts.erase(std::remove(
			scripts.begin(),
			scripts.end(),
			descendent));
	}
}

void RBX::ScriptContext::runScript(RBX::BaseScript* script)
{
	lua_State* thread;
	std::string source;

	if (std::find(scripts.begin(), scripts.end(), script) == scripts.end()) return;
	thread = script->establishScriptThread(globalState);

	if (!thread)
	{
		throw std::exception("ScriptContext::runScript: Unable to create a new thread");
	}

	RBX::Lua::SharedPtrBridge<RBX::Instance>::push(thread, script);
	lua_setglobal(thread, "script");

	source = script->getSource();

	if (luaL_loadbuffer(thread, source.c_str(), source.size(), script->getName().c_str()))
	{
		throw std::runtime_error(lua_tostring(thread, -1));
	}
	else
	{
		resume(thread, 0);
	}
}

void RBX::ScriptContext::runScripts()
{
	for (unsigned int i = 0; i < scripts.size(); i++)
	{
		RBX::BaseScript* script = scripts.at(i);
		if (!script) continue;
		try
		{
			runScript(script);
		}
		catch (std::runtime_error err)
		{
			RBX::StandardOut::print(RBX::MESSAGE_ERROR, err.what());
		}
	}
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

RBX::ScriptContext* RBX::ScriptContext::singleton()
{
	RBX::RunService* runService = RBX::RunService::singleton();
	if (runService->scriptContext) return runService->scriptContext;
	return 0;
}