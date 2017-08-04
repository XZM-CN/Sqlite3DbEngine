//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/08/01  
// ><����ʱ�� ><  2017��:08��:01��   14ʱ:13��:34��  
// ><�ļ�     ><  llist.h  
// ><�ļ�·�� ><  C:\Users\xzm\Downloads\ChessServer  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  https://www.codeproject.com/Articles/59/Multithreaded-TCP-IP-Telnet-Server-Chess-Game-Exam
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  Multithreaded TCP/IP Telnet Server - Chess Game �����߳�TCP / IP Telnet������ - ������Ϸʾ����
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><��Ƽ��� ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////�����޸�///////////////////////////////////////////////////////  
// ><����     ><  xzm  
// ><�޸����� ><  2017��:08��:01��   14ʱ:13��:34��  
// ><ԭ��     ><    
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
