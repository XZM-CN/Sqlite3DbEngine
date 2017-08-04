//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/01  
// ><创建时间 ><  2017年:08月:01日   14时:13分:28秒  
// ><文件     ><  llist.cpp  
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
// ><修改日期 ><  2017年:08月:01日   14时:13分:28秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
//*******************
//*
//* Filename: llist.cpp
//*
//* Notes: 
//*
//*		Linked list class for handing any type of object.
//*
//***************************
//*
//* Date Created: 05/14/98
//* Author: Lee Patterson (lee@antws.com)
//*
//* Copyright (c) 1999, Lee Patterson
//*
//*************************************

#include <stdlib.h>
#include <malloc.h>
#include "Xlist.h"

//uncomment the following line if this will be compiled using a windows mfc application
//used only when debugging
//#define WINDEBUGGING

#ifdef WINDEBUGGING
#include "windows.h"
#include <stdio.h>
#endif

CXList::CXList()
{
	m_iNumEntries = 0;
	m_uniqueID=1;
	m_pHead = NULL;
	m_pTail = NULL;
	m_pNext = NULL;
	m_pPrev = NULL;
}  

CXList::~CXList()
{
	RemoveAll();
}

void CXList::RemoveAll()
{
	if(m_iNumEntries)
	{
		CXList* pList = GetHead();
		CXList* pNext;
		while (pList)
		{
			m_iNumEntries--;
			pNext = pList->m_pNext;
			//if(m_iNumEntries>0) //this line removed as a remove all wouldn't remove the last item.
				delete pList;
			pList = pNext;
		}
		m_pHead=NULL;
		m_pTail=NULL;
		m_iNumEntries=0;
	}
}

void CXList::RemoveAllItems()
{
	if(m_iNumEntries)
	{
		CXList* pList = GetHead();
		CXList* pNext;
		while (pList)
		{
			m_iNumEntries--;
			pNext = pList->m_pNext;
			delete pList->m_pvItem;
			//if(m_iNumEntries>0)
				delete pList;
			pList = pNext;	
		}
		m_pHead=NULL;
		m_pTail=NULL;
		m_iNumEntries=0;
	}
}

void CXList::AddHead (void* pvItem)
{
	CXList *pHead = GetHead ();
	CXList *pItem = new CXList;
#ifdef WINDEBUGGING
	char buf[80];
	sprintf(buf,"CLList::AddHead new %X\n",pItem);
	OutputDebugString(buf);
#endif

	pItem->m_pvItem = pvItem;
	pItem->m_id=m_uniqueID++;
	
	//setup next & prev for new item
	pItem->m_pPrev = NULL;
	pItem->m_pNext = pHead;

	m_pHead = pItem;		//new head item
	if(!m_pTail)
		m_pTail=m_pHead;	//LBP (06/02/99): new item, so tail has to be the same
	
	//setup next & prev for adjacent items

	if(pHead)
		pHead->m_pPrev = pItem;
	
	m_iNumEntries++;
}

void CXList::AddTail (void* pvItem)
{
		CXList *pTail = GetTail ();
		CXList *pItem = new CXList;
#ifdef WINDEBUGGING
	char buf[80];
	sprintf(buf,"CLList::AddTail new %X\n",pItem);
	OutputDebugString(buf);
#endif

		pItem->m_pvItem = pvItem;
		pItem->m_id=m_uniqueID++;
		pItem->m_pPrev = pTail;

		if(!pTail)
			m_pHead=pItem;
		else
			pTail->m_pNext = pItem;

		m_pTail = pItem;
		m_iNumEntries++;
}


void CXList::RemoveItem (CXList* pList)
{
	if(!pList)
		return;

	m_iNumEntries--;
	if(!m_iNumEntries)
	{
		m_pHead=NULL;
		m_pTail=NULL;
		m_pvItem=NULL;
		delete pList;
		return;
	}

	if (pList->m_pPrev)
	{
		pList->m_pPrev->m_pNext = pList->m_pNext;
		if (pList->m_pNext)
			pList->m_pNext->m_pPrev = pList->m_pPrev;
		else
			m_pTail=pList->m_pPrev;		//this was the last item in the list
	}
	else 
	{
		m_pHead=pList->m_pNext;		//this was the first item in the list
		if(pList->m_pNext)
			pList->m_pNext->m_pPrev = NULL;
	}
	delete pList;
}

void CXList::RemoveTail ()
{
	CXList *pList = GetTail ();
	if(pList)
	{
		if(pList->m_pPrev)
			pList->m_pPrev->m_pNext=NULL;
		m_pTail=pList->m_pPrev;
		if(m_pTail==NULL)
			m_pHead=NULL;		//nothing left on the list
		
		delete pList;
		m_iNumEntries--;
	}
}

void CXList::RemoveHead ()
{
	CXList *pList = GetHead ();
	if(pList)
	{
		if(pList->m_pNext)
			pList->m_pNext->m_pPrev=NULL;
		m_pHead=pList->m_pNext;
		if(m_pHead==NULL)
			m_pTail=NULL;		//nothing left on the list

		delete pList;
		m_iNumEntries--;
	}
}

void CXList::InsertAfter (CXList* pList, void* pvItem)
{
	if (!pList->m_pNext)
	{
		AddTail (pvItem);
	}
	else
	{
		CXList* pItem = new CXList;

		pItem->m_pvItem = pvItem;
		pItem->m_id=m_uniqueID++;

		//setup next & prev for new item
		pItem->m_pPrev = pList;
		pItem->m_pNext = pList->m_pNext;
		
		//setup next & prev for adjacent items
		pList->m_pNext->m_pPrev = pItem;
		pList->m_pNext = pItem;

		m_iNumEntries++;
	}
}

void CXList::InsertBefore (CXList* pList, void* pvItem)
{
	if (!pList->m_pPrev)
	{
		AddTail (pvItem);
	}
	else
	{
		CXList* pItem = new CXList;

		pItem->m_pvItem = pvItem;
		pItem->m_id=m_uniqueID++;
		
		//setup next & prev for new item
		pItem->m_pNext = pList;
		pItem->m_pPrev = pList->m_pPrev;
		
		//setup next & prev for adjacent items
		pList->m_pPrev->m_pNext = pItem;
		pList->m_pPrev = pItem;
		
		m_iNumEntries++;
	}
}

CXList* CXList::FindItem(void* pvItem)
{
	CXList* pList = GetHead();
	while(pList)
	{
		if(pList->GetItem()==pvItem)
			return pList;
		pList=pList->GetNext();
	}
	return NULL;
}

CXList* CXList::FindID(int id)
{
	CXList* pList = GetHead();
	while(pList)
	{
		if(pList->GetID()==id)
			return pList;
		pList=pList->GetNext();
	}
	return NULL;
}

CXList* CXList::GetIndex(int iIndex)
{
	CXList* plist=GetHead();
	if(plist)
		for(int i=1; i<iIndex && plist; i++,plist=plist->GetNext()) {}
	return plist;
}
