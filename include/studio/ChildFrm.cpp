
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include <thread>

#include "framework.h"
#include "3DStudio.h"

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

		if (manager)
		{
			frame->application = app;
			suspendCurrentApplication(frame->application);

			if (frame->application)
			{
				frame->application->start();
			}

			manager->start();
		}
	}
}

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_SETFOCUS()
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

	cs.style &= ~WS_CAPTION;
	cs.style &= ~WS_VSCROLL;
	cs.style &= ~WS_BORDER;

	cs.style |= WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CChildFrame::OnSetFocus(CWnd* pNewWnd)
{
	RBX::AppManager* manager = RBX::AppManager::singleton();

	if (application)
	{
		suspendCurrentApplication(application);
	}
	else
	{
		
		std::thread{ createApplication, this }.detach();
	}
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	MDIMaximize();
	return 1;
}