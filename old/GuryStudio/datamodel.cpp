
#include "scene.h"
#include "workspace.h"
#include "jointservice.h"
#include "thumbnailGenerator.h"
#include "yieldingthreads.h"
#include "controller.h"
#include "lighting.h"
#include "players.h"

#include "appmanager.h"
#include "serializer.h"
#include "stdout.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<RBX::Datamodel>("Datamodel")
         .constructor<>()
         .method("SetMessageBrickCount", &RBX::Datamodel::setMessageBrickCount)
         .method("load", &RBX::Datamodel::loadContent);
}

RBX::Datamodel* RBX::Datamodel::getDatamodel()
{
    RBX::Experimental::Application* application = RBX::AppManager::singleton()->getApplication();
    if (application)
    {
        return application->getDatamodel();
    }
    return 0;
}

void RBX::Datamodel::loadContent(std::string contentId)
{
    RBX::StandardOut::print(MESSAGE_INFO, "DataModel loading %s", contentId.c_str());
    RBX::Serializer::load(contentId);
    //runService->reset();
}

void RBX::Datamodel::close()
{
    RBX::StandardOut::print(MESSAGE_INFO, "DataModel::close()");
    RBX::RunService::singleton()->getPhysics()->close();
}

void RBX::Datamodel::open()
{
    workspace = new Workspace();
    runService = new RunService();
    jointService = new JointService();
    lighting = new Lighting();
    scene = new Scene();
    controllerService = new ControllerService();
    thumbnailGenerator = new ThumbnailGenerator();
    scriptContext = new ScriptContext();
    yieldingThreads = new Lua::YieldingThreads(scriptContext);
    players = new RBX::Network::Players();
    guiRoot = Gui::singleton();

    workspace->setParent(this);
    runService->setParent(this);
    jointService->setParent(this);
    lighting->setParent(this);
    controllerService->setParent(this);
    thumbnailGenerator->setParent(this);
    players->setParent(this);
    /* if not server */
    // guiRoot->initFont();
}

void RBX::Datamodel::step()
{
    if (runService->isRunning)
    {
        runService->heartbeat();
        jointService->update();
    }

}

void RBX::Datamodel::heartbeat(double step)
{
    yieldingThreads->resume(step);
}
