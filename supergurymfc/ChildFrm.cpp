
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include <thread>

#include "framework.h"
#include "supergurymfc.h"

#include "ChildFrm.h"

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
	//RBX::AppManager* manager = RBX::AppManager::singleton();

	//if (application)
	//{
	//	suspendCurrentApplication(application);
	//	application->onFocus();
	//}
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{

	//std::thread{ createApplication, this }.detach();

	//return 1;

	return m_wndSplitter.Create(this, 50, 50, CSize(10, 10), pContext, WS_CHILD | WS_VISIBLE | SPLS_DYNAMIC_SPLIT);
}

void CChildFrame::OnKillFocus(CWnd* pNewWnd)
{
	CMDIChildWnd::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here

	//if (application) application->onKillFocus();
}


void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	// TODO: Add your message handler code here
}
