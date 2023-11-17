
#pragma once

#include "selection.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTree window

class CViewTree : public CTreeCtrl
{
private:
	RBX::ISelectable* lastSelected;
	std::vector<HTREEITEM> selection;
// Construction
public:
	CViewTree() noexcept;

	void DeselectItem(HTREEITEM item);
	void SelectItem(HTREEITEM item, BOOL multiSelect);

	void SelectInstance(RBX::Instance* instance);

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
