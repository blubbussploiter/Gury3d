
// 3DStudio.h : main header file for the 3DStudio application
//
#pragma once

#include "framework.h"
#include "resource.h"       // main symbols

// CMy3DStudioApp:
// See 3DStudio.cpp for the implementation of this class
//

class CMy3DStudioApp : public CWinApp
{
public:
	CMy3DStudioApp() noexcept;

// Overrides
public:

	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();

	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();

	DECLARE_MESSAGE_MAP()

};

extern CMy3DStudioApp theApp;
