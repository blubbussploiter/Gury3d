
#include <G3D/Color3.h>

#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "supergurymfc.h"

#include "stdout.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd() noexcept
{

}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + 0, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() -(cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertiesWnd::ClearProperties()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.RedrawWindow();
	catergories.clear();
}

void CPropertiesWnd::FillProperties(RBX::Instance* i)
{
	rttr::type base = rttr::detail::get_type_from_instance(i);
	rttr::type type = rttr::type::get_by_name(i->getClassName());

	if (!type) type = base;

	ClearProperties();

	for (auto& p : type.get_properties())
	{
		AddProperty(i, p);
	}
}

void setProperty(rttr::variant v, CMFCPropertyGridProperty*& property, std::string name)
{
	rttr::type t = v.get_type();

	if (t == rttr::type::get<std::string>())
	{
		std::string s = v.to_string();
		property = new CMFCPropertyGridProperty(name.c_str(), (_variant_t)s.c_str(), "");
	}

	if (t == rttr::type::get<bool>())
	{
		property = new CMFCPropertyGridProperty(name.c_str(), (_variant_t)v.to_bool(), "");
	}

	if (t == rttr::type::get<float>())
	{
		property = new CMFCPropertyGridProperty(name.c_str(), v.to_float(), "");
	}

	if (t == rttr::type::get<double>())
	{
		property = new CMFCPropertyGridProperty(name.c_str(), v.to_double(), "");
	}

	if (t == rttr::type::get<int>())
	{
		int n = v.to_int();
		property = new CMFCPropertyGridProperty(name.c_str(), (_variant_t)n, "");
	}

	if (t == rttr::type::get<G3D::Vector3>())
	{
		property = new CMFCPropertyGridProperty(name.c_str());
	}

	if (t == rttr::type::get<G3D::Color3>())
	{
		G3D::Color3 c = v.convert<G3D::Color3>();
		property = new CMFCPropertyGridColorProperty(name.c_str(), RGB(c.r * 255, c.g * 255, c.b * 255));
	}

	if (t.is_enumeration())
	{
		rttr::enumeration e = t.get_enumeration();
		rttr::string_view sv = e.value_to_name(v);
		std::string s = sv.to_string();

		property = new CMFCPropertyGridProperty(name.c_str(), (_variant_t)s.c_str(), "");

		for (auto& val : e.get_names())
		{
			std::string str = val.to_string();
			property->AddOption(str.c_str());
		}
	}
}

void CPropertiesWnd::AddProperty(RBX::Instance* i, rttr::property p)
{
	rttr::variant var = p.get_metadata("Type");
	if (!var.is_valid()) return;

	std::string name = p.get_name().to_string();
	RBX::Types type = var.convert<RBX::Types>();

	CMFCPropertyGridProperty* catergory = catergories[type];
	CMFCPropertyGridProperty* property = 0;

	rttr::variant v = p.get_value(i);

	if (!catergory)
	{
		catergory = new CMFCPropertyGridProperty(RBX::getType(type).c_str());

		m_wndPropList.AddProperty(catergory);
		catergories[type] = catergory;
	}

	setProperty(v, property, name);

	if (property)
	{
		if (p.is_readonly())
		{
			property->Enable(0);
		}

		catergory->AddSubItem(property);
	}
	m_wndPropList.ExpandAll();
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);
	
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.SetVSDotNetLook(0);
	m_wndPropList.MarkModifiedProperties();

	m_wndPropList.EnableHeaderCtrl(0);
	m_wndPropList.EnableScrollBar(SB_HORZ);

	m_wndPropList.SetBoolLabels("true", "false");
	m_wndPropList.SetCustomColors(-1, -1, RGB(0, 0, 0), RGB(0, 0, 0), -1, -1, -1);

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}
