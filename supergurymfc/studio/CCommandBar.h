#pragma once

// CCommandBar

class CCommandBar : public CDockablePane
{
	DECLARE_DYNAMIC(CCommandBar)

public:
	CCommandBar();
	virtual ~CCommandBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};


