#include "StdAfx.h"
#include "AboutDlg.h"


CAboutDlg::CAboutDlg(void)
{
}


CAboutDlg::~CAboutDlg(void)
{
}

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
