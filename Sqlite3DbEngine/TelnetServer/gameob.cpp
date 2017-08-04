//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/01  
// ><创建时间 ><  2017年:08月:01日   14时:13分:11秒  
// ><文件     ><  gameob.cpp  
// ><文件路径 ><  C:\Users\xzm\Downloads\ChessServer  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  https://www.codeproject.com/Articles/59/Multithreaded-TCP-IP-Telnet-Server-Chess-Game-Exam
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  Multithreaded TCP/IP Telnet Server - Chess Game （多线程TCP / IP Telnet服务器 - 象棋游戏示例）
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:08月:01日   14时:13分:11秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999 Lee Patterson
// lee@antws.com

#include <winsock.h>
#include <stdio.h>
#include "Xlist.h"
#include "blockingsocket.h"
#include "gameob.h"

//**********
//*
//*	PERSON INSTANCE OBJECT
//*
//**********************
CPersonInstance::CPersonInstance()
{
	m_color=WHITE;
	m_location=CHESS;
	m_channel=0; 
}

CPersonInstance::~CPersonInstance()
{
}

//**********
//*
//*	BOARD SPACE OBJECT
//*
//**********************
CBoardSpace::CBoardSpace()
{
	static int icount=0;
	m_id=icount++;
	m_type=EMPTY;
}

//**********
//*
//*	BOARD GAME OBJECT
//*
//**********************
CBoardGame::CBoardGame()
{
	m_white=NULL;
	m_black=NULL;
	m_nBoardType=BTYPE_UNDEFINED;
	memset(m_gamename,0,sizeof m_gamename);
}

void CBoardGame::AddPerson(CPersonInstance* pInstance)
{
	m_connections.AddTail(pInstance);
}

void CBoardGame::RemovePerson(CPersonInstance* pInstance)
{
	CXList* plist=m_connections.FindItem(pInstance);
	if(plist)
		m_connections.RemoveItem(plist);
}

int CBoardGame::Left(int v)
{
	return (v&0xf0)>>4;
}

int CBoardGame::Right(int v)
{
	return v&0xf;
}

//**********
//*
//*	CHESS BOARD OBJECT
//*
//**********************
ChessBoard::ChessBoard()
{
	FillBoard();
}

bool ChessBoard::ValidMove(int from0,int from1,int to0,int to1)
{
	return true;	//all moves are cool
}

void ChessBoard::Move(int from0,int from1,int to0,int to1)
{

	m_spaces[to1-'0'][to0-'0'].m_type=m_spaces[from1-'0'][from0-'0'].m_type;
	m_spaces[from1-'0'][from0-'0'].m_type=CBoardSpace::EMPTY;
}

void ChessBoard::FillBoard()
{
	int blackbackrow[]={CBoardSpace::BROOK,CBoardSpace::BKNIGHT,CBoardSpace::BBISHOP,CBoardSpace::BQUEEN,CBoardSpace::BKING,CBoardSpace::BBISHOP,CBoardSpace::BKNIGHT,CBoardSpace::BROOK};
	int whitebackrow[]={CBoardSpace::WROOK,CBoardSpace::WKNIGHT,CBoardSpace::WBISHOP,CBoardSpace::WQUEEN,CBoardSpace::WKING,CBoardSpace::WBISHOP,CBoardSpace::WKNIGHT,CBoardSpace::WROOK};
	int i;
	int j;

	//fill back rows
	for(i=0; i<8; i++)
	{
		m_spaces[0][i].m_type=blackbackrow[i];
		m_spaces[7][i].m_type=whitebackrow[i];
	}

	//fill pawn row
	for(i=0; i<8; i++)
	{
		m_spaces[1][i].m_type=CBoardSpace::BPAWN;
		m_spaces[6][i].m_type=CBoardSpace::WPAWN;
	}

	//fill in the rest of the empty spaces
	for(i=2; i<6; i++)
	{
		for(j=0; j<8; j++)
			m_spaces[i][j].m_type=CBoardSpace::EMPTY;
	}
}

void ChessBoard::Draw()
{
	CPersonInstance* pInstance;
	CXList* plist=m_connections.GetHead();
	while(plist)
	{
		pInstance=(CPersonInstance*)plist->GetItem();
		DrawBoard(pInstance);
		plist=plist->GetNext();
	}
}

void ChessBoard::DrawBoard(CPersonInstance* pInstance)
{
	char buffer[80];	//buffer to store a line
	char* p=buffer;
	for(int y=0; y<8; y++)
	{
		sprintf(buffer,"%d ",y); 
		p=buffer+strlen(buffer);
		for(int x=0; x<8; x++)
		{
			*(p++)=GetPiece(m_spaces[y][x].m_type);
			*(p++)=' ';
		}
		*p='\0';
		pInstance->Print(buffer);
	}
	pInstance->Print("  0 1 2 3 4 5 6 7");
}


char ChessBoard::GetPiece(int type)
{
	char id;

	switch(type)
	{

		case CBoardSpace::EMPTY:
			id='.';
			break;

		case CBoardSpace::WPAWN:
			id='p';
			break;

		case CBoardSpace::WBISHOP:
			id='b';
			break;

		case CBoardSpace::WKNIGHT:
			id='n';
			break;

		case CBoardSpace::WROOK:
			id='r';
			break;

		case CBoardSpace::WQUEEN:
			id='q';
			break;

		case CBoardSpace::WKING:
			id='k';
			break;

		case CBoardSpace::BPAWN:
			id='P';
			break;

		case CBoardSpace::BBISHOP:
			id='B';
			break;

		case CBoardSpace::BKNIGHT:
			id='N';
			break;

		case CBoardSpace::BROOK:
			id='R';
			break;

		case CBoardSpace::BQUEEN:
			id='Q';
			break;

		case CBoardSpace::BKING:
			id='K';
			break;

		default:
			id='?';
			break;
	}

	return id;
}

//**********
//*
//*	CHESS GAME OBJECT
//*
//**********************

ChessGameOb::ChessGameOb(CPersonInstance* pInstanceWhite,const char* pGameName)
{
	m_white = pInstanceWhite;
	strcpy(m_gamename,pGameName);
	m_connections.AddTail(pInstanceWhite);
	m_black=NULL;
}

ChessGameOb::~ChessGameOb()
{
	m_white->Cleanup();
	m_black->Cleanup();

	delete m_white;
	delete m_black;
}

void ChessGameOb::AddWatcher(CPersonInstance* pInstance)
{
	m_connections.AddTail(pInstance);
}

void ChessGameOb::AddBlack(CPersonInstance* pInstance)
{
	m_black = pInstance;
	m_connections.AddTail(pInstance);
}

bool ChessGameOb::HasAllPlayers()
{
	if(m_white && m_black)
		return true;

	return false;
}


//**********
//*
//*	CHECKER BOARD OBJECT
//*
//**********************

CheckerBoard::CheckerBoard()
{
	FillBoard();
}

bool CheckerBoard::ValidMove(int from0,int from1,int to0,int to1)
{
	return true;	//all moves are cool
}

void CheckerBoard::Move(int from0,int from1,int to0,int to1)
{

	m_spaces[to1-'0'][to0-'0'].m_type=m_spaces[from1-'0'][from0-'0'].m_type;
	m_spaces[from1-'0'][from0-'0'].m_type=CBoardSpace::EMPTY;
}

void CheckerBoard::FillBoard()
{
	int rows[]=
	{
		CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,
		CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,
		CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,CBoardSpace::WSQUARE,CBoardSpace::BCHECKER,
		
		CBoardSpace::BSQUARE,CBoardSpace::WSQUARE,CBoardSpace::BSQUARE,CBoardSpace::WSQUARE,CBoardSpace::BSQUARE,CBoardSpace::WSQUARE,CBoardSpace::BSQUARE,CBoardSpace::WSQUARE,
		CBoardSpace::WSQUARE,CBoardSpace::BSQUARE,CBoardSpace::WSQUARE,CBoardSpace::BSQUARE,CBoardSpace::WSQUARE,CBoardSpace::BSQUARE,CBoardSpace::WSQUARE,CBoardSpace::BSQUARE,

		CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,
		CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,
		CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,CBoardSpace::WCHECKER,CBoardSpace::WSQUARE,
	};
	int irow=0;
	for(int y=0; y<8; y++)
		for(int x=0; x<8; x++)
			m_spaces[y][x].m_type=rows[irow++];
}

void CheckerBoard::Draw()
{
	CPersonInstance* pInstance;
	CXList* plist=m_connections.GetHead();
	while(plist)
	{
		pInstance=(CPersonInstance*)plist->GetItem();
		DrawBoard(pInstance);
		plist=plist->GetNext();
	}
}

void CheckerBoard::DrawBoard(CPersonInstance* pInstance)
{
	char buffer[80];	//buffer to store a line
	char* p=buffer;
	pInstance->Print("    0   1   2   3   4   5   6   7");
	for(int y=0; y<8; y++)
	{
		pInstance->Print("   --- --- --- --- --- --- --- ---");
		sprintf(buffer,"%d |",y); 
		p=buffer+strlen(buffer);
		for(int x=0; x<8; x++)
		{
			*(p++)=' ';
			*(p++)=GetPiece(m_spaces[y][x].m_type);
			*(p++)=' ';
			*(p++)='|';
		}
		*(p++)=' ';
		*(p++)='0'+y;
		*p='\0';
		pInstance->Print(buffer);
	}
	pInstance->Print("   --- --- --- --- --- --- --- ---");
	pInstance->Print("    0   1   2   3   4   5   6   7");
}

char CheckerBoard::GetPiece(int type)
{
	char id;

	switch(type)
	{
		case CBoardSpace::EMPTYCHECKER:
			id=' ';
			break;

		case CBoardSpace::WSQUARE:
			id=' ';
			break;

		case CBoardSpace::BSQUARE:
			id=' ';
			break;

		case CBoardSpace::WCHECKER :
			id='W';
			break;

		case CBoardSpace::BCHECKER:
			id='b';
			break;

		default: id='?'; break;
	}

	return id;
}

//**********
//*
//*	CHECKER GAME OBJECT
//*
//**********************

CheckerGameOb::CheckerGameOb(CPersonInstance* pInstanceWhite,const char* pGameName)
{
	m_white = pInstanceWhite;
	strcpy(m_gamename,pGameName);
	m_connections.AddTail(pInstanceWhite);
	m_black=NULL;
}

CheckerGameOb::~CheckerGameOb()
{
	m_white->Cleanup();
	m_black->Cleanup();

	delete m_white;
	delete m_black;
}

void CheckerGameOb::AddWatcher(CPersonInstance* pInstance)
{
	m_connections.AddTail(pInstance);
}

void CheckerGameOb::AddBlack(CPersonInstance* pInstance)
{
	m_black = pInstance;
	m_connections.AddTail(pInstance);
}

bool CheckerGameOb::HasAllPlayers()
{
	if(m_white && m_black)
		return true;

	return false;
}
