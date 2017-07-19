#include "StdAfx.h"
#include "ButtonImpl.h"



CButtonImpl::CButtonImpl(int type)
	: m_type(type)
{

}

LRESULT CButtonImpl::OnSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)

{
	static HCURSOR hcur = NULL;
	switch (m_type)
	{
	case 0x01:
		hcur = LoadCursor ( NULL, IDC_ARROW );
		break;
	case 0x02:
		hcur = LoadCursor ( NULL, IDC_IBEAM );
		break;
	case 0x03:
		hcur = LoadCursor ( NULL, IDC_WAIT );
		break;
	case 0x04:
		hcur = LoadCursor ( NULL, IDC_CROSS );
		break;
	case 0x05:
		hcur = LoadCursor ( NULL, IDC_UPARROW );
		break;
	case 0x06:
		hcur = LoadCursor ( NULL, IDC_SIZE );
		break;
	case 0x07:
		hcur = LoadCursor ( NULL, IDC_ICON );
		break;
	case 0x08:
		hcur = LoadCursor ( NULL, IDC_SIZENWSE );
		break;
	case 0x09:
		hcur = LoadCursor ( NULL, IDC_SIZENESW );
		break;
	case 0x0A:
		hcur = LoadCursor ( NULL, IDC_SIZEWE );
		break;
	case 0x0B:
		hcur = LoadCursor ( NULL, IDC_SIZENS );
		break;
	case 0x0C:
		hcur = LoadCursor ( NULL, IDC_SIZEALL );
		break;
	case 0x0D:
		hcur = LoadCursor ( NULL, IDC_NO );
		break;
	case 0x0E:
		hcur = LoadCursor ( NULL, IDC_HAND );
		break;
	case 0x0F:
		hcur = LoadCursor ( NULL, IDC_APPSTARTING );
		break;
	case 0x10:
		hcur = LoadCursor ( NULL, IDC_HELP );
		break;
	default:
		hcur = LoadCursor ( NULL, IDC_WAIT );
		break;
	}

	if ( NULL != hcur )
	{
		SetCursor ( hcur );
		return TRUE;
	}
	else
	{
		SetMsgHandled(false);
		return FALSE;
	}
}
