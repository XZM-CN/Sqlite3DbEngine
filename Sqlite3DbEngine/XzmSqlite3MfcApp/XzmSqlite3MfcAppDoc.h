
// XzmSqlite3MfcAppDoc.h : CXzmSqlite3MfcAppDoc ��Ľӿ�
//


#pragma once


class CXzmSqlite3MfcAppDoc : public CDocument
{
protected: // �������л�����
	CXzmSqlite3MfcAppDoc();
	DECLARE_DYNCREATE(CXzmSqlite3MfcAppDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CXzmSqlite3MfcAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
