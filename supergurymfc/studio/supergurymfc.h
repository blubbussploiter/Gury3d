
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
	afx_msg void OnFileNew();
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
		afx_msg void OnInsertObject();
		afx_msg void OnInsertModel();
		afx_msg void OnExecuteScript();
		afx_msg void OnLoadCharacter();
		afx_msg void OnRunRunService();
		afx_msg void OnPauseRunService();
		afx_msg void OnResetRunService();
		afx_msg void OnMoveUp();
		afx_msg void OnMoveDown();
		afx_msg void OnTiltLeft();
		afx_msg void OnTiltRight();
		afx_msg void OnTiltUp();
		afx_msg void OnTiltDown();
		afx_msg void OnZoomIn();
		afx_msg void OnZoomOut();
		afx_msg void OnLookat();
		afx_msg void OnZoomExtents();
		afx_msg void OnMouseToolSelected();
};

extern CsupergurymfcApp theApp;
