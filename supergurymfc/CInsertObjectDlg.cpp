// CInsertObjectDlg.cpp : implementation file
//

#include "pch.h"
#include "supergurymfc.h"
#include "afxdialogex.h"
#include "CInsertObjectDlg.h"


// CInsertObjectDlg dialog

IMPLEMENT_DYNAMIC(CInsertObjectDlg, CDialogEx)

CInsertObjectDlg::CInsertObjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INSERTOBJECT, pParent)
	, CLASSTOINSERT(_T(""))
{

}

CInsertObjectDlg::~CInsertObjectDlg()
{
}

void CInsertObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, ID_CLASSINSERT, CLASSTOINSERT);
}


BEGIN_MESSAGE_MAP(CInsertObjectDlg, CDialogEx)
END_MESSAGE_MAP()


// CInsertObjectDlg message handlers
