
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include <thread>

#include "resource.h"

#include "framework.h"
#include "supergurymfc.h"

#include "ChildFrm.h"

HCURSOR RBX::Experimental::guryCursor = 0;

void suspendCurrentApplication(RBX::Experimental::Application* application)
{
	RBX::AppManager* manager = RBX::AppManager::get();

	if (manager->getApplication())
	{
		manager->getApplication()->suspend();
	}

	manager->setCurrentApplication(application);
	manager->getApplication()->resume();
}

void createApplication(CChildFrame* frame)
{
	RBX::AppManager* manager = RBX::AppManager::get();

	if (!frame->application)
	{
		RBX::Experimental::Application* app = manager->instantiate(frame->GetSafeHwnd());

		if (!manager->toLoad.empty())
		{
			app->rbxlFile = manager->toLoad;
		}

		frame->application = app;
		suspendCurrentApplication(frame->application);

		frame->application->start();
	}
}

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)
BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
//	ON_WM_PAINT()
//	ON_WM_CREATE()
//	ON_WM_TIMER()
//	ON_WM_PAINT()
ON_WM_PAINT()
ON_WM_TIMER()
ON_WM_CREATE()
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame() noexcept
{
	// TODO: add member initialization code here
	isChildFrame = 1;
}

CChildFrame::~CChildFrame()
{

}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	std::string cursor = GetFileInPath("\\content\\font\\gury.cur");
	RBX::Experimental::guryCursor = LoadCursorFromFile(cursor.c_str());
	cs.lpszClass = AfxRegisterWndClass(
		CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, // use any window styles
		RBX::Experimental::guryCursor,
		(HBRUSH)(COLOR_WINDOW + 1)); // background brush

	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

void CChildFrame::OnSetFocus(CWnd* pNewWnd)
{
	CMDIChildWnd::OnSetFocus(pNewWnd);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{

	createApplication(this);
	SetTimer(1, (UINT)(application->fps*1.085f), NULL);

	RBX::AppManager* manager = RBX::AppManager::get();
	if (!manager->fileName.empty())
	{
		SetWindowTextA(manager->fileName.c_str());
	}

	return 1;
}

void CChildFrame::OnKillFocus(CWnd* pNewWnd)
{
	CMDIChildWnd::OnKillFocus(pNewWnd);
}

void CChildFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	DestroyCursor(guryCursor);

	CMDIChildWndEx::OnClose();
}


BOOL CChildFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	if (application)
	{
		application->getCamera()->cam_zoom(zDelta > 0);
	}

	return CMDIChildWndEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	// TODO: Add your message handler code here
	CMDIChildWndEx::OnSize(nType, cx, cy);
}

void CChildFrame::OnSizing(UINT fwSide, LPRECT pRect)
{

	CMDIChildWndEx::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
}

void CChildFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{

	if (application)
	{
		int cx = lpwndpos->cx;
		int cy = lpwndpos->cy;
		Rect2D viewport = application->renderDevice->getViewport();
		application->renderDevice->notifyResize(cx,cy);
		application->renderDevice->setViewport(Rect2D::xywh(viewport.x0(), viewport.y0(), application->renderDevice->getWidth(), application->renderDevice->getHeight()));
		application->onGraphics();
	}

	CMDIChildWndEx::OnWindowPosChanged(lpwndpos);

	// TODO: Add your message handler code here
}


void CChildFrame::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{

	if (application)
	{
		int cx = lpwndpos->cx;
		int cy = lpwndpos->cy;
		Rect2D viewport = application->renderDevice->getViewport();
		application->renderDevice->notifyResize(cx,cy);
		application->renderDevice->setViewport(Rect2D::xywh(viewport.x0(), viewport.y0(), application->renderDevice->getWidth(), application->renderDevice->getHeight()));
		application->onGraphics();
	}

	CMDIChildWndEx::OnWindowPosChanging(lpwndpos);

	// TODO: Add your message handler code here
}


//void CChildFrame::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CMDIChildWndEx::OnPaint() for painting messages
//}


//int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
	// TODO:  Add your specialized creation code here

	//SetTimer(1, 20, NULL);

//	return 0;
//}

void CChildFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

	if (application)
	{
		application->mainProcessStep();
	}
}

void CChildFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	InvalidateRect(0, 0);

	CMDIChildWndEx::OnTimer(nIDEvent);
}


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
