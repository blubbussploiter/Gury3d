#pragma once

class CStartView : public CView
{
protected: // create from serialization only
	CStartView() noexcept;
	DECLARE_DYNCREATE(CStartView)

		// Attributes
public:
	CsupergurymfcDoc* GetDocument() const;

	// Operations
public:
	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	afx_msg void OnSetFocus(CWnd* pNewWnd);
protected:

	// Implementation
public:
	virtual ~CStartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in supergurymfcView.cpp
inline CsupergurymfcDoc* CStartView::GetDocument() const
{
	return reinterpret_cast<CsupergurymfcDoc*>(m_pDocument);
}
#endif

