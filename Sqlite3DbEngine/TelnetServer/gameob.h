//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/01  
// ><创建时间 ><  2017年:08月:01日   14时:13分:19秒  
// ><文件     ><  gameob.h  
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
// ><修改日期 ><  2017年:08月:01日   14时:13分:19秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999 Lee Patterson
// lee@antws.com

#include "TelnetSocket.h"
class CPersonInstance : public CTelnetSocket
{
public:
	CPersonInstance();
	~CPersonInstance();

public:
	enum Color {WHITE,BLACK};
	enum Location {LOBBY,CHESS,CHECKERS};

protected:
	int m_color;
	int m_location;
	int m_channel;
};


class CBoardSpace
{
public:
	CBoardSpace();

public:
	enum { EMPTY,WPAWN,WKNIGHT,WBISHOP,WROOK,WQUEEN,WKING,BPAWN,BKNIGHT,BBISHOP,BROOK,BQUEEN,BKING };
	enum { EMPTYCHECKER, WSQUARE, BSQUARE, WCHECKER, BCHECKER };

		
public:
	int m_id;
	int m_type;
};

class CBoardGame
{
public:
	CBoardGame();
	void AddPerson(CPersonInstance* pInstance);
	void RemovePerson(CPersonInstance* pInstance);
	int Left(int v);
	int Right(int v);
	UINT Type() {return m_nBoardType;}
	virtual bool HasAllPlayers()=0;
	virtual void AddBlack(CPersonInstance* pInstance)=0;
	virtual void AddWatcher(CPersonInstance* pInstance)=0;

	//functions that must be defined in derived class
	virtual bool ValidMove(int from0,int from1,int to0,int to1)=0;
	virtual void Move(int from0,int from1,int to0,int to1)=0;
	virtual void Draw()=0;			//draw the board on all connections to this board game
	CXList m_connections;			//list of PersonInstances
	CPersonInstance* m_white;
	CPersonInstance* m_black;
	char m_gamename[30];

	enum {BTYPE_UNDEFINED, BTYPE_CHECKERS, BTYPE_CHESS};
protected:
	CBoardSpace m_spaces[8][8];
	UINT m_nBoardType;
};

class ChessBoard : public CBoardGame
{
public:
	ChessBoard();
	 bool ValidMove(int from0,int from1,int to0,int to1);
	virtual void Move(int from0,int from1,int to0,int to1);
	virtual void FillBoard();
	virtual void Draw();
	virtual bool HasAllPlayers()=0;
	virtual void AddBlack(CPersonInstance* pInstance)=0;
	virtual void AddWatcher(CPersonInstance* pInstance)=0;

protected:
	void DrawBoard(CPersonInstance* psocket);
	char GetPiece(int type);
};


class ChessGameOb : public ChessBoard
{
public:
	ChessGameOb(CPersonInstance* pInstanceWhite,const char* pGameName);
	~ChessGameOb();
	void AddWatcher(CPersonInstance* pInstance);
	void AddBlack(CPersonInstance* pInstance);
	bool HasAllPlayers();

public:
	CXList m_watchers;
	WORD m_wFlags;
	enum tagFlags {WAITINGFORPLAYER,INPROGRESS};

};

class CheckerBoard : public CBoardGame
{
public: 
	CheckerBoard();
	 bool ValidMove(int from0,int from1,int to0,int to1);
	virtual void Move(int from0,int from1,int to0,int to1);
	virtual void FillBoard();
	virtual void Draw();
	virtual bool HasAllPlayers()=0;
	virtual void AddBlack(CPersonInstance* pInstance)=0;
	virtual void AddWatcher(CPersonInstance* pInstance)=0;

protected:
	void DrawBoard(CPersonInstance* psocket);
	char GetPiece(int type);
};

class CheckerGameOb : public CheckerBoard
{
public:
	CheckerGameOb(CPersonInstance* pInstanceWhite,const char* pGameName);
	~CheckerGameOb();
	void AddWatcher(CPersonInstance* pInstance);
	void AddBlack(CPersonInstance* pInstance);
	bool HasAllPlayers();
	bool HasBothPlayers();

public:
	CXList m_watchers;
	WORD m_wFlags;
	enum tagFlags {WAITINGFORPLAYER,INPROGRESS};
};
