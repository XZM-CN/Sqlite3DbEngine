#pragma once
#include "json.h"
#include "BaseFuncLib.h"
#include "JsonEngine_i.h"

/**
*
*
* @Function List 
*
* @author WZH
*
* @Date 2011-09-25
*
*
* @brief json������װ��
*
* @ingroup WeiboEngine
**/

class CJsonParser
{
public:
	CJsonParser(Json::Value* pRoot = NULL);
	~CJsonParser();

	void PutCodingType(ECodingType eCodingType)
	{
		m_eCodingType = eCodingType;
	}

	ECodingType GetCodingType()
	{
		return m_eCodingType;
	}

	bool GetValue(VARIANT *pVal);

	ULONG GetArraySize();

	bool IsMember(const CString& strKeyName);

	bool SetIntValue(const CString& strKeyName, LONGLONG lnVal);
	bool SetStringValue(const CString& strKeyName, const CString& strValue);
	bool SetValue(const CString& strKeyName, CComVariant& varValue);

	CJsonParser* CreateChildJson(const CString& strKeyName);

	/// ���浽ָ���ļ�
	BOOL SaveToFile(const CString& strJsonFile);

	/// ����json�ַ���
	/*
	@param	pJsonContent json�ַ���������Ϊutf8
	*/
	bool Parse(const char* pJsonContent);

	/// ����json�ļ�
	/*
	@param	strJsonFile
	*/
	bool ParseFile(const CString& strJsonFile);

	/// ��õ�ǰ�ڵ�����
	Json::ValueType GetType();

	/// ����ӽڵ�
	Json::ValueType GetChildTypeByName(const CString& strName);

	CString GetStringChildByName(const CString& strName);

	bool GetBoolChildByName(const CString& strName);

	int GetIntChildByName(const CString& strName);

	unsigned long long GetUIntChildByName(const CString& strName);

	long long GetLongLongChildByName(const CString& strName);

	double GetDoubleChildByName(const CString& strName);

	bool GetVariantChildByName(const CString& strName,CComVariant& varVal);
	bool GetVariantChildByKey(char *szName,CComVariant& varVal);

	bool GetArrayValue(short sIndex,CComBSTR& bstrKeyName,CComVariant& varVal);

	/// ���ݽڵ����ƻ�ȡ�ӽڵ�
	/*
	@param	nIndex �ӽڵ����
	*/
	CJsonParser* GetChildJson(const CString& strName);

	/// ������Ż�ȡ�ӽڵ�
	/*
	@param	nIndex �ӽڵ����
	*/
	CJsonParser* GetChildJson(const int nIndex);

	/// �����ӽڵ�
	bool SetChild(const CString& strName,const CString& strValue);

	std::vector<std::string> GetChildNames();

	HRESULT GetObjectString(BSTR* pVal);

	/// ��ô�����Ϣ
	CString GetErrInfo()
	{
		return m_strErrInfo;
	}

	BOOL CreateJsonDemoToFile(const CString& strJsonFile);
protected:

	/// �Ƿ��ڴ�
	void Release();

	bool GetBoolChildByName(const char* pName);

	/// �������ֻ�ȡ�ӽڵ�����ֵ
	/*
	@param	pName���ӽڵ�����
	*/
	int	GetIntChildByName(const char* pName);

	/// �������ֻ�ȡ�ӽ��ַ���
	/*
	@param	pName �ӽڵ�����
	*/
	/*����ֵ������ͷ�*/
	char* GetStringChildByName(const char* pName);

	/// �������ֻ�ȡ�ӽڵ�Ƿ�������ֵ
	/*
	@param	pName	in���ӽڵ�����
	*/
	unsigned long long GetUintChildByName(const char* pName);

	/// �������ֻ�ȡ�ӽڵ�64λ�Ƿ�������ֵ
	/*
	@param	pName	in���ӽڵ�����
	*/
	long long GetInt64ChildByName(const char* pName);
	/// ��ȡ�ӽڵ㸡��
	double GetDoubleChildByName(const char* pName);

	/// ��ýڵ�����
	Json::ValueType GetChildTypeByName(const char* pName);

	/// ������Ż�ȡ�ӽڵ��ַ���
	/*
	@param	nIndex �ӽڵ����
	*/
	/*����ֵ���ͷ�*/
	char* GetStringChildByName(const int nIndex);

	/// ������Ż�ȡ�ӽڵ㲼��ֵ
	/*
	@param	nIndex	in���ӽڵ����
	*/
	bool GetBoolChildByIndex(const int nIndex);

	/// ������Ż�ȡ�ӽڵ�����ֵ
	/*
	@param	nIndex	in���ӽڵ����
	*/
	int	GetIntChildByIndex(const int nIndex);

	/// ������Ż�ȡ�ӽڵ�Ƿ�������ֵ
	/*
	@param	nIndex���ӽڵ����
	*/
	unsigned long long GetUintChildByIndex(const int nIndex);

	/// ������Ż�ȡ�ӽڵ�64λ�Ƿ�������ֵ
	/*
	@param	nIndex���ӽڵ����
	*/
	long long GetInt64ChildByIndex(const int nIndex);

	/// ������Ż���ӽڵ�
	Json::Value* GetChildNodeByIndex(const int nIndex);

	/// �������ֻ�ȡ�ӽڵ�
	/*
	@param	pName���ӽڵ�����
	*/
	CJsonParser* GetChildJson(const char* pName); 

	/// ����ӽڵ�
	Json::Value* GetChildNode(const char* pName);

	/// �����ӽڵ�����
	bool SetChild(const char* pName,const char* pValue);

	/// ���ֻ��������
	static CJsonParser* GetReadReader(Json::Value* pRoot);

protected:	
	///  json�ļ����ڵ�
	Json::Value* m_pRoot;

	/// ��ʱ����
	Json::Value m_TempValue;

	///  �Ƿ���Ҫ�Լ������ڴ��ͷ�
	bool m_bDeleteSelf;

	/// ��������
	ECodingType	m_eCodingType;

	/// ��������
	CString	m_strErrInfo;
};