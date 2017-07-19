#include "StdAfx.h"
#include "ODButtonImpl.h"
#include "resource.h"


CODButtonImpl::CODButtonImpl()
{
	m_bmp.LoadBitmap ( IDB_ALYSON );
}

void CODButtonImpl::DrawItem ( LPDRAWITEMSTRUCT lpdis )
{
	CDCHandle dc = lpdis->hDC;
	CDC dcMem;

	dcMem.CreateCompatibleDC ( dc );
	dc.SaveDC();
	dcMem.SaveDC();

	// Draw the button's background, red if it has the focus, blue if not.
	if ( lpdis->itemState & ODS_FOCUS ) 
		dc.FillSolidRect ( &lpdis->rcItem, RGB(255,0,0) );
	else
		dc.FillSolidRect ( &lpdis->rcItem, RGB(0,0,255) );

	// Draw the bitmap in the top-left, or offset by 1 pixel if the button
	// is clicked.
	dcMem.SelectBitmap ( m_bmp );

	if ( lpdis->itemState & ODS_SELECTED ) 
		dc.BitBlt ( 1, 1, 80, 80, dcMem, 0, 0, SRCCOPY );
	else
		dc.BitBlt ( 0, 0, 80, 80, dcMem, 0, 0, SRCCOPY );

	dcMem.RestoreDC(-1);
	dc.RestoreDC(-1);
}