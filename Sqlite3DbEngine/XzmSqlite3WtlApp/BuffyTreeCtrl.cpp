#include "StdAfx.h"
#include "BuffyTreeCtrl.h"


LRESULT CBuffyTreeCtrl::OnItemExpanding ( NMHDR* phdr )
{
	NMTREEVIEW* pnmtv = (NMTREEVIEW*) phdr;

	if ( pnmtv->action & TVE_COLLAPSE )
		return TRUE;    // don't allow it
	else
		return FALSE;   // allow it
}

DWORD CBuffyTreeCtrl::OnPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD)
{
	// Get notifications for every tree item being drawn.
	return CDRF_NOTIFYITEMDRAW;
}

DWORD CBuffyTreeCtrl::OnItemPrePaint(int idCtrl, LPNMCUSTOMDRAW lpNMCD)
{
	NMTVCUSTOMDRAW* pnmtv = (NMTVCUSTOMDRAW*) lpNMCD;

	switch ( lpNMCD->lItemlParam )
	{
	case 1:
		pnmtv->clrText = RGB(0,128,0);
		break;

	case 2:
		pnmtv->clrText = RGB(255,0,0);
		break;

	case 3:
		pnmtv->clrText = RGB(0,0,255);
		break;

	case 4:
		pnmtv->clrText = RGB(196,196,196);
		break;
	}

	return CDRF_DODEFAULT;
}