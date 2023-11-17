// CCommandBar.cpp : implementation file
//

#include "pch.h"
#include "supergurymfc.h"
#include "CCommandBar.h"


// CCommandBar

IMPLEMENT_DYNAMIC(CCommandBar, CDockablePane)

CCommandBar::CCommandBar()
{

}

CCommandBar::~CCommandBar()
{
}

int CCommandBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

    return 0;
}


BEGIN_MESSAGE_MAP(CCommandBar, CDockablePane)
END_MESSAGE_MAP()



// CCommandBar message handlers


