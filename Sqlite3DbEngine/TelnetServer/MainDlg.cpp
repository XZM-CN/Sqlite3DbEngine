#include "stdafx.h"
#include "MainDlg.h"
#include "AboutDlg.h"

//
//////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <memory.h>
#include <process.h>    /* _beginthread, _endthread */
#include <iostream>
#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>		//strstr
#include "Xlist.h"
#include "blockingsocket.h"
#include "gameob.h"
using namespace std;


//
//////////////////////////////////////////////////////////////////////////
int repeat=1;

#define BUFFERSIZE 1000
CXList m_games;				//list of Chess Game Objects (NOTE: This isn't thread safe!)
CXList m_pending;			//list of games that are waiting for second player (NOTE: This isn't thread safe!)
CXList m_connections;		//list of all the people connected

enum tagEnum
{
	CREATE = 1,
	JOIN,
	WATCH,
	QUIT,
	PLAY,
	COLOR
};
char* cmds[]={"create","join","watch","quit","play","color"};

void logMsg(const char* s)
{
	printf("%s\n",s);
}

//This is the main game loop. It reads in users moves, and prints out the board
//contents to all people in a single game.
void GameLoop(void* pParam)
{
	logMsg("GameLoop start");

	CBoardGame* pgame=(CBoardGame*)pParam;
	char* help= "Enter the column & row of the piece you are moving from & to\r\n"
		"in the format FFTT. (ie: moving whites queen pawn ahead one from \r\n"
		"column 3, row 6 to column 3, row 5 would be entered 3635.)\r\n";
	char* entermove="What is your move? (?=help) ";
	char* whitesmove="It's whites move.";
	char* blacksmove="It's blacks move.";
	bool bAbort=false;
	bool bMadeMove=false;
	char inbuf[10];
	try
	{
		do
		{
			//handle whites move
			pgame->Draw();
			pgame->m_black->Print(whitesmove);
			bMadeMove=false;
			do
			{
				pgame->m_white->Write(entermove,strlen(entermove));
				pgame->m_white->ReadLine(inbuf,sizeof inbuf,300);
				if(!stricmp(inbuf,"quit"))
				{
					bAbort=true;
					pgame->m_white->Print("Aborting game");
					pgame->m_black->Print("White aborted game");
				}
				else if(!strcmp(inbuf,"?"))
				{
					pgame->m_white->Print(help);
				}
				else
				{
					if(strlen(inbuf)!=4)
						pgame->m_white->Print("Syntax error");
					else
					{
						if(pgame->ValidMove(inbuf[0],inbuf[1],inbuf[2],inbuf[3]))
						{
							bMadeMove=true;
							pgame->Move(inbuf[0],inbuf[1],inbuf[2],inbuf[3]);
						}
						else 
							pgame->m_white->Print("not a valid move");
					}
				}
			} while(!bMadeMove && !bAbort);
			//handle blacks move
			if(!bAbort)
			{
				pgame->Draw();
				pgame->m_white->Print(blacksmove);
				do
				{
					pgame->m_black->Write(entermove,strlen(entermove));
					pgame->m_black->ReadLine(inbuf,sizeof inbuf,300);
					if(!stricmp(inbuf,"quit"))
					{
						bAbort=true;
						pgame->m_black->Print("Aborting game");
						pgame->m_white->Print("Black aborted game");
					}
					else if(!strcmp(inbuf,"?"))
					{
						pgame->m_black->Print(help);
					}
					else
					{
						if(strlen(inbuf)!=4)
							pgame->m_white->Print("Syntax error");
						else
						{
							bMadeMove=true;
							pgame->Move(inbuf[0],inbuf[1],inbuf[2],inbuf[3]);
						}
					}
				} while(!bMadeMove && !bAbort);
			}
		} while(!bAbort);
	}
	catch(const char* e)
	{
		logMsg(e);
	}
	CXList* plist=m_games.FindItem(pgame);
	m_games.RemoveItem(plist);
	//	delete pgame;

	logMsg("GameLoop end");
}

//read in the persons user name, and make sure they are a valid user
bool Login(CTelnetSocket* psocket)
{
	char* login="Enter your login name (only \"guest\" works): ";
	char line[1000];

	psocket->Write(login,strlen(login));
	psocket->ReadLine(line,sizeof line);

	if(stricmp(line,"guest"))
		return false;

	psocket->m_wFlags |= CTelnetSocket::FLAG_VALIDATED;
	return true;

}

ChessGameOb* FindGame(const char* pgamename)
{
	logMsg("Findgame");
	ChessGameOb* o;
	CXList* plist=m_games.GetHead();
	while(plist)
	{
		o=(ChessGameOb*)plist->GetItem();
		logMsg(o->m_gamename);
		if(!stricmp(o->m_gamename,pgamename))
		{
			//found the game name
			return o;
		}
		plist=plist->GetNext();
	}
	return NULL;
}

ChessGameOb* FindPendingGame(const char* pgamename)
{
	logMsg("Find pending game");
	ChessGameOb* o;
	CXList* plist=m_pending.GetHead();
	while(plist)
	{
		o=(ChessGameOb*)plist->GetItem();
		logMsg(o->m_gamename);
		if(!stricmp(o->m_gamename,pgamename))
		{
			//found the game name
			return o;
		}
		plist=plist->GetNext();
	}
	return NULL;
}

int FindToken(const char* buf)
{
	for(int i=0; i<sizeof cmds/sizeof cmds[0]; i++)
	{
		if(strstr(buf,cmds[i]))
			return i+1;
	}
	return 0;
}

//send the chat in buf to all connections
void Chat(CPersonInstance* pInstance, char* buf)
{
	CPersonInstance* o;
	CXList* plist=m_connections.GetHead();
	while(plist)
	{
		o=(CPersonInstance*)plist->GetItem();
		if(o!=pInstance)
			o->Print(buf);

		plist=plist->GetNext();
	}
}

//Look at what the user typed, and act upon it.
bool ParsCmd(char* buf,CPersonInstance* pInstance)
{
	char* SyntaxErrorMsg="Syntax error. 'create chess|checkers name' " ;
	int itoken=FindToken(buf);
	char cmd[30],parm1[30],parm2[30];
	CBoardGame* pgame;
	CXList* plist;

	switch(itoken)
	{
	case WATCH:
		//user typed "watch game"
		sscanf(buf,"%s %s",cmd,parm1);
		pgame=FindGame(parm1);
		if(!pgame)
			pInstance->Print("No game found");
		else
		{
			if(!pgame->HasAllPlayers())
				pInstance->Print("Both players have to be playing befor watchin is aloud");
			else
			{
				pgame->AddWatcher(pInstance);
				pInstance->Print("You will see the board when the next person moves");
			}
			return true;
		}
		break;

	case CREATE:
		//user typed "create chess game"
		if(3!=sscanf(buf,"%s %s %s",cmd,parm1,parm2))	//get game name
		{
			pInstance->Print(SyntaxErrorMsg);
		}
		else
		{
			if(!stricmp(parm1,"checkers"))
			{
				pgame=new CheckerGameOb(pInstance,parm2);
				pInstance->Print("Created new checkers game");
			}
			else if(!stricmp(parm1,"chess"))
			{
				pInstance->Print("Created new chess game");
				pgame=new ChessGameOb(pInstance,parm2);
			}
			else
			{
				pInstance->Print(SyntaxErrorMsg);
				return false;
			}

			m_pending.AddTail(pgame);
#if 1
			logMsg("Pending games:");
			plist=m_pending.GetHead();
			while(plist)
			{
				pgame=(CBoardGame*)plist->GetItem();
				logMsg(pgame->m_gamename);
				plist=plist->GetNext();
			}
#endif
			pInstance->Print("You must now wait till someone joins your game");
			return true;
		}
		break;

	case JOIN:
		//user typed "join game"
		sscanf(buf,"%s %s",cmd,parm1);
		pgame=FindPendingGame(parm1);
		if(pgame)
		{
			pgame->AddBlack(pInstance);
			plist=m_pending.FindItem(pgame);
			if(plist)
				m_pending.RemoveItem(plist);
			m_games.AddTail(pgame);
			_beginthread(GameLoop,0,pgame);
			return true;
		}
		else
		{
			pInstance->Print("No game found");
		}
		break;

	case QUIT:
		//user typed "quit"
		pInstance->Print("Good bye");
		pInstance->Close();
		delete pInstance;
		return true;

	default:
		//LBP (01/08/99): Print this chat to all players in the lobby.
		Chat(pInstance,buf);
		break;
	}
	return false;
}

//LBP (01/08/99): this is the lobby of sorts.
void TalkProc(void* pParam)
{
	logMsg("TalkProc start");
	char* msg[]=
	{
		"Type 'create name' to make a game where 'name' is the game name you choose.\r\n" 	//0
		"Type 'join name' to join a player that created a game.\r\n"
		"Type 'watch name' to watch a game in progress",
		"Good bye.",						//1
		"Syntax Error"						//2
	};
	CPersonInstance* pInstance=(CPersonInstance*)pParam;
	char inbuf[CTelnetSocket::nSizeRecv];
	bool bAbort=false;

	m_connections.AddTail(pInstance);

	try
	{
		pInstance->Print(msg[0]);
		do
		{
			pInstance->Write("% ",2);
			pInstance->ReadLine(inbuf,sizeof inbuf,1800);
			if(ParsCmd(inbuf,pInstance))
			{
				bAbort=true;
			}
		} while(!bAbort);
	}
	catch(const char* e)
	{
		logMsg(e);
		pInstance->Cleanup();
		delete pInstance;
	}
	logMsg("TalkProc end");
	_endthread();
}

void AcceptProc(void* pParam)
{
	CBlockingSocket* psockListen=(CBlockingSocket*)pParam;
	CSockAddr saClient;
	CPersonInstance sConnect;
	char* buffer;

	try 
	{
		char* welcome="Welcome to the club.";
		char* nonuser="Sorry, members only;对不起，只允许会员登录";

		buffer=(char*)calloc(1,1000);
		assert(buffer);
		if(!psockListen->Accept(sConnect,saClient))
			_endthread();
		_beginthread(AcceptProc,0,pParam);		//get ready for another connection

		logMsg("Connection accepted");

		if(!Login(&sConnect))
		{
			sConnect.Print(nonuser);
			sConnect.Close();
		}
		else
		{
			sConnect.Print(welcome);

			//LBP (01/08/99): start up a lobby for this new connection
			CPersonInstance* newinstance=new CPersonInstance(sConnect);
			_beginthread(TalkProc,0,newinstance);
		}
	}
	catch(const char* e)
	{
		logMsg(e);
		sConnect.Cleanup();
	}
	_endthread();
}

void CheckKey(void* dummy)
{
	int a;
	cin >> a;
	repeat=0;
}

void ProcessGames()
{
	_beginthread(CheckKey,0,NULL);

	do
	{
	} while(repeat);
}


/************************************************************************/
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/
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

	// Hook up controls & variables没有这一行,变量与控件关联失败
	if(!DoDataExchange(false))
		return FALSE;

	InitXzmTree();

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}


LRESULT CMainDlg::OnTreeXzmClickTree(NMHDR* phdr)
{

	POINT pt;
	GetCursorPos(&pt);
	POINT MousePosition = pt;
	m_TreeXzm.ScreenToClient ( &MousePosition );
	TVHITTESTINFO HitInfo;
	HitInfo.pt = MousePosition;
	UINT uiMask = TVHT_ONITEMLABEL;//TVHT_ONITEM;// | TVHT_ONITEMRIGHT;
	HTREEITEM hItemHit = m_TreeXzm.HitTest(&HitInfo);
	if (hItemHit != NULL &&						// Obvious
		uiMask & HitInfo.flags &&				// Make sure we are on the label
		m_TreeXzm.GetParentItem(hItemHit) != NULL)	// Also that we have NO Parent!!
	{
		BSTR bstr = NULL;
		m_TreeXzm.GetItemText(hItemHit,bstr);
		//if(bstr == NULL) return 0;
		ATL::CString str(bstr);

		if(IsYourChild( _T("Server"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("启动")))
			{
				ChessWrapper();
			}
			else if(0 == str.Compare( _T("xxxxx")))
			{
			}
		}
	}

	return 0;
}

void CMainDlg::InitXzmTree()
{
	HTREEITEM hRoot, hItem;
	TCHAR pc_name[64] = { 0 };
	TV_INSERTSTRUCT TCItem;//插入数据项数据结构
	TCItem.hParent = TVI_ROOT;//增加根项
	TCItem.hInsertAfter = TVI_LAST;//在最后项之后
	TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText = _T("ServerCollect");
	TCItem.item.lParam = 0;//序号 
	TCItem.item.iImage = 0;//正常图标 
	TCItem.item.iSelectedImage = 1;//选中时图标 
	hItem = m_TreeXzm.InsertItem(&TCItem);//返回根项句柄 
	m_TreeXzm.Expand(hItem, TVE_EXPAND);//展开上一级树
	hRoot = hItem;

	HTREEITEM hItem1 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("Server"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("启动"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("xxxxx"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("xxxxx"));


	m_TreeXzm.Expand(hRoot, TVE_EXPAND);//展开上一级树

}
void CMainDlg::InitDlgTreeSurplus(HTREEITEM hItem,TV_INSERTSTRUCT& TCItem)
{
}

HTREEITEM CMainDlg::InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name)
{
	HTREEITEM hParentItem;
	hParentItem = hItem;
	//MultiByteToWideChar(CP_ACP, 0, troot->Name, strlen(troot->Name) + 1, pc_name,sizeof(pc_name) / sizeof(pc_name[0]));
	TCItem.item.pszText = pc_name;
	TCItem.hParent = hItem;
	hItem = Tree.InsertItem(&TCItem);//返回根项句柄

	Tree.Expand(hParentItem, TVE_EXPAND);//展开上一级树

	return hItem;
}

BOOL CMainDlg::IsYourChild(ATL::CString strParentName, CTreeViewCtrl& Tree,HTREEITEM &hSelItem)
{
	HTREEITEM hParentItem = Tree.GetParentItem(hSelItem);

	BSTR bstr = NULL;
	Tree.GetItemText(hParentItem,bstr);
	if(bstr == NULL)
		return FALSE;
	ATL::CString strParent(bstr);
	if(0 == strParentName.Compare(strParent))
	{
		return TRUE;
	}
	else
	{
		while (NULL != hParentItem)
		{
			bstr = NULL;
			hParentItem = Tree.GetParentItem(hParentItem);
			Tree.GetItemText(hParentItem,bstr);
			strParent.Format(_T("%s"),bstr);
			if(0 == strParentName.Compare(strParent))
				return TRUE;
		}
	}
	return FALSE;
}

void CMainDlg::ChessWrapper()
{
	CBlockingSocket sockListen;
	WSADATA wsd;
	if(WSAStartup(0x0101,&wsd)!=0)
	{
		cout << "Unable to start socket\n";
	}
	else
	{
		cout << "Chess Server Demo by Lee Pattersonleepatterson@home.com\n\n";
		cout << "Demonstrates an object oriented server that can handle\n";
		cout << "any number of chess games with 2 players and any number ofwatchers per game.\n\n";
		cout << "Use telnet as a client connect to this server (ie: telnet localhost).\n";
		cout << "Server uses port 23 (telnet default).\n";
		cout << "Ctrl+c exits server"<<endl<<endl<<endl;
		try 
		{
			CSockAddr saServer("192.168.0.56",23);
			sockListen.Create();
			sockListen.Bind(saServer);
			sockListen.Listen();
			_beginthread(AcceptProc,0,&sockListen);
		}
		catch(const char* e)
		{
			sockListen.Cleanup();
			cout << e << "\n";
		}

		ProcessGames();

		try
		{
			sockListen.Close();
			Sleep(300);
			WSACleanup();
		}
		catch(const char* e)
		{
			cout << e << "\n";
		}
	}

	return ;
}
