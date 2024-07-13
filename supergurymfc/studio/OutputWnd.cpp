
#include "pch.h"
#include "framework.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

int r = 0, g = 0, b = 0;

COutputWnd::COutputWnd() noexcept
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create output panes:
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	//if (!m_wndOutput.Create(dwStyle, rectDummy, this, 2))
	if (!m_wndOutput.Create("", dwStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	UpdateFonts();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndOutput.SetWindowPos (nullptr, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax =
			(((cxExtentMax) > ((int)dc.GetTextExtent(strItem).cx)) ? (cxExtentMax) : ((int)dc.GetTextExtent(strItem).cx));
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::UpdateFonts()
{
	m_wndOutput.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::SetTextColor(int r0, int g0, int b0)
{
	r = r0;
	g = g0;
	b = b0;
}

HBRUSH COutputWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
	{ 
		pDC->SetTextColor(RGB(r, g, b));
		break;
	}
	default: break;
	}

	return CDockablePane::OnCtlColor(pDC, pWnd, nCtlColor);
}
