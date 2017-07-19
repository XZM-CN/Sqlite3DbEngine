#pragma once


// Custom styles for our check list ctrl. Note that when subclassing an existing
// control, only the list extended styles (LVS_EX_*) are used, so that's why
// the first two params here are 0.
typedef CCheckListViewCtrlImplTraits< 
	0, 0,
	LVS_EX_CHECKBOXES   |
	LVS_EX_GRIDLINES    |
	LVS_EX_UNDERLINEHOT |
	LVS_EX_ONECLICKACTIVATE> CMyCheckListTraits;

class CMyCheckListCtrl : 
	public CCheckListViewCtrlImpl<CMyCheckListCtrl, CListViewCtrl, CMyCheckListTraits>
{
private:
	typedef CCheckListViewCtrlImpl<CMyCheckListCtrl, CListViewCtrl, CMyCheckListTraits> baseClass;
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_CheckListView"), GetWndClassName())

	BEGIN_MSG_MAP(CMyCheckListCtrl)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};