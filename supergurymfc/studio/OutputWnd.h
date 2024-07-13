
#pragma once

#include <string>

class COutputWnd : public CDockablePane
{
// Construction
public:
	COutputWnd() noexcept;

	void UpdateFonts();	

	void SetTextColor(int r, int g, int b);

	void AddText(std::string Text)
	{
		CString text;
		m_wndOutput.GetWindowTextA(text);
		text.Append(Text.c_str());
		m_wndOutput.SetWindowTextA(text);
	}

// Attributes
protected:

	CStatic m_wndOutput;

protected:

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

