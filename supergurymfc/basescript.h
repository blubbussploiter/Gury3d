#pragma once

#include "instance.h"
#include "luabridge.h"

namespace RBX
{
	class BaseScript : 
		public RBX::Instance
	{
	private:
		std::string source;
		lua_State* scriptThread;
	public:

		void setSource(std::string newSource) { source = newSource; }
		std::string getSource() { return source; }

		lua_State* establishScriptThread(lua_State* globalState)
		{
			if (!scriptThread) scriptThread = lua_newthread(globalState);
			return scriptThread;
		}

		BaseScript()
		{
			scriptThread = 0;
			setClassName("Script");
			setName("Script");
		}

		RTTR_ENABLE(RBX::Instance);
	};
}