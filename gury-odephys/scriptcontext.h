#pragma once

#include "runservice.h"
#include "instancebridge.h"

namespace RBX
{
	class ScriptContext : public RBX::Instance
	{
	private:
		lua_State* globalState;
	public:

		void openState();
		void runScript(std::string script, std::string name);

		int resume(lua_State* L, int narg);

		/* custom funcs */

		static int wait(lua_State* L);

		ScriptContext()
		{
			setName("ScriptContext");
			setClassName("ScriptContext");
		}
		RTTR_ENABLE(RBX::Instance);
	};
}