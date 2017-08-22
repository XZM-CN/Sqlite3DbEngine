#include "stdafx.h"
#include "MainDlg.h"

#include "ChatClient.h"


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

	m_pClientObj = new CChatClient();
	Innermain();
	

	return TRUE;
}
#include <time.h>
char *rand_str(char *str,const int len)
{
	srand(time(NULL));
	int i;
	for(i=0;i<len;++i)
	{
		switch((rand()%3))
		{
		case 1:
			str[i]='A'+rand()%26;
			break;
		case 2:
			str[i]='a'+rand()%26;
			break;
		default:
			str[i]='0'+rand()%10;
			break;
		}
	}
	str[++i]='\0';
	return str;
}

int CMainDlg::Innermain(int argc /*= 0*/, char* argv /*= NULL*/)
{
	char buf[4096];
	cout<<"This is a client TCP/IP application\nConnecting to port 8084\n";
	cout<<"\nPress ONLY ENTER to quit";
	cout<<"\nWritten by Boby Thomas";
	cout<<"\n===============================================\n";

	char strTempPath[MAX_PATH];
	memset(strTempPath,0,MAX_PATH);
	GetModuleFileNameA(NULL,strTempPath,MAX_PATH);
	strrchr(strTempPath,'\\')[0]=0;   // 将\字符置0
	strrchr(strTempPath,'\\')[1]=0;     // 将\后第一个字符置0,看使用情况酌情处理
	strcat(strTempPath,"server.ini");

	FILE *fp = fopen(strTempPath,"r");
	if(fp == NULL)
	{
		cout<<"\nUnable to open server.ini. Please specify server IPsddress in server.ini";
		return 1; // main failed
	}
	string sServerAddress;
	while((fgets(buf,4096,fp)) != NULL)
	{
		if(buf[0] == '#')
			continue;
		sServerAddress = buf;

	}
	fclose(fp);

	if(sServerAddress.size() == 0)
	{
		cout<<"\nUnable to find server IP address in server.ini";
		cout<<"\nPlease set server IPaddress";
		cout<<"\nThis is Boby Signing off. BYE:";
		getch();
		return 0;
	}

	m_pClientObj->Init(sServerAddress.c_str(),514);
	if(!m_pClientObj->IsConnected())
	{
		cout<<"\nUnable to connect to the IP address specified in server.ini";
		cout<<"\nPlease check server IPaddress";
		cout<<"\nThis is Boby Signing off. BYE:";
		getch();
		return 0;	
	}

	// AfxBeginThread(MessageRecThread,0);
	HANDLE hThread;
	DWORD dwThreadId;
	hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CChatClient::MessageRecThread, m_pClientObj, 0, &dwThreadId);

	/*
	while(gets(buf))
	{
		if(strlen(buf) == 0){
			cout<<"\nthe buffer is NULL,the while is breaking";
			break;
		}
		if(m_pClientObj->SendMessagePort(buf))
		{
			cout<<"Problem in connecting to server. Check whether server is running\n";
			break;
		}
	}
	*/
	char name[20];
	cout<<">--"<<rand_str(name,20)<<endl;
	strcat(buf,name);
	int i = 0;
	while (1)
	{
		memset(buf,0,4096);
		sprintf(buf,"%s%3d",name,i++);
		if(strlen(buf) == 0){
			cout<<"\nthe buffer is NULL,the while is breaking";
			break;
		}
		if(m_pClientObj->SendMessagePort(buf))
		{
			cout<<"Problem in connecting to server. Check whether server is running\n";
			break;
		}

		Sleep(2000);
	}

	cout<<"\nThis is Boby Signing off. BYE:";
	getch();
	return 0;
}
