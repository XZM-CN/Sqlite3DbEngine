
// XzmSqlite3MfcAppView.cpp : CXzmSqlite3MfcAppView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "XzmSqlite3MfcApp.h"
#endif

#include "XzmSqlite3MfcAppDoc.h"
#include "XzmSqlite3MfcAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXzmSqlite3MfcAppView

IMPLEMENT_DYNCREATE(CXzmSqlite3MfcAppView, CView)

BEGIN_MESSAGE_MAP(CXzmSqlite3MfcAppView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CXzmSqlite3MfcAppView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CXzmSqlite3MfcAppView ����/����

CXzmSqlite3MfcAppView::CXzmSqlite3MfcAppView()
{
	// TODO: �ڴ˴���ӹ������

}

CXzmSqlite3MfcAppView::~CXzmSqlite3MfcAppView()
{
}

BOOL CXzmSqlite3MfcAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CXzmSqlite3MfcAppView ����

void CXzmSqlite3MfcAppView::OnDraw(CDC* /*pDC*/)
{
	CXzmSqlite3MfcAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CXzmSqlite3MfcAppView ��ӡ


void CXzmSqlite3MfcAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CXzmSqlite3MfcAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CXzmSqlite3MfcAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CXzmSqlite3MfcAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CXzmSqlite3MfcAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CXzmSqlite3MfcAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CXzmSqlite3MfcAppView ���

#ifdef _DEBUG
void CXzmSqlite3MfcAppView::AssertValid() const
{
	CView::AssertValid();
}

void CXzmSqlite3MfcAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXzmSqlite3MfcAppDoc* CXzmSqlite3MfcAppView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXzmSqlite3MfcAppDoc)));
	return (CXzmSqlite3MfcAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CXzmSqlite3MfcAppView ��Ϣ�������
