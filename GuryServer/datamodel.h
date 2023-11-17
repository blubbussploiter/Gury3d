#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "GuiRoot.h"

namespace RBX
{
	class Workspace;
	class RunService;
	class ControllerService;
	class ThumbnailGenerator;
	class Lighting;
	class Scene;
	class JointService;

	namespace Network { class Players; }

	class Datamodel : public RBX::Instance
	{
	public:

		Workspace* workspace;
		RunService* runService;
		JointService* jointService;
		ControllerService* controllerService;
		ThumbnailGenerator* thumbnailGenerator;
		Lighting* lighting;
		Scene* scene;

		Gui::GuiHint* message;
		Gui::GuiRoot* guiRoot;

		RBX::Network::Players* players;

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