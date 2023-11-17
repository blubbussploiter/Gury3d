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

		void executeInNewThread(std::string script);
		void execute(std::string script); /* try: except wrapper for executeInNewThread */

		int resume(lua_State* L, int narg);

		/* custom funcs */

		static int wait(lua_State* L);
		static int print(lua_State* L);

		ScriptContext()
		{
			setName("ScriptContext");
			setClassName("ScriptContext");
		}
		RTTR_ENABLE(RBX::Instance);
	};
}