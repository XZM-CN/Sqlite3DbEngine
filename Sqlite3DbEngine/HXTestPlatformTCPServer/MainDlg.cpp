#include "stdafx.h"
#include "MainDlg.h "

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	m_pServerObj = new CTCPServer();
	Innermain();

	return TRUE;
}

void CMainDlg::Innermain()
{
	int nRetCode = 0;
	char buf[4096];

	cout << "This is written by Boby Thomas.\r\n";
	cout << "This aplication act as a chat server.\n";
	cout << "Messages from any pc will be broadcasted to all connected pcs.\n";
	cout << "Connect to the server pc port 8084(Digital BOBY).\n";
	cout << "Press ONLY ENTER to quit.\n";
	cout << "=================================================\n";

	if(!m_pServerObj->IsConnected())
	{
		cout<<"\nFailed to initialise server socket";
		cout<<"\nThis is boby signing off : Bye";
		getch();
		return ;
	}

	// AfxBeginThread(CTCPServer::ServerListenThread,0);

	HANDLE hThread;
	DWORD dwThreadId;
	hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CTCPServer::ServerListenThread, m_pServerObj, 0, &dwThreadId);


	while(gets(buf))
	{
		if(strlen(buf) == 0)
			break;

		// ¹ã²¥Êý¾Ý
		if(m_pServerObj->SendMultiplyMessagePort(buf))
		{
			cout<<"Problem in connecting to server. Check whether server is running\n";
			break;
		}
	}

	cout<<"This is Boby signing off.";
	getch();

	return;
}
