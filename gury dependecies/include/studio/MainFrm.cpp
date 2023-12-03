
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "framework.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndEditToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndEditToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		MessageBox("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRunServiceToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndRunServiceToolBar.LoadToolBar(IDR_RunService))
	{
		MessageBox("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		MessageBox("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndExplorer = new CExplorer();
	if (!m_wndExplorer)
	{
		MessageBox("Failed to create explorer");
		return -1;
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndExplorer->ShowWindow(1);
	m_wndExplorer->UpdateWindow();

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	//m_wndEditToolBar.EnableDocking(CBRS_ALIGN_TOP);
	//m_wndRunServiceToolBar.EnableDocking(CBRS_ALIGN_TOP);

	//EnableDocking(CBRS_ALIGN_TOP);

	//DockControlBar(&m_wndEditToolBar);
	//DockControlBar(&m_wndRunServiceToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}
