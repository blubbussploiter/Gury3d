
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"

#include "scene.h"
#include "workspace.h"
#include "thumbnailGenerator.h"
#include "yieldingthreads.h"
#include "controller.h"
#include "lighting.h"
#include "players.h"

#include "appmanager.h"
#include "serializer.h"
#include "stdout.h"

#include "jointsservice.h"
#include "soundservice.h"

#include "selection.h"

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
}

void RBX::Datamodel::close()
{
    RBX::StandardOut::print(MESSAGE_INFO, "DataModel::close()");
    RBX::Scene::singleton()->close();
    Kernel::get()->cleanup();
    RBX::Log::cleanup();
    emptyExplorerWindow();
}

void RBX::Datamodel::onDescendentAdded(RBX::Instance* i)
{
    if (i->parent != this)
    {
        addToExplorerWindow(i);
    }
}

void RBX::Datamodel::onDescendentRemoved(RBX::Instance* i)
{
    removeFromExplorerWindow(i);
}

void RBX::Datamodel::fillExplorerWindow()
{
    addToExplorerWindow(workspace);
    addToExplorerWindow(players);
    addToExplorerWindow(lighting);
    addToExplorerWindow(soundService);
    addToExplorerWindow(jointService);
}

void RBX::Datamodel::emptyExplorerWindow()
{
    CMainFrame::mainFrame->m_wndClassView.ClearInstances();
}

void RBX::Datamodel::addToExplorerWindow(RBX::Instance* i)
{
    CMainFrame::mainFrame->m_wndClassView.AddInstance(i);
}

void RBX::Datamodel::removeFromExplorerWindow(RBX::Instance* i)
{
    CMainFrame::mainFrame->m_wndClassView.RemoveInstance(i);
}

void RBX::Datamodel::open()
{
    workspace = new Workspace();
    runService = new RunService();
    lighting = new Lighting();
    scene = new Scene();
    controllerService = new ControllerService();
    thumbnailGenerator = new ThumbnailGenerator();
    scriptContext = new ScriptContext();
    soundService = new SoundService();
    players = new RBX::Network::Players();   
    jointService = new JointsService();
    selectionService = new Selection();
    guiRoot = Gui::singleton();
    runService->scriptContext = scriptContext;
    yieldingThreads = new Lua::YieldingThreads(scriptContext);

    fillExplorerWindow();

    /* if not server */
    // guiRoot->initFont();
}

void RBX::Datamodel::step(double deltaTime)
{
    runService->heartbeat(deltaTime);
    yieldingThreads->resume(deltaTime);
}