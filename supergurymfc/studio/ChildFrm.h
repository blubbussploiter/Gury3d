
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

#include "..\appmanager.h"

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

	// Attributes

protected:
	CSplitterWnd m_wndSplitter;

public:

	bool isChildFrame;
	HCURSOR guryCursor;

	RBX::Experimental::Application* application;

	// Operations
public:

	// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext);

	afx_msg void OnSetFocus(CWnd* pNewWnd);

	// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
//	afx_msg void OnPaint();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnPaint();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

