
// supergurymfcView.h : interface of the CsupergurymfcView class
//

#pragma once

#include "appmanager.h"

class CsupergurymfcView : public CView
{
protected: // create from serialization only
	CsupergurymfcView() noexcept;
	DECLARE_DYNCREATE(CsupergurymfcView)

// Attributes
public:

	RBX::Experimental::Application* application;

	CsupergurymfcDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CsupergurymfcView();
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
inline CsupergurymfcDoc* CsupergurymfcView::GetDocument() const
   { return reinterpret_cast<CsupergurymfcDoc*>(m_pDocument); }
#endif

