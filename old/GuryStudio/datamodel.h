#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "GuiRoot.h"

namespace RBX
{
	class Workspace;
	class RunService;
	class ControllerService;
	class ThumbnailGenerator;
	class ScriptContext;
	class Lighting;
	class Scene;
	class JointService;

	namespace Network { class Players; }
	namespace Lua { class YieldingThreads; }

	class Datamodel : 
		public RBX::Instance
	{
	public:

		Workspace* workspace;
		RunService* runService;
		JointService* jointService;
		ControllerService* controllerService;
		ThumbnailGenerator* thumbnailGenerator;
		ScriptContext* scriptContext;
		Lighting* lighting;
		Scene* scene;

		Gui::GuiHint* message;
		Gui::GuiRoot* guiRoot;

		RBX::Network::Players* players;
		Lua::YieldingThreads* yieldingThreads;

		bool uiBrickcount;
	public:

		static Datamodel* getDatamodel();
		void loadContent(std::string contentId);

		void setMessage(std::string msg) { message->text = msg; }
		void setMessageBrickCount() { uiBrickcount = !uiBrickcount; }

		bool messageBrickCount() { return uiBrickcount; }

		void open();
		void close();

		void step();
		void heartbeat(double step); /* non runservice stuff */

		Datamodel()
		{
			setClassName("Datamodel");
			setName("<<<ROOT>>>");
			message = new Gui::GuiHint();
			isParentLocked = 1;
		}

		RTTR_ENABLE(RBX::Instance)
	};
}

#endif