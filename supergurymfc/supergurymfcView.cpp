
// supergurymfcView.cpp : implementation of the CsupergurymfcView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "supergurymfc.h"
#endif

#include "supergurymfcDoc.h"
#include "supergurymfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <thread>


// CsupergurymfcView

IMPLEMENT_DYNCREATE(CsupergurymfcView, CView)

BEGIN_MESSAGE_MAP(CsupergurymfcView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CsupergurymfcView construction/destruction

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

void createApplication(CsupergurymfcView* frame)
{
	RBX::AppManager* manager = RBX::AppManager::singleton();

	if (!frame->application)
	{
		RBX::Experimental::Application* app = manager->instantiate(frame->GetSafeHwnd());

		if (!manager->toLoad.empty())
		{
			app->rbxlFile = manager->toLoad;
			manager->toLoad = std::string();
		}

		if (!manager->fileName.empty())
		{
			frame->SetWindowTextA(manager->fileName.c_str());
			manager->fileName = std::string();
		}

		frame->application = app;
		suspendCurrentApplication(frame->application);

		frame->application->start();
		manager->start();
	}
}

CsupergurymfcView::CsupergurymfcView() noexcept
{
	// TODO: add construction code here

}

CsupergurymfcView::~CsupergurymfcView()
{
}

BOOL CsupergurymfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CsupergurymfcView::OnInitialUpdate() {
	std::thread{ createApplication, this }.detach();
}
// CsupergurymfcView drawing

void CsupergurymfcView::OnDraw(CDC* /*pDC*/)
{
	CsupergurymfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CsupergurymfcView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CsupergurymfcView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}

void CsupergurymfcView::OnSetFocus(CWnd* pNewWnd) {
	RBX::AppManager* manager = RBX::AppManager::singleton();

	if (application)
	{
		suspendCurrentApplication(application);
		application->onFocus();
	}
}

void CsupergurymfcView::OnKillFocus(CWnd* pNewWnd) 
{
	CView::OnKillFocus(pNewWnd);
}
// CsupergurymfcView diagnostics

#ifdef _DEBUG
void CsupergurymfcView::AssertValid() const
{
	CView::AssertValid();
}

void CsupergurymfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CsupergurymfcDoc* CsupergurymfcView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CsupergurymfcDoc)));
	return (CsupergurymfcDoc*)m_pDocument;
}
#endif //_DEBUG


// CsupergurymfcView message handlers
