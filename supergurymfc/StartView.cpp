#include "pch.h"
#include "framework.h"

#include "supergurymfcDoc.h"
#include "StartView.h"

IMPLEMENT_DYNCREATE(CStartView, CView);

BEGIN_MESSAGE_MAP(CStartView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

CStartView::CStartView() noexcept {

}

CStartView::~CStartView() {

}

BOOL CStartView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CStartView::OnInitialUpdate() {
}

void CStartView::OnDraw(CDC* pDC) {
	CsupergurymfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->SetTitle(_T("Start Page"));
}

void CStartView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStartView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
}

void CStartView::OnSetFocus(CWnd* pNewWnd) {
}