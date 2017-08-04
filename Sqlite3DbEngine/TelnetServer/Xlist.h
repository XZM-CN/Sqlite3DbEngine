//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/01  
// ><创建时间 ><  2017年:08月:01日   14时:13分:34秒  
// ><文件     ><  llist.h  
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
// ><修改日期 ><  2017年:08月:01日   14时:13分:34秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef LLIST_H
#define LLIST_H

//*************************************
//*
//* Notes: 
//*
//*		Linked list class for handing any type of object.
//*
//***************************
//*
//* Date Created: 05/14/98
//* Author: Lee Patterson
//*
//* Copyright (c) 1999, Lee Patterson (lee@antws.com)
//*
//*************************************

class CXList
{
public:
	CXList ();
	~CXList ();

	//adding and removing from the list
	void AddTail (void*);
	void AddHead (void*);
	void InsertAfter (CXList* pList, void* pvItem);
	void InsertBefore (CXList* pList, void* pvItem);
	void RemoveItem (CXList*);
	void RemoveHead ();
	void RemoveTail ();
	void RemoveAll();
	void RemoveAllItems();

	//retrieving items
	int NumEntries(){return m_iNumEntries;}
	void* GetItem (CXList* pList) {return pList->m_pvItem;}
	void* GetItem () {return m_pvItem;}
	int GetID () {return m_id;}
	CXList* GetHead () {return m_pHead;}
	CXList* GetTail () {return m_pTail;}
	CXList* GetNext (CXList* pList) {return pList->m_pNext;}
	CXList* GetPrev (CXList* pList) {return pList->m_pPrev;}
	CXList* GetNext () {return m_pNext;}
	CXList* GetPrev () {return m_pPrev;}

	//finding items
	CXList* FindItem(void* pvItem);
	CXList* FindID(int id);
	CXList* GetIndex(int iIndex);

protected:
	int m_iNumEntries;
	int m_uniqueID;
	int m_id;

	void* m_pvItem;

	CXList* m_pHead;
	CXList* m_pTail;
	CXList* m_pPrev;
	CXList* m_pNext;
};

#endif
