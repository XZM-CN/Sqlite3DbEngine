/*

*/
#pragma once

#ifdef _MSC_VER
#include <atlcomcli.h>
#include <comutil.h>
#include <atlstr.h>
#include <comdef.h>
#include <comdefsp.h>
#include "EventStructDef.h"

typedef IXMLDOMDocument DocumentType;
typedef IXMLDOMNode NodeType;

//遍历子节点
//node: 要遍历的节点
//childNodeVariableName: 枚举到的子节点变量名
//childNodeNameVariableName_bstr_t: childNodeVariableName 的节点名，_bstr_t 类型的
#define BEGIN_TRAVEL_CHILDREN(node, childNodeVariableName, childNodeNameVariableName) \
	CComQIPtr<IXMLDOMNodeList> children; \
	if( FAILED(node->get_childNodes(&children)) ) \
{ \
	return false; \
} \
	\
	long ttl = 0; \
	if( FAILED(children->get_length(&ttl)) ) \
{ \
	return false; \
} \
	if( ttl < 0 ) \
{ \
	return false; \
} \
	\
	for(UINT i = 0; i < (UINT)ttl; ++i) \
{ \
	CComQIPtr<IXMLDOMNode> childNodeVariableName; \
	if( FAILED(children->get_item(i, &childNodeVariableName)) ) \
{ \
	return false; \
} \
	DOMNodeType nodeType; \
	if( FAILED(childNodeVariableName->get_nodeType(&nodeType)) ) \
{ \
	return false; \
} \
	if( nodeType != NODE_ELEMENT ) \
{ \
	continue; \
} \
	BSTR bstr; \
	if( FAILED(childNodeVariableName->get_nodeName(&bstr)) ) \
{ \
	return false; \
} \
	_bstr_t childNodeNameVariableName(bstr); \
	::SysFreeString(bstr); \
	bstr = NULL;

#define END_TRAVEL_CHILDREN \
}




//遍历节点属性
#define BEGIN_TRAVEL_ATTRIBUTES(node, attrNameVariableName, attrValueVariableName) \
	CComQIPtr<IXMLDOMNamedNodeMap> attrList; \
	if( FAILED(node->get_attributes(&attrList)) ) \
{ \
	return false; \
} \
	\
	long ttl = 0; \
	if( FAILED(attrList->get_length(&ttl)) ) \
{ \
	return false; \
} \
	if( ttl < 0 ) \
{ \
	return false; \
} \
	\
	for(UINT i = 0; i < (UINT)ttl; ++i) \
{ \
	CComQIPtr<IXMLDOMNode> attrNode; \
	if( FAILED(attrList->get_item(i, &attrNode)) ) \
{ \
	return false; \
} \
	CComQIPtr<IXMLDOMAttribute> attr = attrNode; \
	if( !attr ) \
{ \
	return false; \
} \
	\
	_bstr_t attrNameVariableName; \
	BSTR bstr; \
	if( FAILED(attr->get_name(&bstr)) ) \
{ \
	return false; \
} \
	attrNameVariableName = bstr; \
	::SysFreeString(bstr); \
	bstr = NULL; \
	\
	_variant_t attrValueVariableName; \
	if( FAILED(attr->get_nodeValue(&attrValueVariableName)) ) \
{ \
	return false; \
}


#define END_TRAVEL_ATTRIBUTES \
}



#endif


class CParseEventLogXML
{
public:
	CParseEventLogXML();
	virtual ~CParseEventLogXML();

	DWORD LoadFromString(LPCTSTR lpszXMLBuffer);
	BOOL ParseSetupEventLog(SYSEVENTINFO& stEvnetLog);

protected:
	DWORD Parse_Source(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_EventID(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_Level(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_EventTask(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_EventTime(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_EventClass(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_EventComputer(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_EventUser(NodeType* childNode,SYSEVENTINFO& stEvnetLog);

	DWORD Parse_Discription(NodeType* childNode,SYSEVENTINFO& stEvnetLog);
	DWORD Parse_DiscriptionSP(NodeType* childNode,SYSEVENTINFO& stEvnetLog);

	DWORD ConvertXMLClassToEvent(DWORD dwLevel);
	BOOL RemoveMSFlagFrom(CString& strSource);
	DWORD ConvertTimeToSecond(CString strTime);
private:


	CComQIPtr<DocumentType> m_XMLDoc;
	CString m_csErrMsg;
};
