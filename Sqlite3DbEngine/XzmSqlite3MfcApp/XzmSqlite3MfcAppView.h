
// XzmSqlite3MfcAppView.h : CXzmSqlite3MfcAppView ��Ľӿ�
//

#pragma once


class CXzmSqlite3MfcAppView : public CView
{
protected: // �������л�����
	CXzmSqlite3MfcAppView();
	DECLARE_DYNCREATE(CXzmSqlite3MfcAppView)

// ����
public:
	CXzmSqlite3MfcAppDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CXzmSqlite3MfcAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // XzmSqlite3MfcAppView.cpp �еĵ��԰汾
inline CXzmSqlite3MfcAppDoc* CXzmSqlite3MfcAppView::GetDocument() const
   { return reinterpret_cast<CXzmSqlite3MfcAppDoc*>(m_pDocument); }
#endif

