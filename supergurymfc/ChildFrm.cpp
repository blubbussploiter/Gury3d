
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include <thread>

#include "framework.h"
#include "supergurymfc.h"

#include "ChildFrm.h"

void suspendCurrentApplication(RBX::Experimental::Application* application)
{
	RBX::AppManager* manager = RBX::AppManager::singleton();

	if (manager->getApplication())
	{
		manager->getApplication()->suspend();
	}

	manager->setCurrentApplication(application);
	manager->getApplication()->resume();
}

void createApplication(CChildFrame* frame)
{
	RBX::AppManager* manager = RBX::AppManager::singleton();

	if (!frame->application)
	{
		RBX::Experimental::Application* app = manager->instantiate(frame->GetSafeHwnd());

		if (!manager->toLoad.empty())
		{
			app->rbxlFile = manager->toLoad;
		}

		if (!manager->fileName.empty())
		{
			frame->SetWindowTextA(manager->fileName.c_str());
		}

		frame->application = app;
		suspendCurrentApplication(frame->application);

		frame->application->start();
		manager->start();
	}
}

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame() noexcept
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{

}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

void CChildFrame::OnSetFocus(CWnd* pNewWnd)
{
	RBX::AppManager* manager = RBX::AppManager::singleton();

	if (application)
	{
		suspendCurrentApplication(application);
		application->onFocus();
	}
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{

	std::thread{ createApplication, this }.detach();

	return 1;
}

void CChildFrame::OnKillFocus(CWnd* pNewWnd)
{
	CMDIChildWnd::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here

	if (application) application->onKillFocus();
}


void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	// TODO: Add your message handler code here
}
