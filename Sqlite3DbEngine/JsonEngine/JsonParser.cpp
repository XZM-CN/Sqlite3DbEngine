#include "stdafx.h"
#include "JsonParser.h"

#include <iostream>
#include <fstream> // ifstream ofstream 头文件
using namespace std;

#ifndef WIN64
#ifdef _DEBUG
#pragma comment(lib, "..\\bin\\Debug\\JsonLib.lib")
#else
#pragma comment(lib, "..\\bin\\Release\\JsonLib.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "..\\bin\\Debug\\X64\\JsonLib.lib")
#else
#pragma comment(lib, "..\\bin\\Release\\X64\\JsonLib.lib")
#endif
#endif

CJsonParser::CJsonParser(Json::Value* pRoot)
	:m_pRoot(pRoot)
	,m_strErrInfo(_T(""))
	,m_eCodingType(CODINGTYPE_UTF8)
{
	if(NULL == m_pRoot)
		m_bDeleteSelf = false;
	else
		m_bDeleteSelf = true;
}

CJsonParser::~CJsonParser()
{
	Release();
}

void CJsonParser::Release()
{
	m_strErrInfo.Empty();
	if(m_bDeleteSelf && NULL != m_pRoot)
		delete m_pRoot;
	m_pRoot = NULL;
	m_bDeleteSelf = false;
}

bool CJsonParser::SetIntValue(const CString& strKeyName, LONGLONG lnVal)
{
	bool bSaveFlag = false;
	if(NULL == m_pRoot)
		m_pRoot = new Json::Value();
	if(NULL == m_pRoot)
		return bSaveFlag;
	char *szName = NULL;
	CBaseFuncLib::US2ToUtf8(strKeyName,&szName);
	if(NULL == szName)
		return bSaveFlag;
	(*m_pRoot)[szName] = Json::Value(lnVal);
	delete []szName;
	szName = NULL;
	bSaveFlag = true;
	return bSaveFlag;
}

bool CJsonParser::SetStringValue(const CString& strKeyName, const CString& strValue)
{
	bool bSaveFlag = false;
	if(NULL == m_pRoot)
		m_pRoot = new Json::Value();
	if(NULL == m_pRoot)
		return bSaveFlag;
	char *szName = NULL,*szValue = NULL;
	CBaseFuncLib::US2ToUtf8(strKeyName,&szName);
	CBaseFuncLib::US2ToUtf8(strValue,&szValue);
	if(NULL == szName || NULL == szValue)
		return bSaveFlag;
	(*m_pRoot)[szName] = Json::Value(szValue);
	delete []szName;
	szName = NULL;
	delete []szValue;
	szValue = NULL;

	bSaveFlag = true;
	return bSaveFlag;
}

bool CJsonParser::SetValue(const CString& strKeyName, CComVariant& varValue)
{
	bool bSaveFlag = false;
	if(NULL == m_pRoot)
		m_pRoot = new Json::Value();
	if(NULL == m_pRoot)
		return bSaveFlag;
	char *szName = NULL;
	CBaseFuncLib::US2ToUtf8(strKeyName,&szName);
	if(NULL == szName)
		return bSaveFlag;
	switch(varValue.vt)
	{
	case VT_BSTR:
		{
			char *szValue = NULL;
			CBaseFuncLib::US2ToUtf8(varValue.bstrVal,&szValue);
			if(NULL == szValue)
				break;
			(*m_pRoot)[szName] = Json::Value(szValue);
			delete []szValue;
			szValue = NULL;
			bSaveFlag = true;
		}
		break;
	case VT_BOOL:
	case VT_I1:
	case VT_I2:
	case VT_I4:
	case VT_INT:
	case VT_I8:
		{
			varValue.ChangeType(VT_I8);
			(*m_pRoot)[szName] = Json::Value(varValue.llVal);
			bSaveFlag = true;
		}
		break;
	case VT_UI1:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
	case VT_UINT:
		{
			varValue.ChangeType(VT_UI8);
			(*m_pRoot)[szName] = Json::Value(varValue.ullVal);
			bSaveFlag = true;
		}
		break;
	case VT_R4:
	case VT_R8:
	case VT_DATE:
		{
			varValue.ChangeType(VT_R8);
			(*m_pRoot)[szName] = Json::Value(varValue.fltVal);
			bSaveFlag = true;
		}
		break;
	default:
		ATLASSERT(0);
		break;
	}

	delete []szName;
	szName = NULL;
	return bSaveFlag;
}

CJsonParser* CJsonParser::CreateChildJson(const CString& strKeyName)
{
	CJsonParser* pParser = NULL;
	char *szName = NULL;
	CBaseFuncLib::US2ToUtf8(strKeyName,&szName);
	if(NULL == szName)
		return pParser;
	Json::Value defJson(Json::nullValue);
	Json::Value *pVal = new Json::Value(defJson);
	pParser = new CJsonParser(pVal);
	delete []szName;
	szName = NULL;

	ATLASSERT(pParser);
	return pParser;
}

HRESULT CJsonParser::GetObjectString(BSTR* pVal)
{
	HRESULT hRet(E_FAIL);
	if(NULL == m_pRoot)
		return hRet;
	Json::StyledWriter writer;
	std::string output = writer.write((*m_pRoot));
	if(CODINGTYPE_US2 == m_eCodingType)
	{
		WCHAR *szContent = NULL;
		CBaseFuncLib::Utf8ToUS2(output.c_str(),&szContent);
		if(NULL == szContent)
			return hRet;
		CComBSTR bstrVal(szContent);
		delete []szContent;
		szContent = NULL;
		output.empty();
		bstrVal.CopyTo(pVal);
		return S_OK;
	}
	if(CODINGTYPE_ANSI == m_eCodingType)
	{
		char *szContent = NULL;
		CBaseFuncLib::Utf8ToChar(output.c_str(),&szContent);
		if(NULL == szContent)
			return hRet;
		CComBSTR bstrVal(szContent);
		delete []szContent;
		szContent = NULL;
		output.empty();
		bstrVal.CopyTo(pVal);
		return S_OK;
	}
	char *szContent = (char *)output.c_str();
	if(NULL == szContent)
		return E_OUTOFMEMORY;
	CComBSTR bstrVal(szContent);
	output.empty();
	bstrVal.CopyTo(pVal);
	return S_OK;
}

BOOL CJsonParser::SaveToFile(const CString& strJsonFile)
{
	BOOL bSaveFlag = FALSE;
	if(NULL == m_pRoot)
		return bSaveFlag;
	Json::StyledWriter writer;
	std::string output = writer.write((*m_pRoot));
	if(CODINGTYPE_US2 == m_eCodingType)
	{
		WCHAR *szContent = NULL;
		int nLen = CBaseFuncLib::Utf8ToUS2(output.c_str(),&szContent);
		if(NULL != szContent)
		{
			bSaveFlag = CBaseFuncLib::WriteToFile(strJsonFile,(BYTE *)szContent,2*(nLen-1));
			delete []szContent;
			szContent = NULL;
		}
		output.empty();
		return bSaveFlag;
	}
	if(CODINGTYPE_ANSI == m_eCodingType)
	{
		char *szContent = NULL;
		int nLen = CBaseFuncLib::Utf8ToChar(output.c_str(),&szContent);
		if(NULL != szContent)
		{
			bSaveFlag = CBaseFuncLib::WriteToFile(strJsonFile,(BYTE *)szContent,2*(nLen-1));
			delete []szContent;
			szContent = NULL;
		}
		output.empty();
		return bSaveFlag;
	}
	char *szContent = (char *)output.c_str();
	if(NULL != szContent)
		bSaveFlag = CBaseFuncLib::WriteToFile(strJsonFile,(BYTE *)szContent,(DWORD)strlen(szContent));
	output.empty();
	return bSaveFlag;
}

bool CJsonParser::ParseFile(const CString& strJsonFile)
{
	if(strJsonFile.IsEmpty() || !CBaseFuncLib::IsPathExist(strJsonFile))
	{
		ATLASSERT(0);
		return false;
	}
	ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strJsonFile);
	ATLASSERT(dwFileSize);
	if(!dwFileSize)
		return false;
	Release();
	BYTE *pBuf = NULL;
	int nLength = CBaseFuncLib::ReadAllData(strJsonFile,&pBuf);
	if(NULL == pBuf)
	{
		ATLASSERT(0);
		return false;
	}
	bool bParseFlag = false;
	if(CODINGTYPE_US2 == m_eCodingType)
	{
		char *szContent = NULL;
		CBaseFuncLib::US2ToUtf8(CString(pBuf),&szContent);
		if(NULL != pBuf)
		{	
			delete []pBuf;
			pBuf = NULL;
		}
		bParseFlag = Parse(szContent);
		if(NULL != szContent)
		{	
			delete []szContent;
			szContent = NULL;
		}
	}
	else
		bParseFlag = Parse((const char*)pBuf);
	if(!bParseFlag)
	{
#ifdef _DEBUG
		if(CODINGTYPE_UTF8 == m_eCodingType)
		{
			/// 转换到US2编码查看
			char *szContent = NULL;
			int nLen = CBaseFuncLib::Utf8ToChar((const char *)pBuf,&szContent);
			if(NULL != szContent)
			{
				CBaseFuncLib::WriteToFile(strJsonFile+_T(".json"),(BYTE *)szContent,nLen-1);
				delete []szContent;
				szContent = NULL;
			}
		}
#endif
	}
	if(NULL != pBuf)
	{	
		delete []pBuf;
		pBuf = NULL;
	}
	return bParseFlag;
}

bool CJsonParser::Parse(const char* pJsonContent)
{
	m_strErrInfo.Empty();
	if(NULL == pJsonContent)
	{
		ATLASSERT(0);
		return false;
	}
	Release();

	Json::Reader reader;
	Json::Value  *pRoot = new Json::Value();

	if( !reader.parse(string(pJsonContent),(*pRoot),false) )
	{
		ATLTRACE(_T("JSON内容解析失败："));
		m_strErrInfo = reader.getFormattedErrorMessages().c_str();
		m_strErrInfo.Replace(_T("\""),_T("“"));
		ATLTRACE(m_strErrInfo);
		ATLTRACE(_T("\r\n"));
		delete pRoot;
		pRoot = NULL;
		return false;
	}

	m_pRoot = pRoot;
	m_bDeleteSelf = true;
	return true;
}

void CJsonParser::TraversalJsonPrint2Console(const char* pJsonContent)
{
	Json::Reader reader;
	Json::Value  Root;

	ifstream in(pJsonContent, ios::in|ios::binary);  

	if( !reader.parse(in,Root,false) )
	{
		ATLTRACE(_T("JSON内容解析失败："));
		// 1.可能是本身Json的格式不对
		// 2.格式正确，可能是字符集编码问题，导致格式反倒不正确了
		m_strErrInfo = reader.getFormattedErrorMessages().c_str();
		m_strErrInfo.Replace(_T("\""),_T("“"));
		ATLTRACE(m_strErrInfo);
		ATLTRACE(_T("\r\n"));
		return ;
	}

	print(Root);
	cout<<"解析结束"<<endl;
}

void CJsonParser::TraversalJsonPrint2Console(const CString& strJsonFile)
{
	if(strJsonFile.IsEmpty() || !CBaseFuncLib::IsPathExist(strJsonFile))
	{
		ATLASSERT(0);
		return ;
	}
	ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strJsonFile);
	ATLASSERT(dwFileSize);
	if(!dwFileSize)
		return ;
	Release();
	BYTE *pBuf = NULL;
	int nLength = CBaseFuncLib::ReadAllData(strJsonFile,&pBuf);
	if(NULL == pBuf)
	{
		ATLASSERT(0);
		return ;
	}
	bool bParseFlag = false;
	if(CODINGTYPE_US2 == m_eCodingType)
	{
		char *szContent = NULL;
		CBaseFuncLib::US2ToUtf8(CString(pBuf),&szContent);
		if(NULL != pBuf)
		{	
			delete []pBuf;
			pBuf = NULL;
		}
		bParseFlag = Parse(szContent);
		if(NULL != szContent)
		{	
			delete []szContent;
			szContent = NULL;
		}
	}
	else
		bParseFlag = Parse((const char*)pBuf);
}

void CJsonParser::print(Json::Value v,unsigned int indent)
{
	unsigned int indentTemp = 0;
	if (indentTemp<indent)
	{
		indentTemp = indent;
	}

	if(indentTemp == 0){
		cout<<"{"<<endl;
	}
	Json::Value::Members mem = v.getMemberNames();
	for (auto iter = mem.begin(); iter != mem.end(); iter++)
	{
		if (v[*iter].type() == Json::objectValue)
		{
			for (unsigned int j=0;j<indentTemp;j++)
			{
				cout<<"\t"<<endl;
			}

			cout<<*iter<<" :{ "<<endl;
			print(v[*iter],indentTemp+1);

			for (unsigned int j=0;j<indentTemp;j++)
			{
				cout<<"\t}"<<endl;
			}
		}
		else if (v[*iter].type() == Json::arrayValue) // 数组类型
		{
			unsigned int temp = indentTemp +1;
			for (unsigned int j=0;j<temp;j++)
			{
				cout<<"\t";
			}

			cout<<*iter<<" : "<<"["<<endl;
			unsigned int cnt = v[*iter].size();
			for (unsigned int i = 0; i < cnt; i++)
			{
				switch (v[*iter][i].type())
				{
				case Json::objectValue:
					{
						print(v[*iter][i],temp+1);
					}
					break;
				case Json::arrayValue:
					{
						cout<<endl;
						print(v[*iter][i],temp+1);
					}
					break;
				case Json::stringValue:
					{
						for (unsigned int j=0;j<temp+1;j++)
						{
							cout<<"\t";
						}
						cout<<v[*iter][i].asString()<<","<<endl;
					}
					break;
				case Json::realValue:
					{
						for (unsigned int j=0;j<temp+1;j++)
						{
							cout<<"\t";
						}
						cout<<v[*iter][i].asDouble()<<","<<endl;
					}
					break;
				case Json::uintValue:
					{
						for (unsigned int j=0;j<temp+1;j++)
						{
							cout<<"\t";
						}
						cout<<v[*iter][i].asUInt()<<","<<endl;
					}
					break;
				default :
					{
						for (unsigned int j=0;j<temp+1;j++)
						{
							cout<<"\t";
						}
						cout<<v[*iter][i].asInt()<<","<<endl;
					}
					break;
				}

				for (unsigned int j=0;j<temp;j++)
				{
					cout<<"\t]"<<endl;
				}
			}


			for (unsigned int j=0;j<indentTemp;j++)
			{
				cout<<"\t"<<endl;
			}
			cout<<endl<<"]"<<endl;
		}
		else if (v[*iter].type() == Json::stringValue)
		{
			unsigned int temp = indentTemp +1;
			for (unsigned int j=0;j<temp;j++)
			{
				cout<<"\t";
			}
			cout<<*iter<<" : "<<v[*iter].asString()<<endl;
		}
		else if (v[*iter].type() == Json::realValue)
		{
			unsigned int temp = indentTemp +1;
			for (unsigned int j=0;j<temp;j++)
			{
				cout<<"\t";
			}
			cout<<*iter<<" : "<<v[*iter].asDouble()<<endl;
		}
		else if (v[*iter].type() == Json::uintValue)
		{
			unsigned int temp = indentTemp +1;
			for (unsigned int j=0;j<temp;j++)
			{
				cout<<"\t";
			}
			cout<<*iter<<" : "<<v[*iter].asUInt()<<endl;
		}
		else
		{
			unsigned int temp = indentTemp +1;

			for (unsigned int j=0;j<temp;j++)
			{
				cout<<"\t";
			}
			cout<<*iter<<" : "<<v[*iter].asInt()<<endl;
		}
	}

	if(indentTemp == 0){
		cout<<"}"<<endl;
	}
	return;
}

bool CJsonParser::IsMember(const CString& strKeyName)
{
	bool bExistFlag = false;
	if(strKeyName.IsEmpty() || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return bExistFlag;
	}
	char *szBuf = NULL;
	CBaseFuncLib::US2ToUtf8(strKeyName,&szBuf);
	if(NULL == szBuf)
		return bExistFlag;
	bExistFlag = m_pRoot->isMember(szBuf);
	delete []szBuf;
	szBuf = NULL;
	return bExistFlag;
}

ULONG CJsonParser::GetArraySize()
{
	ULONG ulSize = 0;
	if(NULL == m_pRoot)
	{
		ATLASSERT(0);
		return ulSize;
	}
	Json::ValueType vType = m_pRoot->type();
	if(Json::objectValue == vType || Json::arrayValue == vType)
		ulSize = m_pRoot->size();
	return ulSize;
}

void CJsonParser::OutputStyledJson(void)
{
	cout << m_pRoot->toStyledString().c_str() << endl << endl;
}

bool CJsonParser::SetObjAsChild(const char* pName,const char* pValue)
{
	if(NULL == pName)
	{
		ATLASSERT(0);
		return false;
	}
	if(NULL == m_pRoot)
		m_pRoot = new Json::Value();
	if(NULL == m_pRoot)
		return false;
	if(Json::nullValue != m_pRoot->type() 
		&& Json::objectValue != m_pRoot->type()
		&& Json::arrayValue != m_pRoot->type())
		return false;

	Json::Value  *pRoot = new Json::Value();

	Json::Reader reader;
	if( !reader.parse(string(pValue),(*pRoot),false) )
	{
		ATLTRACE(_T("JSON内容解析失败\r\n"));
		delete pRoot;
		pRoot = NULL;
		return false;
	}
	cout << "子节点Json"<< endl ;
	cout << pRoot->toStyledString().c_str() << endl << endl;
	cout << "父节点Json" << endl ;
	cout << m_pRoot->toStyledString().c_str() << endl << endl;

	/// 判断是否重复
	if(Json::nullValue == m_pRoot->type() || Json::objectValue == m_pRoot->type())
	{
		if(m_pRoot->isMember(pName))
		{
			m_pRoot->removeMember(pName);
			cout << "父Json中出现了相同的节点，删除之" << endl ;
		}

		(*m_pRoot)[pName] = *pRoot;

		cout << "追加节点以后Json, " << __FILE__<< ","<< __LINE__<< ","<< __TIME__<< endl ;
		cout << m_pRoot->toStyledString().c_str() << endl << endl;

		Json::StyledWriter writer;
		std::string output = writer.write((*m_pRoot));
		if(CODINGTYPE_US2 == m_eCodingType)
		{
			WCHAR *szContent = NULL;
			int nLen = CBaseFuncLib::Utf8ToUS2(output.c_str(),&szContent);
			if(NULL != szContent)
			{
				CString str = CBaseFuncLib::GetModulePath();
				str = str + _T("Log.txt");
				BOOL bSaveFlag = CBaseFuncLib::WriteToFile(str,(BYTE *)szContent,2*(nLen-1));
				delete []szContent;
				szContent = NULL;
			}
			output.empty();
		}
		else{
		}


		
		return true;
	}

	if(Json::arrayValue == m_pRoot->type())
	{
		Json::ArrayIndex nIndex = -1;
		Json::ArrayIndex size = m_pRoot->size();
		for(Json::ArrayIndex index = 0; index < size; index++)
		{
			if(Json::arrayValue != m_pRoot[index].type())
				continue;
			Json::Value JsonVal = m_pRoot[index];
			Json::ArrayIndex ChildSize = JsonVal.size();
			for(Json::ArrayIndex i = 0; i < ChildSize; i++)
			{
				Json::Value val = JsonVal[i];
				if(val.isMember(pName))
				{
					nIndex = index;
					break;
				}
			}
			if(-1 != nIndex)
				break;
		}
		if(-1 == nIndex)
			m_pRoot->append((*pRoot));
		else
		{
			m_pRoot[nIndex] = (*pRoot);
		}
	}
	return true;
}

CJsonParser* CJsonParser::GetChildJson(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return NULL;
	}
	Json::Value *pChild = NULL;
	if(Json::objectValue != m_pRoot->type() && Json::arrayValue != m_pRoot->type())
		return NULL;
	if(Json::objectValue == m_pRoot->type())
	{
		Json::Value* temVal = (&(*m_pRoot)[pName]);
		if(NULL != temVal && Json::nullValue != temVal->type())
			pChild = temVal;
		else
			m_pRoot->removeMember(pName);
	}
	if(NULL == pChild)
		return NULL;
	return GetReadReader(pChild);
}

Json::Value* CJsonParser::GetChildNode(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return NULL;
	}
	Json::Value *pChild = NULL;
	if(Json::objectValue != m_pRoot->type() && Json::arrayValue != m_pRoot->type())
		return NULL;
	if(Json::objectValue == m_pRoot->type())
	{
		Json::Value* temVal = (&(*m_pRoot)[pName]);
		if(NULL != temVal && Json::nullValue != temVal->type())
			pChild = temVal;
		else
			m_pRoot->removeMember(pName);
	}
	return pChild;
}

Json::Value* CJsonParser::GetRootNode(const char* pName)
{
	if(m_pRoot != NULL)
		return m_pRoot;
	else
		return NULL;
}

bool CJsonParser::SetObjAsChild(const CString& strName,const CString& strValue)
{
	bool bRet = false;
	if(strName.IsEmpty())
	{
		ATLASSERT(0);
		return bRet;
	}
	CString strChild(_T(""));
	//strChild.Format(_T("{ \"%s\" : %s }"),strName,strValue);
	strChild.Format(_T("%s"),strValue);
	char *szName = NULL,*szValue = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&szName);
	CBaseFuncLib::Us2ToChar(strValue/*strChild*/,&szValue);
	if(NULL != szName && NULL != szValue)
	{
		bRet = SetObjAsChild((const char *)szName,szValue);
	}
	if(NULL != szName)
	{
		delete []szName;
		szName = NULL;
	}
	if(NULL != szValue)
	{
		delete []szValue;
		szValue = NULL;
	}
	return bRet;
}

CJsonParser* CJsonParser::GetChildJson(const CString& strName)
{
	if(strName.IsEmpty() || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return NULL;
	}	
	char *szBuf = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&szBuf);
	if(NULL == szBuf)
	{
		ATLASSERT(0);
		return NULL;
	}
	CJsonParser *pJsonParser = GetChildJson(szBuf);
	delete []szBuf;
	szBuf = NULL;
	return pJsonParser;
}

CJsonParser* CJsonParser::GetChildJson(const int nIndex)
{
	if (nIndex < 0 || m_pRoot == NULL)
	{
		ATLASSERT(0);
		return NULL;
	}

	Json::Value *pChild = GetChildNodeByIndex(nIndex);
	ATLASSERT(pChild);
	return GetReadReader(pChild);
}

Json::ValueType CJsonParser::GetChildTypeByName(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
		return Json::nullValue;
	Json::Value *pChild = (&(*m_pRoot)[pName]);
	if (NULL == pChild)
	{
		ATLASSERT(0);
		return Json::nullValue;
	}
	return pChild->type();
}

Json::ValueType CJsonParser::GetType()
{
	Json::ValueType jType = Json::nullValue;
	if(NULL == m_pRoot)
	{
		ATLASSERT(0);
		return jType;
	}
	
	/*
	enum ValueType {
		nullValue = 0, ///< 'null' value
		intValue,      ///< signed integer value
		uintValue,     ///< unsigned integer value
		realValue,     ///< double value
		stringValue,   ///< UTF-8 string value
		booleanValue,  ///< bool value
		arrayValue,    ///< array value (ordered list)
		objectValue    ///< object value (collection of name/value pairs).
	};
	*/
	return m_pRoot->type();
}

Json::ValueType CJsonParser::GetChildTypeByName(const CString& strName)
{
	Json::ValueType jType = Json::nullValue;
	if(strName.IsEmpty() || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return jType;
	}
	char *szBuf = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&szBuf);
	if(NULL == szBuf)
	{
		ATLASSERT(0);
		return jType;
	}
	jType = GetChildTypeByName(szBuf);
	delete []szBuf;
	szBuf = NULL;
	return jType;
}

std::vector<std::string> CJsonParser::GetChildNames()
{
	if(NULL == m_pRoot)
	{
		std::vector<std::string> KeyList;
		return KeyList;
	}
	return m_pRoot->getMemberNames();
}

bool CJsonParser::GetArrayValue(short sIndex,CComBSTR& bstrKeyName,CComVariant& varVal)
{
	bool bRet = false;
	varVal.Clear();
	ULONG nSize = GetArraySize();
	if(sIndex < 0 || sIndex >= (signed int )nSize || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return bRet;
	}
	if(Json::arrayValue == m_pRoot->type())
	{
		if(Json::arrayValue != m_TempValue.type())
			m_TempValue = m_pRoot[0];
		if(Json::arrayValue == m_TempValue.type())
		{
			Json::Value childVal = m_TempValue[sIndex];
			if(Json::uintValue == childVal.type())
			{
				varVal.vt = VT_UI8;
				varVal.ullVal = childVal.asLargestInt();
				bRet = true;
			}
			else if(Json::intValue == childVal.type())
			{
				varVal.vt = VT_UI8;
				varVal.ullVal = childVal.asInt();
				bRet = true;
			}
			else if(Json::realValue == childVal.type())
			{
				varVal.vt = VT_R4;
				varVal.fltVal = childVal.asFloat();
				bRet = true;
			}
			else if(Json::stringValue == childVal.type())
			{
				WCHAR *szVal = NULL;
				CBaseFuncLib::Utf8ToUS2(childVal.asCString(),&szVal);
				if(NULL != szVal)
				{
					varVal.vt = VT_BSTR;
					CComBSTR bstrVal(szVal);
					bstrVal.CopyTo(&varVal.bstrVal);
					bstrVal.Empty();
					delete []szVal;
					szVal = NULL;
					bRet = true;
				}
			}
		}
		return bRet;
	}

	std::vector<std::string> KeyList = GetChildNames();
	char *szKeyName = (char *)KeyList[sIndex].c_str();
	bRet = GetVariantChildByKey(szKeyName,varVal);
	ATLASSERT(bRet);
	if(!bRet)
	{
		ATLASSERT(0);
		return bRet;
	}
	WCHAR *szVal = NULL;
	CBaseFuncLib::Utf8ToUS2(szKeyName,&szVal);
	if(NULL != szVal)
	{
		bstrKeyName.Append(szVal);
		delete []szVal;
		szVal = NULL;
	}
	return bRet;
}

bool CJsonParser::GetVariantChildByKey(char *szName,CComVariant& varVal)
{
	Json::Value* pNode = GetChildNode(szName);
	if(NULL == pNode)
		return false;
	bool bRet = true;
	Json::ValueType jType = pNode->type();
	switch(jType)
	{
	case Json::booleanValue:
		{
			bool bVal = pNode->asBool();
			varVal.vt = VT_BOOL;
			if(bVal)
				varVal.boolVal = VARIANT_TRUE;
			else
				varVal.boolVal = VARIANT_FALSE;
		}
		break;
	case Json::intValue:
		{
			long long llVal = pNode->asLargestInt();
			varVal.vt = VT_I8;
			varVal.llVal = llVal;
		}
		break;
	case Json::uintValue:
		{
			unsigned long long llVal = pNode->asLargestUInt();
			varVal.vt = VT_UI8;
			varVal.ullVal = llVal;
		}
		break;
	case Json::realValue:
		{
			double dVal = pNode->asDouble();
			varVal.vt = VT_R8;
			varVal.dblVal = dVal;
		}
		break;
	case Json::stringValue:
		{
			WCHAR *szUs2Val = NULL;
			const char *szVal = pNode->asCString();
			CBaseFuncLib::Utf8ToUS2(szVal,&szUs2Val);
			if(NULL == szUs2Val)
			{
				ATLASSERT(szUs2Val);
				bRet = false;
			}
			else
			{
				varVal.vt = VT_BSTR;
				CComBSTR bstrVal(szUs2Val);
				bstrVal.CopyTo(&varVal.bstrVal);
				bstrVal.Empty();
				delete []szUs2Val;
				szUs2Val = NULL;
			}
		}
		break;
	case Json::arrayValue:
		varVal.vt = VT_SAFEARRAY;
		varVal.parray = (SAFEARRAY*)GetChildJson(szName);
		break;
	case Json::nullValue:
		bRet = false;
		break;
	default:
		bRet = false;
		/// 未处理
		ATLASSERT(0);
		break;
	}
	pNode = NULL;
	return bRet;
}

bool CJsonParser::GetVariantChildByName(const CString& strName,CComVariant& varVal)
{
	bool bRet = false;
	varVal.Clear();
	if(strName.IsEmpty() || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return bRet;
	}
	char *szBuf = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&szBuf);
	if(NULL == szBuf)
	{
		ATLASSERT(0);
		return bRet;
	}
	bRet = GetVariantChildByKey(szBuf,varVal);
	delete []szBuf;
	szBuf = NULL;

	return bRet;
}

char* CJsonParser::GetStringChildByName(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return NULL;
	}

	Json::Value *pChild = (&(*m_pRoot)[pName]);
	if (pChild == NULL)
	{
		ATLASSERT(0);
		return NULL;
	}
	Json::ValueType jType = pChild->type();
	if(Json::stringValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	char* pString = new char[pChild->asString().length() + 1];
	ATLASSERT(pString);
	strncpy_s(pString,pChild->asString().length() + 1,pChild->asString().c_str(), pChild->asString().length());
	return pString;
}

CString CJsonParser::GetStringChildByName(const CString& strName)
{
	if (NULL == m_pRoot || strName.IsEmpty())
	{
		ATLASSERT(0);
		return _T("");
	}
	char *pszKey = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&pszKey);
	if(NULL == pszKey)
	{
		ATLASSERT(0);
		return _T("");
	}
	char* szVal = GetStringChildByName(pszKey);
	delete []pszKey;
	pszKey = NULL;
	if(NULL == szVal)
		return _T("");
	WCHAR*  strW = NULL;
	CBaseFuncLib::Utf8ToUS2(szVal,&strW);
	delete []szVal;
	szVal = NULL;
	if(NULL == strW)
		return _T("");
	CString strValue(strW);
	delete []strW;
	strW = NULL;
	return strValue;
}

bool CJsonParser::GetValue(VARIANT *pVal)
{
	bool bRet = false;
	if(NULL == pVal)
		return bRet;
	EJsonType eJsonType = (EJsonType )this->GetType();
	switch(eJsonType)
	{
	case JSONTYPE_STRING:
		{
			char* pString = new char[m_pRoot->asString().length() + 1];
			ATLASSERT(pString);
			strncpy_s(pString,m_pRoot->asString().length() + 1,m_pRoot->asString().c_str(), m_pRoot->asString().length());
			WCHAR*  strVal = NULL;
			CBaseFuncLib::Utf8ToUS2(pString,&strVal);
			delete []pString;
			pString = NULL;
			pVal->vt = VT_BSTR;
			pVal->bstrVal = CString(strVal).AllocSysString();
			delete []strVal;
			strVal = NULL;
		}
		break;
	default:
		break;
	}
	return bRet;
}

bool CJsonParser::GetBoolChildByName(const CString& strName)
{
	bool bValue = false;
	if (NULL == m_pRoot || strName.IsEmpty())
	{
		ATLASSERT(0);
		return bValue;
	}
	char *pName = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&pName);
	if(NULL == pName)
	{
		ATLASSERT(0);
		return bValue;
	}
	Json::ValueType jType = GetChildTypeByName(pName);
	if(Json::booleanValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	bValue = (*m_pRoot)[pName].asBool();
	delete []pName;
	pName = NULL;
	return bValue;
}

char* CJsonParser::GetStringChildByName(const int nIndex)
{
	if(nIndex < 0 || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return NULL;
	}

	Json::Value *pChild = GetChildNodeByIndex(nIndex);
	if (pChild == NULL)
	{
		ATLASSERT(0);
		return NULL;
	}

	char* pString = new char[pChild->asString().length() + 1];
	ATLASSERT(pString);
	strncpy_s(pString,pChild->asString().length() + 1,pChild->asString().c_str(), pChild->asString().length());
	return pString;
}

bool CJsonParser::GetBoolChildByIndex(const int nIndex)
{
	if(nIndex < 0 || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return false;
	}

	Json::Value *pChild = GetChildNodeByIndex(nIndex);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return false;
	}
	Json::ValueType jType = pChild->type();
	if(Json::stringValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	return pChild->asBool();
}

bool CJsonParser::GetBoolChildByName(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0;
	}

	Json::Value *pChild = (&(*m_pRoot)[pName]);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::booleanValue != jType)
	{
		ATLASSERT(0);
		return 0;
	}
	return pChild->asBool();
}

int CJsonParser::GetIntChildByName(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0;
	}

	Json::Value *pChild = (&(*m_pRoot)[pName]);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::stringValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}
	return pChild->asInt();
}

int CJsonParser::GetIntChildByName(const CString& strName)
{
	bool bValue = false;
	if (NULL == m_pRoot || strName.IsEmpty())
	{
		ATLASSERT(0);
		return bValue;
	}
	char *pszKey = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&pszKey);
	if(NULL == pszKey)
	{
		ATLASSERT(0);
		return bValue;
	}
	int nValue = GetIntChildByName(pszKey);
	delete []pszKey;
	pszKey = NULL;
	return nValue;
}

int CJsonParser::GetIntChildByIndex(const int nIndex)
{
	if(nIndex < 0 || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0;
	}

	Json::Value *pChild = GetChildNodeByIndex(nIndex);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::stringValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	return pChild->asInt();
}

unsigned long long CJsonParser::GetUintChildByName(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0;
	}

	Json::Value *pChild = (&(*m_pRoot)[pName]);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::uintValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	return pChild->asUInt64();
}

unsigned long long CJsonParser::GetUIntChildByName(const CString& strName)
{
	if (NULL == m_pRoot || strName.IsEmpty())
	{
		ATLASSERT(0);
		return 0;
	}
	char *pszKey = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&pszKey);
	if(NULL == pszKey)
	{
		ATLASSERT(0);
		return 0;
	}
	unsigned long long nValue = GetUintChildByName(pszKey);
	delete []pszKey;
	pszKey = NULL;
	return nValue;
}

unsigned long long CJsonParser::GetUintChildByIndex(const int nIndex)
{
	if(nIndex < 0 || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0;
	}

	Json::Value *pChild = GetChildNodeByIndex(nIndex);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::uintValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	return pChild->asUInt64();
}

long long CJsonParser::GetLongLongChildByName(const CString& strName)
{
	if (NULL == m_pRoot || strName.IsEmpty())
	{
		ATLASSERT(0);
		return 0;
	}
	char *pszKey = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&pszKey);
	if(NULL == pszKey)
	{
		ATLASSERT(0);
		return 0;
	}
	long long nValue = GetInt64ChildByName(pszKey);
	delete []pszKey;
	pszKey = NULL;
	return nValue;
}

long long CJsonParser::GetInt64ChildByName(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0;
	}

	Json::Value *pChild = (&(*m_pRoot)[pName]);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::uintValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	return (long long)pChild->asInt64();
}

long long CJsonParser::GetInt64ChildByIndex(const int nIndex)
{
	if(nIndex < 0 || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0;
	}

	Json::Value *pChild = GetChildNodeByIndex(nIndex);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::stringValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	return (long long)pChild->asInt64();
}

double CJsonParser::GetDoubleChildByName(const CString& strName)
{
	if (NULL == m_pRoot || strName.IsEmpty())
	{
		ATLASSERT(0);
		return 0.0;
	}
	char *pszKey = NULL;
	CBaseFuncLib::US2ToUtf8(strName,&pszKey);
	if(NULL == pszKey)
	{
		ATLASSERT(0);
		return 0.0;
	}
	double dVal = GetDoubleChildByName(pszKey);
	delete []pszKey;
	pszKey = NULL;

	return dVal;
}

double CJsonParser::GetDoubleChildByName(const char* pName)
{
	if(NULL == pName || NULL == m_pRoot)
	{
		ATLASSERT(0);
		return 0.0;
	}

	Json::Value *pChild = (&(*m_pRoot)[pName]);
	if( NULL == pChild)
	{
		ATLASSERT(0);
		return 0;
	}
	Json::ValueType jType = pChild->type();
	if(Json::realValue < jType)
	{
		if(Json::nullValue != jType)
		{
			ATLASSERT(0);
		}
		return 0;
	}

	return pChild->asDouble();
}

CJsonParser* CJsonParser::GetReadReader(Json::Value* pRoot)
{
	if (NULL == pRoot)
	{
		ATLASSERT(0);
		return NULL;
	}

	CJsonParser* pReader = new CJsonParser();
	if (NULL == pReader)
	{
		ATLASSERT(0);
		return NULL;
	}
	pReader->m_pRoot = pRoot;
	pReader->m_bDeleteSelf = false;

	return pReader;
}

// 创建Json文件写入到本地文件".....Config/JsonCreateTest.json"
BOOL CJsonParser::CreateJsonDemoToFile(const CString& strJsonFile)
{
	m_eCodingType = CODINGTYPE_US2;
	//根节点
	Json::Value root;
	m_pRoot = &root;
	//根节点属性
	root["name"] = Json::Value("Tsybius");
	root["age"] = Json::Value(23);
	root["sex_is_male"] = Json::Value(true);

	//子节点
	Json::Value partner;

	//子节点属性
	partner["partner_name"] = Json::Value("Galatea");
	partner["partner_age"] = Json::Value(21);
	partner["partner_sex_is_male"] = Json::Value(false);

	//子节点挂到根节点上
	root["partner"] = Json::Value(partner);

	//数组形式
	root["achievement"].append("ach1");
	root["achievement"].append("ach2");
	root["achievement"].append("ach3");

	// 输出上面创建的Json格式内容
	Json::FastWriter fw;
	std::string str = fw.write(root);
	cout << fw.write(root) << endl << endl;

	std::cout << "换一种方式输出" << endl << endl;

	Json::StyledWriter writer;
	std::string output = writer.write((root));
	cout << writer.write(root) << endl << endl;


	SaveToFile(strJsonFile);

	return 0;
}

BOOL CJsonParser::CreateJsonDemoToString(BSTR* strResult)
{
	return 0;
}

BOOL CJsonParser::ParseJsonFromFile(const CString& strJsonFile)
{
	return 0;
}

BOOL CJsonParser::ParseJsonFromString(const CString& strJsonFile)
{
	return 0;
}

// 反序列化Json对象
// --------------------------------------------------------------------------------------
// http://www.cnblogs.com/ytjjyy/archive/2012/04/17/2453348.html
// 比如一个Json对象的字符串序列如下,其中”array”:[...]表示Json对象中的数组：
// {“key1″:”value1″,”array”:[{"xxx":"value2"},{"xxx":"value3"},{"xxx":"value4"}]},
// 那怎么分别取到key1和xxx的值呢
void CJsonParser::JsonDesignFormatTest00()
{
	/*
	{
		"array" : [
			{
				"xxx" : "value2"
			},
			{
				"xxx" : "value3"
			},
			{
				"xxx" : "value4"
			}
		],
		"key1" : "value1"
	}
	*/
	std::string strValue = "{\"key1\":\"value1\",\"array\":[{\"xxx\":\"value2\"},{\"xxx\":\"value3\"},{\"xxx\":\"value4\"}]}";

	Json::Reader reader;
	Json::Value value;

	if (reader.parse(strValue, value))
	{
		std::string out = value["key1"].asString();
		std::cout << out << std::endl;
		const Json::Value arrayObj = value["array"];
		for (unsigned int i=0; i<arrayObj.size(); i++)
		{
			out = arrayObj[i]["xxx"].asString();
			std::cout << out;
			if (i != (arrayObj.size() - 1 ))
				std::cout << std::endl;
		}
	}

	std::cout << std::endl << std::endl << std::endl;
	Json::StyledWriter writer;
	std::string output = writer.write((value));
	cout << writer.write(value) << endl << endl;
}

// 序列化Json对象
// --------------------------------------------------------------------------------------
// http://www.cnblogs.com/ytjjyy/archive/2012/04/17/2453348.html
// 构建下面格式的Json文件，并格式化输出到控制台
// 重点是拼接数组
void CJsonParser::JsonDesignFormatTest01()
{
	/*
	{
		"array" : [
			{
				"key" : 0
			},
			{
				"key" : 1
			},
			{
				"key" : 2
			},
			{
				"key" : 3
			},
			{
				"key" : 4
			},
			{
				"key" : 5
			}
		],
		"key1" : "value1",
		"key2" : "value2"
	}
	*/
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;
	for (int i=0; i<=5; i++)
	{
		item["key"] = i;
		arrayObj.append(item);
	}
	const char *p = "sdfsdfsdfsdf";
	root[p] = "dfsdf";
	cout << root.toStyledString().c_str() << endl << endl;
	root["key1"] = "value1";
	root["key2"] = "value2";
	root["array"] = arrayObj;
	root.toStyledString();
	std::string out = root.toStyledString();
	std::cout << out << std::endl;

	// 删除key节点,key是array节点数组里的一个元素,删除key节点是不成功的
	root.removeMember("key");
	printf("%s \n",root.toStyledString().c_str());

	// 删除array节点,成功
	root.removeMember("array");
	printf("%s \n",root.toStyledString().c_str());
}

// 序列化Json对象
// --------------------------------------------------------------------------------------
// http://www.cnblogs.com/ytjjyy/archive/2012/04/17/2453348.html
// 添加子节点的方法--就是字符串赋值
void CJsonParser::JsonDesignFormatTest02()
{
	/*
	{
		"ChildNode" : {
			"ChildNodekey1" : "ChildNodevalue1",
			"ChildNodekey2" : "ChildNodevalue2",
			"ChildNodekey3" : "ChildNodevalue3"
		},
		"key1" : "value1",
		"key2" : "value2",
		"key3" : "value3"
	}
	*/
	Json::Value root;
	Json::Value ChildNode;
	Json::Value item;

	root["key1"] = "value1";
	root["key2"] = "value2";
	root["key3"] = "value3";
	root["ChildNode"] = "ChildNode";

	cout << root.toStyledString().c_str() << endl << endl;


	ChildNode["ChildNodekey1"] = "ChildNodevalue1";
	ChildNode["ChildNodekey2"] = "ChildNodevalue2";
	ChildNode["ChildNodekey3"] = "ChildNodevalue3";
	root["ChildNode"] = ChildNode;

	cout << root.toStyledString().c_str() << endl << endl;
}

// 序列化Json对象
// --------------------------------------------------------------------------------------
// http://www.cnblogs.com/SkySoot/archive/2012/04/17/2453010.html
// 创建复杂Json格式
void CJsonParser::JsonDesignFormatTest03()
{
	/*
	{
		"programmers": [
			{
				"firstName": "Brett", 
				"lastName": "McLaughlin", 
				"email": "aaaa"
			}, 
			{
				"firstName": "Jason", 
				"lastName": "Hunter", 
				"email": "bbbb"
			}
		], 
		"authors": [
			{
				"firstName": "Isaac", 
				"lastName": "Asimov", 
				"genre": "science fiction"
			}, 
			{
				"firstName": "Tad", 
				"lastName": "Williams", 
				"genre": "fantasy"
			}
		], 
		"musicians": [
			{
				"firstName": "Eric", 
				"lastName": "Clapton", 
				"instrument": "guitar"
			}, 
			{
				"firstName": "Sergei", 
				"lastName": "Rachmaninoff", 
				"instrument": "piano"
			}
		]
	}
	*/
	Json::Value root;
	Json::Value programmers,programmersChild;
	Json::Value authors,authorsChild;
	Json::Value musicians,musiciansChild;

	programmersChild["firstName"] = "Brett";
	programmersChild["lastName"] = "McLaughlin";
	programmersChild["email"] = "aaaa";
	programmers.append(programmersChild);

	programmersChild["firstName"] = "Jason";
	programmersChild["lastName"] = "Hunter";
	programmersChild["email"] = "bbbb";
	programmers.append(programmersChild);

	authorsChild["firstName"] = "Isaac";
	authorsChild["lastName"] = "Asimov";
	authorsChild["genre"] = "science fiction";
	authors.append(authorsChild);

	authorsChild["firstName"] = "Tad";
	authorsChild["lastName"] = "Williams";
	authorsChild["genre"] = "fantasy";
	authors.append(authorsChild);

	musiciansChild["firstName"] = "Eric";
	musiciansChild["lastName"] = "Clapton";
	musiciansChild["instrument"] = "guitar";
	musicians.append(musiciansChild);

	authorsChild["firstName"] = "Sergei";
	authorsChild["lastName"] = "Rachmaninoff";
	authorsChild["instrument"] = "piano";
	musicians.append(musiciansChild);

	root["programmers"] = programmers;
	root["authors"] = authors;
	root["musicians"] = musicians;

	cout << root.toStyledString().c_str() << endl << endl;
}

// 反序列化Json对象
// --------------------------------------------------------------------------------------
// 获取某个节点的指针
void CJsonParser::JsonDesignFormatTest04()
{

}

// 反序列化Json对象
// --------------------------------------------------------------------------------------
// 遍历整个Json
void CJsonParser::JsonDesignFormatTest05()
{

}

void CJsonParser::JsonDesignFormatTest06()
{

}

void CJsonParser::JsonDesignFormatTest07()
{

}

void CJsonParser::JsonDesignFormatTest08()
{

}

Json::Value* CJsonParser::GetChildNodeByIndex( const int nIndex )
{
	if( !m_pRoot->isValidIndex(nIndex))
	{
		ATLASSERT(0);
		return NULL;
	}
	Json::ValueType vType = m_pRoot->type();
	if(Json::arrayValue != vType && Json::nullValue != vType)
		return NULL;/// 类型错误
	Json::Value *pChild = (&(*m_pRoot)[nIndex]);
	ATLASSERT(pChild);
	return pChild;
}
