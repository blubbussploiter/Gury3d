
// 3DStudioView.h : interface of the CMy3DStudioView class
//

#pragma once

#include "3DStudioDoc.h"

class CMy3DStudioView : public CFormView
{
protected: // create from serialization only
	CMy3DStudioView() noexcept;
	DECLARE_DYNCREATE(CMy3DStudioView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_MY3DSTUDIO_FORM };
#endif

// Attributes
public:
	CMy3DStudioDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMy3DStudioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in 3DStudioView.cpp
inline CMy3DStudioDoc* CMy3DStudioView::GetDocument() const
   { return reinterpret_cast<CMy3DStudioDoc*>(m_pDocument); }
#endif

