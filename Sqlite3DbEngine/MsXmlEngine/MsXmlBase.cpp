#include "StdAfx.h"
#include "MsXmlBase.h"



int CMsXmlBase::m_COMCount = 0;

CMsXmlBase::CMsXmlBase()
{
	if(CMsXmlBase::m_COMCount == 0)
		CoInitialize(NULL);
	else
		CMsXmlBase::m_COMCount++;
}

CMsXmlBase::~CMsXmlBase()
{
	if(CMsXmlBase::m_COMCount == 1){
		CoUninitialize();
		CMsXmlBase::m_COMCount--;
	}
	else
		CMsXmlBase::m_COMCount--;
}

int CMsXmlBase::createXml(TCHAR* filePath)
{
	TCHAR *szXmlFile = filePath; // xml文件
	MSXML2::IXMLDOMDocumentPtr pDoc = NULL; // xml文档
	MSXML2::IXMLDOMProcessingInstructionPtr pProInstruction = NULL; // xml声明
	MSXML2::IXMLDOMCommentPtr pComment = NULL; // 注释
	MSXML2::IXMLDOMElementPtr pRootElement = NULL, pElement = NULL; // 根节点(元素)
	MSXML2::IXMLDOMNodePtr pNode = NULL, pNode1 = NULL, pNode2 = NULL; // 节点
	MSXML2::IXMLDOMAttributePtr pAttrNode = NULL; // 属性

	HRESULT hr = pDoc.CreateInstance("Msxml2.DOMDocument.6.0");//__uuidof(DOMDocument40)
	if (FAILED(hr))
	{
		printf("无法创建DOMDocument40对象，请检查是否安装并初始化了MsXml Parser库！");
		return EXIT_FAILURE;
	}


	// (1)创建xml文档声明(或insertBefore根节点)
	pProInstruction = pDoc->createProcessingInstruction((_bstr_t)(TCHAR*)_T("xml"),
		(_bstr_t)(TCHAR*)_T("version=\"1.0\" encoding=\"utf-8\"")); 
	pDoc->appendChild((MSXML2::IXMLDOMNode*)pProInstruction);

	// (2)创建根节点<China>
	pRootElement =pDoc->createElement((_bstr_t)(TCHAR*)_T("China"));
	pDoc->PutRefdocumentElement(pRootElement); // pXMLDomDoc->documentElement = pRootElement;

	// (3)创建节点<China><Continent>
	pComment = pDoc->createComment((_bstr_t)(TCHAR*)_T("所在的洲"));
	pRootElement->appendChild((MSXML2::IXMLDOMNode*)pComment); // 注释

	pNode = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("Continent"), (_bstr_t)(TCHAR*)XMLNS);
	pNode->Puttext((_bstr_t)(TCHAR*)_T("Asia")); // pNode->text = "Asia";
	pRootElement->appendChild(pNode); // 节点

	// (4)创建节点<China><Population>
	pComment = pDoc->createComment((_bstr_t)(TCHAR*)_T("人口数量"));
	pRootElement->appendChild((MSXML2::IXMLDOMNode*)pComment); // 注释

	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Population"));
	pAttrNode = pDoc->createAttribute((_bstr_t)(TCHAR*)_T("Units"));
	pAttrNode->Puttext((_bstr_t)(TCHAR*)_T("Million Person"));
	pElement->setAttributeNode(pAttrNode); // 统计单位
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("StatisticalYear"), (_variant_t)(TCHAR*)_T("2000")); // 统计年份
	pElement->Puttext((_bstr_t)(TCHAR*)_T("1,296"));
	pRootElement->appendChild(pElement); // 节点

	// (5)创建节点<China><Municipality>
	pComment = pDoc->createComment((_bstr_t)(TCHAR*)_T("四个直辖市"));
	pRootElement->appendChild((MSXML2::IXMLDOMNode*)pComment); // 注释

	pNode = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("Municipality"), (_bstr_t)(TCHAR*)_T(""));
	pRootElement->appendChild(pNode); // 节点

	// (6)创建节点<China><Municipality><TianJin>
	pNode1 = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("TianJin"), (_bstr_t)(TCHAR*)_T(""));

	//创建节点<China><Municipality><TianJin><Area>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Area"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Thousand Square kilometers")); // 统计单位
	pElement->Puttext((_bstr_t)(TCHAR*)_T("12"));
	pNode1->appendChild((MSXML2::IXMLDOMNode*)pElement); // 节点

	//创建节点<China><Municipality><TianJin><Population>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Population"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Million Person")); // 统计单位
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("StatisticalYear"), (_variant_t)(TCHAR*)_T("2000")); // 统计年份
	pElement->Puttext((_bstr_t)(TCHAR*)_T("10.01"));
	pNode1->appendChild((MSXML2::IXMLDOMNode*)pElement); // 节点

	pNode->appendChild(pNode1);
	// (7)创建节点<China><Municipality><BeiJing>并插入<TianJin>前
	pNode2 = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("BeiJing"), (_bstr_t)(TCHAR*)_T(""));

	//创建节点<China><Municipality><BeiJing><Area>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Area"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Thousand Square kilometers")); // 统计单位
	pElement->Puttext((_bstr_t)(TCHAR*)_T("17"));
	pNode2->appendChild((MSXML2::IXMLDOMNode*)pElement); // 节点

	//创建节点<China><Municipality><BeiJing><Population>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Population"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Million Person")); // 统计单位
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("StatisticalYear"), (_variant_t)(TCHAR*)_T("2000")); // 统计年份
	pElement->Puttext((_bstr_t)(TCHAR*)_T("13.82"));
	pNode2->appendChild((MSXML2::IXMLDOMNode*)pElement); // 节点

	pNode->insertBefore(pNode2, (_variant_t)(IDispatch*)pNode1);
	//
	// (8)创建节点<China><Municipality><ShangHai>
	// (9)创建节点<China><Municipality><ChongQing>

	pDoc->save((_variant_t)szXmlFile);

	return EXIT_SUCCESS;
}

int CMsXmlBase::createXmlEx(TCHAR* filePath){
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);

	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("China"));


	IXZMCommentPtr pComment1 = NULL;
	pComment1 = CreateComment(pIXZMDocumentPtr,_T("所在的洲"));
	pRootElement->appendChild((IXZMNode*)pComment1); // 注释


	IXZMNodePtr pNode1 = NULL;
	pNode1 = CreateNode(pIXZMDocumentPtr,_T("Continent"),_T("Asia"),XMLNS);
	pRootElement->appendChild(pNode1); // 节点

	IXZMCommentPtr pComment2 = NULL;
	pComment2 = CreateComment(pIXZMDocumentPtr,_T("人口数量"));
	pRootElement->appendChild((IXZMNode*)pComment2); // 注释

	IXZMElementPtr pElement1 = NULL;
	pElement1 = CreateElement(pIXZMDocumentPtr,_T("Population"),_T("1,296"));

	IXZMAttributePtr pAttr1 = NULL;
	pAttr1 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
	pElement1->setAttributeNode(pAttr1); // 统计单位
	pAttr1 = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("2000"));
	pElement1->setAttributeNode(pAttr1); // 统计年份

	pRootElement->appendChild(pElement1); // 节点

	IXZMCommentPtr pComment3 = NULL;
	pComment3 = CreateComment(pIXZMDocumentPtr,_T("四个直辖市"));
	pRootElement->appendChild((IXZMNode*)pComment3); // 注释

	IXZMNodePtr pNode2 = NULL;
	pNode2 = CreateNode(pIXZMDocumentPtr,_T("Municipality"),_T(""),XMLNS);
	pRootElement->appendChild(pNode2); // 节点

	IXZMNodePtr pNode21 = NULL;
	pNode21 = CreateNode(pIXZMDocumentPtr,_T("TianJin"),_T(""),_T(""));

	IXZMElementPtr pElement2 = NULL;
	pElement2 = CreateElement(pIXZMDocumentPtr,_T("Population"),_T("1,296"));

	IXZMAttributePtr pAttr2 = NULL;
	pAttr2 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
	pElement2->setAttributeNode(pAttr2); // 统计单位
	pAttr2 = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("10.01"));
	pElement2->setAttributeNode(pAttr2); // 统计年份
	pNode21->appendChild((MSXML2::IXMLDOMNode*)pElement2);

	IXZMElementPtr pElement3 = NULL;
	pElement3 = CreateElement(pIXZMDocumentPtr,_T("Area"),_T("17"));

	IXZMAttributePtr pAttr3 = NULL;
	pAttr3 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Thousand Square kilometers"));
	pElement3->setAttributeNode(pAttr3); // 统计单位
	pNode21->appendChild((MSXML2::IXMLDOMNode*)pElement3);


	pNode2->appendChild(pNode21);




	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0; 
}

int CMsXmlBase::CreateDemo01(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);

	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点-IXZMElementPtr"));
	pIXZMDocumentPtr->save((_variant_t)filePath);

	return 0;
}

int CMsXmlBase::CreateDemo02(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);

	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点-IXZMElementPtr"));

	IXZMCommentPtr pComment = NULL;
	pComment = CreateComment(pIXZMDocumentPtr,_T("注释元素"));
	pRootElement->appendChild((IXZMNode*)pComment);


	pIXZMDocumentPtr->save((_variant_t)filePath);

	return 0;
}

int CMsXmlBase::CreateDemo03(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);
	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("元素节点-IXZMElementPtr"));

	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,
		_T("子节点-IXZMNodePtr"),
		_T("子节点值"),
		_T("xzm-xml_space"));//xml的命名空间，有点类似于namespaces似的
	pRootElement->appendChild(pNode);

	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0;
}

int CMsXmlBase::CreateDemo04(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);
	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点-IXZMElementPtr"));



	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("子节点-IXZMNodePtr"),_T(""),XMLNS);
	pRootElement->appendChild(pNode);

	IXZMNodePtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("子节点的子节点-IXZMNodePtr"),_T(""),_T(""));

	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("元素节点-IXZMElementPtr"),_T("值"));

	IXZMAttributePtr pAttr_1_1 = NULL;
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("属性一"),_T("属性value01"));
	pElement_1->setAttributeNode(pAttr_1_1);
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("属性二"),_T("属性value02"));
	pElement_1->setAttributeNode(pAttr_1_1);
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_1);

	pNode->appendChild(pNodeChild);




	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0;
}

int CMsXmlBase::CreateDemo05(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);
	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点-IXZMElementPtr"));



	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("Municipality"),_T(""),XMLNS);
	pRootElement->appendChild(pNode); // 节点

	IXZMNodePtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("TianJin"),_T(""),_T(""));

	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("Population"),_T("1,296"));

	IXZMAttributePtr pAttr_1_1 = NULL;
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
	pElement_1->setAttributeNode(pAttr_1_1); // 统计单位
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("10.01"));
	pElement_1->setAttributeNode(pAttr_1_1); // 统计年份
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_1);

	IXZMElementPtr pElement_2 = NULL;
	pElement_2 = CreateElement(pIXZMDocumentPtr,_T("Area"),_T("17"));

	IXZMAttributePtr pAttr_2_1 = NULL;
	pAttr_2_1 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Thousand Square kilometers"));
	pElement_2->setAttributeNode(pAttr_2_1); // 统计单位
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_2);

	pNode->appendChild(pNodeChild);




	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0;
}

int CMsXmlBase::CreateDemo06(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);
	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点"));



	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("元素"),_T(""),_T(""));
	pRootElement->appendChild(pNode);
	IXZMAttributePtr pAttr_0_0 = NULL;
	pAttr_0_0 = CreateAttr(pIXZMDocumentPtr,_T("类型"),_T("图形"));
	pRootElement->setAttributeNode(pAttr_0_0);


	IXZMElementPtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("节点"),_T(""),_T(""));

	IXZMAttributePtr pAttr_x_x = NULL;
	pAttr_x_x = CreateAttr(pIXZMDocumentPtr,_T("num"),_T("01"));
	pNodeChild->setAttributeNode(pAttr_x_x);

	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("元素属性1"),_T("什么属性"));

	IXZMAttributePtr pAttr_1_1 = NULL;
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_1->setAttributeNode(pAttr_1_1); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_1);

	IXZMElementPtr pElement_2 = NULL;
	pElement_2 = CreateElement(pIXZMDocumentPtr,_T("元素属性1"),_T("什么属性"));

	IXZMAttributePtr pAttr_2_1 = NULL;
	pAttr_2_1 = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_2->setAttributeNode(pAttr_2_1); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_2);

	pNode->appendChild(pNodeChild);




	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0;
}

int CMsXmlBase::CreateDemo07(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);
	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点"));



	IXZMElementPtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("元素"),_T(""),_T(""));
	pRootElement->appendChild(pNode);

	IXZMAttributePtr pAttr_0_0 = NULL;
	pAttr_0_0 = CreateAttr(pIXZMDocumentPtr,_T("类型"),_T("图形"));
	pNode->setAttributeNode(pAttr_0_0);


	IXZMElementPtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("节点"),_T(""),_T(""));


	IXZMAttributePtr pAttr_x_x = NULL;
	pAttr_x_x = CreateAttr(pIXZMDocumentPtr,_T("num"),_T("01"));
	pNodeChild->setAttributeNode(pAttr_x_x);

	IXZMAttributePtr pAttr = NULL;


	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("图片位置"),_T("D:\\CustomCode\\AllXzyUpdate\\trunk\\DiagramEditor\\1.png"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_1->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_1);

	IXZMElementPtr pElement_2 = NULL;
	pElement_2 = CreateElement(pIXZMDocumentPtr,_T("图片位置"),_T("D:\\CustomCode\\AllXzyUpdate\\trunk\\DiagramEditor\\2.png"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_2->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_2);

	IXZMElementPtr pElement_3 = NULL;
	pElement_3 = CreateElement(pIXZMDocumentPtr,_T("水平坐标"),_T("100"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_3->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_3);

	IXZMElementPtr pElement_4 = NULL;
	pElement_4 = CreateElement(pIXZMDocumentPtr,_T("垂直坐标"),_T("200"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_4->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_4);

	IXZMElementPtr pElement_5 = NULL;
	pElement_5 = CreateElement(pIXZMDocumentPtr,_T("宽"),_T("480"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_5->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_5);

	IXZMElementPtr pElement_6 = NULL;
	pElement_6 = CreateElement(pIXZMDocumentPtr,_T("高"),_T("600"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_6->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_6);

	pNode->appendChild(pNodeChild);




	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0;
}

int CMsXmlBase::CreateDemo08(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);
	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点-IXZMElementPtr"));



	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0;
}

int CMsXmlBase::CreateDemo09(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);
	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	IXZMElementPtr pRootElement = NULL;
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("根节点-IXZMElementPtr"));



	pIXZMDocumentPtr->save((_variant_t)filePath);
	return 0;
}

void CMsXmlBase::OpenXml(TCHAR* filePath)
{
	MSXML2::IXMLDOMDocument2Ptr pXMLDocument;
	pXMLDocument.CreateInstance(__uuidof(MSXML2::DOMDocument));
	pXMLDocument->load(filePath);  
	MSXML2::IXMLDOMElementPtr pRootNode = pXMLDocument->documentElement;
	OutputDebugString(_T("ddddd\n"));
	OutputDebugString(pRootNode->text);
	OutputDebugString(_T("\n"));
	//查找<person>节点.  
	MSXML2::IXMLDOMNodeListPtr pPersonNodes = pRootNode->selectNodes(_T("元素")); 
	long size = pPersonNodes->length;
	for (int i = 0; i<size; i++) {
		MSXML2::IXMLDOMNodePtr pPersonNode = pPersonNodes->item[i];
		OutputDebugString(pPersonNode->text);
	}
}

void CMsXmlBase::SystemCfg(TCHAR* filePath)
{
	BOOL bRet = FALSE;

	IXZMDocumentPtr pIXZMDocumentPtr = NULL;
	bRet = CreateDoc(pIXZMDocumentPtr);

	bRet = CreateStatement(pIXZMDocumentPtr,_T("xml"),_T("version=\"1.0\" encoding=\"utf-8\""));

	// 根标签
	IXZMElementPtr pRootElement = NULL;

	if (TRUE) {
		pRootElement = CreateRoot(pIXZMDocumentPtr,_T("SysCfg"));
	}

	if(!SysCfg_CryptLib(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_DecryptEngine(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_DispatchModuleCenter(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_EncryEngine(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_HXTest(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_HXTestClient(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_JsonEngine(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_JsonLib(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_LogEngine(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_MsXmlEngine(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_MySqlDbEngine(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_NetComm(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_NetSocketServerTest(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_SchedulerCenter(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_SecurityCollection(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_ServiceModular(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_SignalToDriveceModular(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_SignalToServiceModular(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_SKFEncry(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_Sqlite3DbEngine(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_SqliteLib(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_TelnetServer(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_XzmSqlite3MfcApp(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_XzmSqlite3WtlApp(pIXZMDocumentPtr, pRootElement))
		return;
	if(!SysCfg_ZLib(pIXZMDocumentPtr, pRootElement))
		return;



	pIXZMDocumentPtr->save((_variant_t)filePath);
}

BOOL CMsXmlBase::SysCfg_CryptLib(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("CryptLib的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("CryptLib"),_T("Asia"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_DecryptEngine(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("DecryptEngine的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment); // 注释

		// 创建元素标签
		pElement = NULL;
		pElement = CreateElement(pIXZMDocumentPtr,_T("DecryptEngine"),_T("1,296"));

		// 创建属性节点，并将其挂在到元素标签内
		pAttr = NULL;
		pAttr = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
		pElement->setAttributeNode(pAttr);
		pAttr = NULL;
		pAttr = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("2000"));
		pElement->setAttributeNode(pAttr);

		// 将元素标签挂载到SysCfg节点下
		pRootElement->appendChild(pElement);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_DispatchModuleCenter(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("DispatchModuleCenter的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment); // 注释

		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("DispatchModuleCenter"),_T(""));
		pRootElement->appendChild(pNode); // 节点

		if (TRUE) {
			IXZMNodePtr pLever02Node = NULL;
			pLever02Node = CreateNode(pIXZMDocumentPtr,_T("TianJin"),_T(""),_T(""));

			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("Population"),_T("1,296"));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("10.01"));
				pElement->setAttributeNode(pAttr); // 统计年份
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}

			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("Area"),_T("17"));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Thousand Square kilometers"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}

			pNode->appendChild(pLever02Node);
		}
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_EncryEngine(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("EncryEngine的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("EncryEngine"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_HXTest(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("HXTest的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("HXTest"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_HXTestClient(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("HXTestClient的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment); // 注释

		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("HXTestClient"),_T(""));
		pRootElement->appendChild(pNode); // 节点

		if (TRUE) {
			IXZMNodePtr pLever02Node = NULL;
			pLever02Node = CreateNode(pIXZMDocumentPtr,_T("TargetServer"),_T(""),_T(""));

			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("ServerMajorIP"),_T("514"));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("IP"),_T("192.168.101.123"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}
			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("ServerMinorIP"),_T("514"));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("IP"),_T("192.168.101.123"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}
			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("ServerSpareIP"),_T("514"));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("IP"),_T("192.168.101.123"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}
			pNode->appendChild(pLever02Node);
		}


		if (TRUE) {
			IXZMNodePtr pLever02Node = NULL;
			pLever02Node = CreateNode(pIXZMDocumentPtr,_T("SpareServer"),_T(""),_T(""));

			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("ServerMajorIP"),_T("ServerMajorIP呵呵"));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("IP"),_T("192.168.101.123"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("Port"),_T("514"));
				pElement->setAttributeNode(pAttr); // 统计年份
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}
			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("ServerMinorIP"),_T(""));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("IP"),_T("192.168.101.123"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("Port"),_T("514"));
				pElement->setAttributeNode(pAttr); // 统计年份
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}
			if (TRUE) {
				pElement = NULL;
				pElement = CreateElement(pIXZMDocumentPtr,_T("ServerSpareIP"),_T("ServerSpareIP呵呵"));

				pAttr = NULL;
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("IP"),_T("192.168.101.123"));
				pElement->setAttributeNode(pAttr); // 统计单位
				pAttr = CreateAttr(pIXZMDocumentPtr,_T("Port"),_T("514"));
				pElement->setAttributeNode(pAttr); // 统计年份
				pLever02Node->appendChild((MSXML2::IXMLDOMNode*)pElement);
			}
			pNode->appendChild(pLever02Node);
		}

	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_JsonEngine(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("JsonEngine的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("JsonEngine"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_JsonLib(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("JsonLib的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("JsonLib"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_LogEngine(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("LogEngine的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("LogEngine"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_MsXmlEngine(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("MsXmlEngine的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("MsXmlEngine"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_MySqlDbEngine(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("MySqlDbEngine的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("MySqlDbEngine"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_NetComm(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("NetComm的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("NetComm"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_NetSocketServerTest(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("NetSocketServerTest的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("NetSocketServerTest"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_SchedulerCenter(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("SchedulerCenter的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("SchedulerCenter"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_SecurityCollection(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("SecurityCollection的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("SecurityCollection"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_ServiceModular(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("ServiceModular的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("ServiceModular"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_SignalToDriveceModular(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("SignalToDriveceModular的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("SignalToDriveceModular"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_SignalToServiceModular(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("SignalToServiceModular的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("SignalToServiceModular"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_SKFEncry(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("SKFEncry的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("SKFEncry"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_Sqlite3DbEngine(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("Sqlite3DbEngine的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("Sqlite3DbEngine"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_SqliteLib(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("SqliteLib的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("SqliteLib"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_TelnetServer(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("TelnetServer的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("TelnetServer"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_XzmSqlite3MfcApp(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("XzmSqlite3MfcApp的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("XzmSqlite3MfcApp"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_XzmSqlite3WtlApp(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("XzmSqlite3WtlApp的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("XzmSqlite3WtlApp"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}

	return TRUE;
}

BOOL CMsXmlBase::SysCfg_ZLib(IXZMDocumentPtr& pIXZMDocumentPtr,IXZMElementPtr& pRootElement)
{
	IXZMCommentPtr pComment      = NULL;// 注释标签指针
	IXZMNodePtr    pNode         = NULL;// 节点标签指针
	IXZMElementPtr pElement      = NULL;// 元素标签指针
	IXZMAttributePtr pAttr       = NULL;// 属性标签指针

	if (TRUE) {
		// 创建注释标签，并将其挂载到SysCfg节点下
		pComment = NULL;
		pComment = CreateComment(pIXZMDocumentPtr,_T("ZLib的XML注释"));
		pRootElement->appendChild((IXZMNode*)pComment);	

		// 创建节点标签，并将其挂在到SysCfg节点下
		pNode = NULL;
		pNode = CreateNode(pIXZMDocumentPtr,_T("ZLib"),_T("NodeValue"));
		pRootElement->appendChild(pNode);
	}


	return TRUE;
}

void CMsXmlBase::OpenSysCfgXml(TCHAR* filePath,
	TCHAR* ServerMajorIP, LONG& ServerMajorPort,
	TCHAR* ServerMinorIP, LONG& ServerMinorPort,
	TCHAR* ServerSpareIP, LONG& ServerSparePort)
{
	ServerMajorPort = 100;
	ServerMajorPort = 200;
	ServerMajorPort = 300;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////              封装的内部核心函数            /////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 文档接口实例化
//
// pIXZMDocumentPtr
//////////////////////////////////////////////////////////////////////////
BOOL CMsXmlBase::CreateDoc(IXZMDocumentPtr& pIXZMDocumentPtr){
	HRESULT hr = pIXZMDocumentPtr.CreateInstance("Msxml2.DOMDocument.6.0");//__uuidof(DOMDocument40)
	if (FAILED(hr))
	{
		printf("无法创建DOMDocument40对象，请检查是否安装并初始化了MsXml Parser库！");
		return FALSE;
	}
	else{
		return TRUE;
	}
}

// 创建声明
//
// pIXZMDocumentPtr
// pstrTargetName
// pstrData
//////////////////////////////////////////////////////////////////////////
BOOL CMsXmlBase::CreateStatement(IXZMDocumentPtr& pIXZMDocumentPtr,
	TCHAR* pstrTargetName,TCHAR* pstrData){
		IXZMProcessingInstructionPtr pIXZMProcessingInstructionPtr = 
			pIXZMDocumentPtr->createProcessingInstruction((_bstr_t)pstrTargetName,(_bstr_t)pstrData); 
		pIXZMDocumentPtr->appendChild((IXZMNode*)pIXZMProcessingInstructionPtr);

		return TRUE;
}
// 创建元素
//
// pIXZMDocumentPtr
// pComment
//////////////////////////////////////////////////////////////////////////
IXZMElementPtr CMsXmlBase::CreateElement(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pstrElemName,TCHAR* pstrElemValue /*= _T("")*/){
	IXZMElementPtr pElement = NULL;
	pElement = pIXZMDocumentPtr->createElement((_bstr_t)pstrElemName);
	pElement->Puttext((_bstr_t)pstrElemValue);
	return pElement;
}
// 创建根节点
//
// pIXZMDocumentPtr
// pstrRootName
//////////////////////////////////////////////////////////////////////////
IXZMElementPtr CMsXmlBase::CreateRoot(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pstrRootName){
	IXZMElementPtr pRootElement = NULL;
	pRootElement = pIXZMDocumentPtr->createElement((_bstr_t)pstrRootName);
	pIXZMDocumentPtr->PutRefdocumentElement(pRootElement);
	return pRootElement;
}
// 创建节点
//
// pstrNodeName
// pstrNodeText
// pstrXmlnsName
//////////////////////////////////////////////////////////////////////////
IXZMNodePtr CMsXmlBase::CreateNode(IXZMDocumentPtr& pIXZMDocumentPtr,
	TCHAR* pstrNodeName,TCHAR* pstrNodeText,TCHAR* pstrXmlnsName /*= _T("")*/){
		IXZMNodePtr pNode = NULL;
		pNode = pIXZMDocumentPtr->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)pstrNodeName, (_bstr_t)pstrXmlnsName);
		pNode->Puttext((_bstr_t)pstrNodeText);
		return pNode;
}
// 创建注释
//
// pIXZMDocumentPtr
// pCommentName
//////////////////////////////////////////////////////////////////////////
IXZMCommentPtr CMsXmlBase::CreateComment(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pCommentName){
	IXZMCommentPtr pComment = 
		pIXZMDocumentPtr->createComment((_bstr_t)pCommentName);
	return pComment;
}
// 创建属性
//
// pIXZMDocumentPtr
// pAttrName
// pAttrValue
//////////////////////////////////////////////////////////////////////////
IXZMAttributePtr CMsXmlBase::CreateAttr(IXZMDocumentPtr& pIXZMDocumentPtr,
	TCHAR* pAttrName,TCHAR* pAttrValue){
		IXZMAttributePtr pAttrNode = NULL;
		pAttrNode = pIXZMDocumentPtr->createAttribute((_bstr_t)pAttrName);
		pAttrNode->Puttext((_bstr_t)pAttrValue);
		return pAttrNode;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////              封装的内部核心函数            /////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////