
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

#include "3DStudio.h"
#include "../appmanager.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// Attributes

protected:
	CSplitterWnd m_wndSplitter;

public:

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
};
