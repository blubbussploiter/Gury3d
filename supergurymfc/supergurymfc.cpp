
// supergurymfc.cpp : Defines the class behaviors for the application.
//

#include <fstream>

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "supergurymfc.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "CInsertObjectDlg.h"

#include "supergurymfcDoc.h"
#include "supergurymfcView.h"


#include "datamodel.h"
#include "crashreporter.h"
#include "scriptcontext.h"
#include "players.h"
#include "serializer.h"

#include "classes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CsupergurymfcApp

BEGIN_MESSAGE_MAP(CsupergurymfcApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CsupergurymfcApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, &CsupergurymfcApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_INSERT_OBJECT, &CsupergurymfcApp::OnInsertObject)
	ON_COMMAND(ID_INSERT_MODEL, &CsupergurymfcApp::OnInsertModel)
	ON_COMMAND(ID_TOOLS_EXECUTESCRIPT, &CsupergurymfcApp::OnExecuteScript)
	ON_COMMAND(ID_TOOLS_LOADCHARACTER, &CsupergurymfcApp::OnLoadCharacter)
	ON_COMMAND(ID_TOOLS_RUNRUNSERVICE, &CsupergurymfcApp::OnRunRunService)
END_MESSAGE_MAP()


// CsupergurymfcApp construction

CsupergurymfcApp::CsupergurymfcApp() noexcept
{
	m_bHiColorIcons = FALSE;

	m_nAppLook = ID_VIEW_APPLOOK_OFF_XP;

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("supergurymfc.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CsupergurymfcApp object

CsupergurymfcApp theApp;


// CsupergurymfcApp initialization

BOOL CsupergurymfcApp::InitInstance()
{
	RBX::CrashReporter::singleton()->start();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(CsupergurymfcDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CsupergurymfcView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	CMainFrame::mainFrame = pMainFrame;

	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}

	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!cmdInfo.m_nShellCommand)   // actually none
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow(); 

	return TRUE;
}

int CsupergurymfcApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	return CWinAppEx::ExitInstance();
}

// CsupergurymfcApp message handlers


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

void CsupergurymfcApp::OnFileOpen()
{
	static TCHAR BASED_CODE szFilter[] = _T("ROBLOX Place Files (*.rbxl;*.rbxlx)|*.rbxl;*.rbxlx|All Files(*.*)|*.*||");

	CFileDialog dlgFile(TRUE, "Open", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter);
	
	if (dlgFile.DoModal() == IDOK)
	{
		RBX::AppManager* manager = RBX::AppManager::singleton();

		CString file = dlgFile.GetPathName();
		CString name = dlgFile.GetFileName();

		OnFileNew();

		manager->fileName = name.GetString();
		manager->toLoad = file.GetString();
		
	}

}


// App command to run the dialog
void CsupergurymfcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CsupergurymfcApp::OnInsertObject()
{
	if (RBX::AppManager::isReady())
	{
		CInsertObjectDlg dialog;
		if (dialog.DoModal() == IDOK)
		{
			std::string classN;
			classN = dialog.CLASSTOINSERT;

			RBX::Instance* n = RBX::fromName(classN);
			if (!n)
			{
				std::string err = RBX::Format("Failed to insert object of type '%s'. Did you spell it correctly?", classN.c_str());
				MessageBox(0, err.c_str(), "Gury3d", MB_ICONWARNING);
			}
			else
			{
				RBX::Instance* parent = RBX::Workspace::singleton();
				if (parent) n->setParent(parent);
			}
		}
	}
}


void CsupergurymfcApp::OnInsertModel()
{
	if (RBX::AppManager::isReady())
	{
		static TCHAR BASED_CODE szFilter[] = _T("ROBLOX Model Files (*.rbxm;*.rbxmx)|*.rbxm; *.rbxmx|All Files (*.*)|*.*||");

		CFileDialog dlgFile(TRUE, "Open", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter);

		if (dlgFile.DoModal() == IDOK)
		{
			CString file = dlgFile.GetPathName();
			RBX::Instances* instances = RBX::Serializer::loadInstances(file.GetString());

			if (instances->size() > 0)
			{
				RBX::Instance* at = instances->at(0);
				RBX::Instance* parent = RBX::Workspace::singleton();

				if (at && parent)
				{
					RBX::ModelInstance* model = dynamic_cast<RBX::ModelInstance*>(at);
					RBX::Camera* camera = RBX::Camera::singleton();

					at->setParent(parent);

					if (model)
					{
						if (!camera)
						{
							return;
						}

						RBX::PartInstance* primaryPart;
						primaryPart = model->getPrimaryPart();

						if (primaryPart)
						{
							camera->lookAt(primaryPart->getPosition());
						}
					}
				}
			}
		}
	}
}

void CsupergurymfcApp::OnExecuteScript()
{
	if (!RBX::AppManager::isReady())
	{
		OnFileNew();
	}

	static TCHAR BASED_CODE szFilter[] = _T("Scripts (*.rbxs;*.lua;*.txt)|*.rbxs; *.lua;*.txt|All Files (*.*)|*.*||");

	CFileDialog dlgFile(TRUE, "Open", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter);

	if (dlgFile.DoModal() == IDOK)
	{
		CString file = dlgFile.GetPathName();
		RBX::Datamodel* datamodel = RBX::Datamodel::getDatamodel();

		if (datamodel && datamodel->scriptContext)
		{
			std::ifstream stream(file);
			std::string script, line = "";

			if (!stream.good()) return;
			while (std::getline(stream, line)) { script += line + ' '; }
			stream.close();

			datamodel->scriptContext->execute(script);
		}
	}
}

void CsupergurymfcApp::OnLoadCharacter()
{
	RBX::Network::Players* players;
	RBX::Network::Player* player;
	RBX::Datamodel* datamodel = RBX::Datamodel::getDatamodel();

	if (!datamodel) return;
	players = datamodel->players;
	if (!players) return;

	player = players->localPlayer;
	if (player) player->loadCharacter();
}

void CsupergurymfcApp::OnRunRunService()
{
	RBX::Datamodel* datamodel = RBX::Datamodel::getDatamodel();
	if (!datamodel) return;
	datamodel->runService->run();
}

// CsupergurymfcApp customization load/save methods

void CsupergurymfcApp::PreLoadState()
{

}

void CsupergurymfcApp::LoadCustomState()
{
}

void CsupergurymfcApp::SaveCustomState()
{
}

// CsupergurymfcApp message handlers
