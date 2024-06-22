#pragma once

#include "GuiRoot.h"
#include "kernel.h"

namespace RBX
{
	class Workspace;
	class RunService;
	class ControllerService;
	class ThumbnailGenerator;
	class ScriptContext;
	class Lighting;
	class Scene;
	class SoundService;
	class Selection;
	class JointsService;

	namespace Network { class Players; }
	namespace Lua { class YieldingThreads; }

	class Datamodel : 
		public RBX::Instance
	{
	public:

		Workspace* workspace;
		RunService* runService;
		JointsService* jointService;
		ControllerService* controllerService;
		ThumbnailGenerator* thumbnailGenerator;
		ScriptContext* scriptContext;
		SoundService* soundService;
		Selection* selectionService;
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

		void onDescendentAdded(RBX::Instance* i);
		void onDescendentRemoved(RBX::Instance* i);

		bool messageBrickCount() { return uiBrickcount; }

		void fillExplorerWindow();
		void emptyExplorerWindow();

		void addToExplorerWindow(RBX::Instance* i);
		void removeFromExplorerWindow(RBX::Instance* i);

		void open();
		void close();

		void step(double deltaTime);

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