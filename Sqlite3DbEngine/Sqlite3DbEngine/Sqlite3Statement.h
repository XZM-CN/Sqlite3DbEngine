// Sqlite3Statement.h : CSqlite3Statement ������

#pragma once
#include "resource.h"       // ������



#include "Sqlite3DbEngine_i.h"

#include "../SqliteLib/sqlite3.h"

#define ZORROSOFT_BLOCKWRITE_SIZE      1024*32


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CSqlite3Statement

class CSqlite3Statement
    // : public CComObjectRootEx<CComSingleThreadModel>,
    //   public CComCoClass<CSqlite3Connect, &CLSID_Sqlite3Connect>,
    //   public IDispatchImpl<ISqlite3Connect, &IID_ISqlite3Connect, &LIBID_Sqlite3DbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSqlite3Statement();
	virtual ~CSqlite3Statement();

	typedef enum
	{
		eInteger = SQLITE_INTEGER,
		eFloat   = SQLITE_FLOAT  ,
		eText    = SQLITE_TEXT   ,
		eBlob    = SQLITE_BLOB   ,
		eNull    = SQLITE_NULL   ,
	}EDataType;

	EDataType DataType(int pos_zero_indexed);
	int Fields();
	const char* FieldName(int pos_zero_indexed);
	int FieldType(int pos_zero_indexed);

	int Records();
	int ValueInt(int pos_zero_indexed);
	sqlite3_int64 ValueLong(int pos_zero_indexed);
	double ValueFloat(int pos_zero_indexed);
	CString ValueString(int pos_zero_indexed);
	BYTE* ValueBlob(int pos_zero_indexed,ULONG& nLen);
	int ValueBlobFile(int pos_zero_indexed,LPCTSTR FilePath);

	bool Bind (int pos_zero_indexed, LPCTSTR value);
	bool Bind (int pos_zero_indexed, double value);
	bool Bind (int pos_zero_indexed, int value);
	bool BindBlobData (int pos_zero_indexed,BYTE* pData,ULONG nDataLen);
	bool BindBlobFile (int pos_zero_indexed, LPCTSTR FilePath);
	bool BindNull (int pos_zero_indexed);

	bool Execute();
	bool NextRow();

	// Call Reset if not depending on the NextRow cleaning up.
	// For example for select count(*) statements.

	bool Reset();

	bool IsOK()
	{
		if(NULL != m_stmt)
			return true;
		return false;
	}

private:
	//
	// CSqlite3Statement's ctor only to be called by CSqlite3Wrapper
	//
	friend class CSqlite3Wrapper;

	CSqlite3Statement(LPCTSTR statement, sqlite3* db);

	bool Bind (int pos_zero_indexed, void *pv,ULONG uiLength);

	void *ValueBlob(int pos_zero_indexed);

	sqlite3*		m_db;

	sqlite3_stmt*	m_stmt;

	void*			m_pBindValue;

	CString			m_strSql;
};