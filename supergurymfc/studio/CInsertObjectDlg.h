#pragma once
#include "afxdialogex.h"


// CInsertObjectDlg dialog

class CInsertObjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInsertObjectDlg)

public:
	CInsertObjectDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CInsertObjectDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSERTOBJECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit Class2Insert;
	afx_msg void OnEnChangeClassinsert();
	CString CLASSTOINSERT;
};
