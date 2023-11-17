
// supergurymfc.h : main header file for the supergurymfc application
//
#pragma once

#include "framework.h"
#include "resource.h"       // main symbols

// CsupergurymfcApp:
// See supergurymfc.cpp for the implementation of this class
//

class CsupergurymfcApp : public CWinAppEx
{
public:
	CsupergurymfcApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnFileOpen();
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
};

extern CsupergurymfcApp theApp;
