
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

#include "appmanager.h"

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
};

