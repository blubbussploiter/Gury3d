
// 3DStudioView.cpp : implementation of the CMy3DStudioView class
//

#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "3DStudio.h"
#endif

#include "3DStudioDoc.h"
#include "3DStudioView.h"


// CMy3DStudioView

IMPLEMENT_DYNCREATE(CMy3DStudioView, CFormView)

BEGIN_MESSAGE_MAP(CMy3DStudioView, CFormView)
END_MESSAGE_MAP()

// CMy3DStudioView construction/destruction

CMy3DStudioView::CMy3DStudioView() noexcept
	: CFormView(IDD_MY3DSTUDIO_FORM)
{
	// TODO: add construction code here

}

CMy3DStudioView::~CMy3DStudioView()
{
}

void CMy3DStudioView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CMy3DStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CMy3DStudioView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

}