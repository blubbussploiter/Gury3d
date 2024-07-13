#pragma once

#include <vector>
#include "scriptcontext.h"

namespace RBX
{
	namespace Lua
	{

		class YieldingThreads
		{
			class WaitingThread
			{
			public:
				lua_State* thread;

				double requestedDelay;
				double elapsedTime;

				WaitingThread(lua_State* thread, double time) : thread(thread), requestedDelay(time) {}
			};
		private:
			std::vector<WaitingThread*> waitingThreads;
			RBX::ScriptContext* context;
		public:

			void queueWaiter(lua_State* L, double time)
			{
				waitingThreads.push_back(new WaitingThread(L, time));
			}

			void resume(double step) /* called per simulation? */
			{
				for (WaitingThread* t : waitingThreads)
				{
					if (t->thread)
					{
						t->elapsedTime += step;
						if (t->requestedDelay <= t->elapsedTime)
						{
							int oldTop = lua_gettop(t->thread);
							lua_pushnumber(t->thread, t->elapsedTime);
							context->resume(t->thread, 0);
							lua_settop(t->thread, oldTop);
							waitingThreads.erase(std::remove(waitingThreads.begin(), waitingThreads.end(), t));
						}
					}
				}
			}

			YieldingThreads(RBX::ScriptContext* context) : context(context) {}
		};
	}
}