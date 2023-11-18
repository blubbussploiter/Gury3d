#pragma once

#include <vector>

#include "runservice.h"
#include "instancebridge.h"

#include "basescript.h"

namespace RBX
{
	class ScriptContext : public RBX::Instance
	{
	private:
		std::vector<BaseScript*> scripts;
		lua_State* globalState;
		bool scriptsDisabled;
	public:

		void openState();

		void executeInNewThread(std::string script);
		void execute(std::string script); /* try: except wrapper for executeInNewThread */

		int resume(lua_State* L, int narg);

		void onWorkspaceDescendentAdded(RBX::Instance* descendent);
		void onWorkspaceDescendentRemoved(RBX::Instance* descendent);

		void runScripts();
		void runScript(RBX::BaseScript* script);

		static ScriptContext* singleton();

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