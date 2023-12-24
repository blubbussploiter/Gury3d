
// 3DStudio.cpp : Defines the class behaviors for the application.
//

#include <thread>

#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "3DStudio.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "3DStudioDoc.h"
#include "3DStudioView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy3DStudioApp

BEGIN_MESSAGE_MAP(CMy3DStudioApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMy3DStudioApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CMy3DStudioApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMy3DStudioApp::OnFileOpen)
END_MESSAGE_MAP()


// CMy3DStudioApp construction

CMy3DStudioApp::CMy3DStudioApp() noexcept
{

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("My3DStudio.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMy3DStudioApp object

CMy3DStudioApp theApp;
CMainFrame* pMainFrame;

// CMy3DStudioApp initialization

BOOL CMy3DStudioApp::InitInstance()
{
	CWinApp::InitInstance();

	EnableTaskbarInteraction(FALSE);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_My3DStudioTYPE,
		RUNTIME_CLASS(CMy3DStudioDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMy3DStudioView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew &&
		!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CMy3DStudioApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinApp::ExitInstance();
}

// CMy3DStudioApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CMy3DStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMy3DStudioApp::OnFileOpen()
{
	CWinApp::OnFileOpen();
}

void CMy3DStudioApp::OnFileNew()
{
	CWinApp::OnFileNew();
}

// CMy3DStudioApp message handlers



