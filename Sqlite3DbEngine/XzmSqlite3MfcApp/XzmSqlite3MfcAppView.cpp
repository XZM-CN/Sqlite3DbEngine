
// XzmSqlite3MfcAppView.cpp : CXzmSqlite3MfcAppView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CXzmSqlite3MfcAppView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CXzmSqlite3MfcAppView 构造/析构

CXzmSqlite3MfcAppView::CXzmSqlite3MfcAppView()
{
	// TODO: 在此处添加构造代码

}

CXzmSqlite3MfcAppView::~CXzmSqlite3MfcAppView()
{
}

BOOL CXzmSqlite3MfcAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CXzmSqlite3MfcAppView 绘制

void CXzmSqlite3MfcAppView::OnDraw(CDC* /*pDC*/)
{
	CXzmSqlite3MfcAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CXzmSqlite3MfcAppView 打印


void CXzmSqlite3MfcAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CXzmSqlite3MfcAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CXzmSqlite3MfcAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CXzmSqlite3MfcAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CXzmSqlite3MfcAppView 诊断

#ifdef _DEBUG
void CXzmSqlite3MfcAppView::AssertValid() const
{
	CView::AssertValid();
}

void CXzmSqlite3MfcAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXzmSqlite3MfcAppDoc* CXzmSqlite3MfcAppView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXzmSqlite3MfcAppDoc)));
	return (CXzmSqlite3MfcAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CXzmSqlite3MfcAppView 消息处理程序
