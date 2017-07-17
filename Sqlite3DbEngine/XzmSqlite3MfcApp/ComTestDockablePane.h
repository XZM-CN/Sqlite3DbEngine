
#pragma once


#include "TreeListCtrl.h"

class CComTestToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CComTestDockablePane : public CDockablePane
{
public:
	CComTestDockablePane();
	virtual ~CComTestDockablePane();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CComTestToolBar m_wndToolBar;
	CTreeListCtrl       m_cTreeList;
	CImageList      m_TestBarImages;
	UINT            m_nCurrSort;
	HTREEITEM       m_hSelItem;

	void FillComTestBar();
	BOOL IsYourChild(CString strParentName, CTreeCtrl &Tree,HTREEITEM &hSelItem );

// ��д
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	// ��ȡѡ��item�ĵ�һ��ֵstrSelCol01�͵ڶ���ֵstrSelCol02
	void GetDataTree(HTREEITEM hSelItem,CString& strSelCol01 ,CString& strSelCol02);
	// ��ȡѡ��item�ĵ�iSelCol�е�ֵ�������ݸ�iSelCol
	void GetDataTree(HTREEITEM hSelItem,UINT iSelCol,CString& strSelCol);
	// ��ȡѡ��item��iSelCol״̬��True����False��������
	BOOL GetDataTree(HTREEITEM hSelItem,UINT iSelCol);
	// �Ҽ�--�ɱ༭ʱ���Ե����˵�
	afx_msg void OnRclickObjectSelected(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	CComPtr <ISqlite3Connect> m_spiSqlite3Connect;
};

