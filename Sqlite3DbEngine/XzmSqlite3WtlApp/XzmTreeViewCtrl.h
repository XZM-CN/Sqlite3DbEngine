#pragma once


class CXzmTreeViewCtrl :
	public CWindowImpl<CXzmTreeViewCtrl, CTreeViewCtrl>
{
public:
	CXzmTreeViewCtrl(void);
	~CXzmTreeViewCtrl(void);
public:
	//DECLARE_WND_CLASS(NULL)
	DECLARE_WND_SUPERCLASS(NULL, CTreeViewCtrl::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CXzmTreeViewCtrl)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(TVN_ITEMEXPANDING, OnItemExpanding)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnItemExpanding(NMHDR* phdr)
	{
		NMTREEVIEW* pnmtv = (NMTREEVIEW*)phdr;

		if (pnmtv->action & TVE_COLLAPSE)
			return TRUE;
		else
			return FALSE;
	}
};