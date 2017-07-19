#pragma once


// A tree control that doesn't allow nodes to be collapsed, and has custom
// draw handlers.
class CBuffyTreeCtrl : public CWindowImpl<CBuffyTreeCtrl, CTreeViewCtrl>,
	public CCustomDraw<CBuffyTreeCtrl>
{
public:
	BEGIN_MSG_MAP(CBuffyTreeCtrl)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(TVN_ITEMEXPANDING, OnItemExpanding)
		CHAIN_MSG_MAP_ALT(CCustomDraw<CBuffyTreeCtrl>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnItemExpanding ( NMHDR* phdr );

	DWORD OnPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD);
	DWORD OnItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD);
};