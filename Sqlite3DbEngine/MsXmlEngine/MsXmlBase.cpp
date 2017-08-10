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
	TCHAR *szXmlFile = filePath; // xml�ļ�
	MSXML2::IXMLDOMDocumentPtr pDoc = NULL; // xml�ĵ�
	MSXML2::IXMLDOMProcessingInstructionPtr pProInstruction = NULL; // xml����
	MSXML2::IXMLDOMCommentPtr pComment = NULL; // ע��
	MSXML2::IXMLDOMElementPtr pRootElement = NULL, pElement = NULL; // ���ڵ�(Ԫ��)
	MSXML2::IXMLDOMNodePtr pNode = NULL, pNode1 = NULL, pNode2 = NULL; // �ڵ�
	MSXML2::IXMLDOMAttributePtr pAttrNode = NULL; // ����

	HRESULT hr = pDoc.CreateInstance("Msxml2.DOMDocument.6.0");//__uuidof(DOMDocument40)
	if (FAILED(hr))
	{
		printf("�޷�����DOMDocument40���������Ƿ�װ����ʼ����MsXml Parser�⣡");
		return EXIT_FAILURE;
	}


	// (1)����xml�ĵ�����(��insertBefore���ڵ�)
	pProInstruction = pDoc->createProcessingInstruction((_bstr_t)(TCHAR*)_T("xml"),
		(_bstr_t)(TCHAR*)_T("version=\"1.0\" encoding=\"utf-8\"")); 
	pDoc->appendChild((MSXML2::IXMLDOMNode*)pProInstruction);

	// (2)�������ڵ�<China>
	pRootElement =pDoc->createElement((_bstr_t)(TCHAR*)_T("China"));
	pDoc->PutRefdocumentElement(pRootElement); // pXMLDomDoc->documentElement = pRootElement;

	// (3)�����ڵ�<China><Continent>
	pComment = pDoc->createComment((_bstr_t)(TCHAR*)_T("���ڵ���"));
	pRootElement->appendChild((MSXML2::IXMLDOMNode*)pComment); // ע��

	pNode = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("Continent"), (_bstr_t)(TCHAR*)_T("dddd"));
	pNode->Puttext((_bstr_t)(TCHAR*)_T("Asia")); // pNode->text = "Asia";
	pRootElement->appendChild(pNode); // �ڵ�

	// (4)�����ڵ�<China><Population>
	pComment = pDoc->createComment((_bstr_t)(TCHAR*)_T("�˿�����"));
	pRootElement->appendChild((MSXML2::IXMLDOMNode*)pComment); // ע��

	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Population"));
	pAttrNode = pDoc->createAttribute((_bstr_t)(TCHAR*)_T("Units"));
	pAttrNode->Puttext((_bstr_t)(TCHAR*)_T("Million Person"));
	pElement->setAttributeNode(pAttrNode); // ͳ�Ƶ�λ
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("StatisticalYear"), (_variant_t)(TCHAR*)_T("2000")); // ͳ�����
	pElement->Puttext((_bstr_t)(TCHAR*)_T("1,296"));
	pRootElement->appendChild(pElement); // �ڵ�

	// (5)�����ڵ�<China><Municipality>
	pComment = pDoc->createComment((_bstr_t)(TCHAR*)_T("�ĸ�ֱϽ��"));
	pRootElement->appendChild((MSXML2::IXMLDOMNode*)pComment); // ע��

	pNode = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("Municipality"), (_bstr_t)(TCHAR*)_T(""));
	pRootElement->appendChild(pNode); // �ڵ�

	// (6)�����ڵ�<China><Municipality><TianJin>
	pNode1 = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("TianJin"), (_bstr_t)(TCHAR*)_T(""));

	//�����ڵ�<China><Municipality><TianJin><Area>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Area"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Thousand Square kilometers")); // ͳ�Ƶ�λ
	pElement->Puttext((_bstr_t)(TCHAR*)_T("12"));
	pNode1->appendChild((MSXML2::IXMLDOMNode*)pElement); // �ڵ�

	//�����ڵ�<China><Municipality><TianJin><Population>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Population"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Million Person")); // ͳ�Ƶ�λ
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("StatisticalYear"), (_variant_t)(TCHAR*)_T("2000")); // ͳ�����
	pElement->Puttext((_bstr_t)(TCHAR*)_T("10.01"));
	pNode1->appendChild((MSXML2::IXMLDOMNode*)pElement); // �ڵ�

	pNode->appendChild(pNode1);
	// (7)�����ڵ�<China><Municipality><BeiJing>������<TianJin>ǰ
	pNode2 = pDoc->createNode((_variant_t)(long)XZMNodeType::NODE_ELEMENT, (_bstr_t)(TCHAR*)_T("BeiJing"), (_bstr_t)(TCHAR*)_T(""));

	//�����ڵ�<China><Municipality><BeiJing><Area>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Area"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Thousand Square kilometers")); // ͳ�Ƶ�λ
	pElement->Puttext((_bstr_t)(TCHAR*)_T("17"));
	pNode2->appendChild((MSXML2::IXMLDOMNode*)pElement); // �ڵ�

	//�����ڵ�<China><Municipality><BeiJing><Population>
	pElement = pDoc->createElement((_bstr_t)(TCHAR*)_T("Population"));
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("Units"), (_variant_t)(TCHAR*)_T("Million Person")); // ͳ�Ƶ�λ
	pElement->setAttribute((_bstr_t)(TCHAR*)_T("StatisticalYear"), (_variant_t)(TCHAR*)_T("2000")); // ͳ�����
	pElement->Puttext((_bstr_t)(TCHAR*)_T("13.82"));
	pNode2->appendChild((MSXML2::IXMLDOMNode*)pElement); // �ڵ�

	pNode->insertBefore(pNode2, (_variant_t)(IDispatch*)pNode1);
	//
	// (8)�����ڵ�<China><Municipality><ShangHai>
	// (9)�����ڵ�<China><Municipality><ChongQing>

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
	pComment1 = CreateComment(pIXZMDocumentPtr,_T("���ڵ���"));
	pRootElement->appendChild((IXZMNode*)pComment1); // ע��


	IXZMNodePtr pNode1 = NULL;
	pNode1 = CreateNode(pIXZMDocumentPtr,_T("Continent"),_T("Asia"),_T("dddd"));
	pRootElement->appendChild(pNode1); // �ڵ�

	IXZMCommentPtr pComment2 = NULL;
	pComment2 = CreateComment(pIXZMDocumentPtr,_T("�˿�����"));
	pRootElement->appendChild((IXZMNode*)pComment2); // ע��

	IXZMElementPtr pElement1 = NULL;
	pElement1 = CreateElement(pIXZMDocumentPtr,_T("Population"),_T("1,296"));

	IXZMAttributePtr pAttr1 = NULL;
	pAttr1 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
	pElement1->setAttributeNode(pAttr1); // ͳ�Ƶ�λ
	pAttr1 = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("2000"));
	pElement1->setAttributeNode(pAttr1); // ͳ�����

	pRootElement->appendChild(pElement1); // �ڵ�

	IXZMCommentPtr pComment3 = NULL;
	pComment3 = CreateComment(pIXZMDocumentPtr,_T("�ĸ�ֱϽ��"));
	pRootElement->appendChild((IXZMNode*)pComment3); // ע��

	IXZMNodePtr pNode2 = NULL;
	pNode2 = CreateNode(pIXZMDocumentPtr,_T("Municipality"),_T(""),_T("dddd"));
	pRootElement->appendChild(pNode2); // �ڵ�

	IXZMNodePtr pNode21 = NULL;
	pNode21 = CreateNode(pIXZMDocumentPtr,_T("TianJin"),_T(""),_T(""));

	IXZMElementPtr pElement2 = NULL;
	pElement2 = CreateElement(pIXZMDocumentPtr,_T("Population"),_T("1,296"));

	IXZMAttributePtr pAttr2 = NULL;
	pAttr2 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
	pElement2->setAttributeNode(pAttr2); // ͳ�Ƶ�λ
	pAttr2 = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("10.01"));
	pElement2->setAttributeNode(pAttr2); // ͳ�����
	pNode21->appendChild((MSXML2::IXMLDOMNode*)pElement2);

	IXZMElementPtr pElement3 = NULL;
	pElement3 = CreateElement(pIXZMDocumentPtr,_T("Area"),_T("17"));

	IXZMAttributePtr pAttr3 = NULL;
	pAttr3 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Thousand Square kilometers"));
	pElement3->setAttributeNode(pAttr3); // ͳ�Ƶ�λ
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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�-IXZMElementPtr"));
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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�-IXZMElementPtr"));

	IXZMCommentPtr pComment = NULL;
	pComment = CreateComment(pIXZMDocumentPtr,_T("ע��Ԫ��"));
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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("Ԫ�ؽڵ�-IXZMElementPtr"));

	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,
		_T("�ӽڵ�-IXZMNodePtr"),
		_T("�ӽڵ�ֵ"),
		_T("xzm-xml_space"));//xml�������ռ䣬�е�������namespaces�Ƶ�
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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�-IXZMElementPtr"));



	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("�ӽڵ�-IXZMNodePtr"),_T(""),_T("dddd"));
	pRootElement->appendChild(pNode);

	IXZMNodePtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("�ӽڵ���ӽڵ�-IXZMNodePtr"),_T(""),_T(""));

	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("Ԫ�ؽڵ�-IXZMElementPtr"),_T("ֵ"));

	IXZMAttributePtr pAttr_1_1 = NULL;
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("����һ"),_T("����value01"));
	pElement_1->setAttributeNode(pAttr_1_1);
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("���Զ�"),_T("����value02"));
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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�-IXZMElementPtr"));



	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("Municipality"),_T(""),_T("dddd"));
	pRootElement->appendChild(pNode); // �ڵ�

	IXZMNodePtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("TianJin"),_T(""),_T(""));

	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("Population"),_T("1,296"));

	IXZMAttributePtr pAttr_1_1 = NULL;
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Million Person"));
	pElement_1->setAttributeNode(pAttr_1_1); // ͳ�Ƶ�λ
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("StatisticalYear"),_T("10.01"));
	pElement_1->setAttributeNode(pAttr_1_1); // ͳ�����
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_1);

	IXZMElementPtr pElement_2 = NULL;
	pElement_2 = CreateElement(pIXZMDocumentPtr,_T("Area"),_T("17"));

	IXZMAttributePtr pAttr_2_1 = NULL;
	pAttr_2_1 = CreateAttr(pIXZMDocumentPtr,_T("Units"),_T("Thousand Square kilometers"));
	pElement_2->setAttributeNode(pAttr_2_1); // ͳ�Ƶ�λ
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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�"));



	IXZMNodePtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("Ԫ��"),_T(""),_T(""));
	pRootElement->appendChild(pNode);
	IXZMAttributePtr pAttr_0_0 = NULL;
	pAttr_0_0 = CreateAttr(pIXZMDocumentPtr,_T("����"),_T("ͼ��"));
	pRootElement->setAttributeNode(pAttr_0_0);


	IXZMElementPtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("�ڵ�"),_T(""),_T(""));

	IXZMAttributePtr pAttr_x_x = NULL;
	pAttr_x_x = CreateAttr(pIXZMDocumentPtr,_T("num"),_T("01"));
	pNodeChild->setAttributeNode(pAttr_x_x);

	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("Ԫ������1"),_T("ʲô����"));

	IXZMAttributePtr pAttr_1_1 = NULL;
	pAttr_1_1 = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_1->setAttributeNode(pAttr_1_1); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_1);

	IXZMElementPtr pElement_2 = NULL;
	pElement_2 = CreateElement(pIXZMDocumentPtr,_T("Ԫ������1"),_T("ʲô����"));

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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�"));



	IXZMElementPtr pNode = NULL;
	pNode = CreateNode(pIXZMDocumentPtr,_T("Ԫ��"),_T(""),_T(""));
	pRootElement->appendChild(pNode);

	IXZMAttributePtr pAttr_0_0 = NULL;
	pAttr_0_0 = CreateAttr(pIXZMDocumentPtr,_T("����"),_T("ͼ��"));
	pNode->setAttributeNode(pAttr_0_0);


	IXZMElementPtr pNodeChild = NULL;
	pNodeChild = CreateNode(pIXZMDocumentPtr,_T("�ڵ�"),_T(""),_T(""));


	IXZMAttributePtr pAttr_x_x = NULL;
	pAttr_x_x = CreateAttr(pIXZMDocumentPtr,_T("num"),_T("01"));
	pNodeChild->setAttributeNode(pAttr_x_x);

	IXZMAttributePtr pAttr = NULL;


	IXZMElementPtr pElement_1 = NULL;
	pElement_1 = CreateElement(pIXZMDocumentPtr,_T("ͼƬλ��"),_T("D:\\CustomCode\\AllXzyUpdate\\trunk\\DiagramEditor\\1.png"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_1->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_1);

	IXZMElementPtr pElement_2 = NULL;
	pElement_2 = CreateElement(pIXZMDocumentPtr,_T("ͼƬλ��"),_T("D:\\CustomCode\\AllXzyUpdate\\trunk\\DiagramEditor\\2.png"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_2->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_2);

	IXZMElementPtr pElement_3 = NULL;
	pElement_3 = CreateElement(pIXZMDocumentPtr,_T("ˮƽ����"),_T("100"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_3->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_3);

	IXZMElementPtr pElement_4 = NULL;
	pElement_4 = CreateElement(pIXZMDocumentPtr,_T("��ֱ����"),_T("200"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_4->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_4);

	IXZMElementPtr pElement_5 = NULL;
	pElement_5 = CreateElement(pIXZMDocumentPtr,_T("��"),_T("480"));
	pAttr = CreateAttr(pIXZMDocumentPtr,_T("unused"),_T("unused"));
	pElement_5->setAttributeNode(pAttr); 
	pNodeChild->appendChild((MSXML2::IXMLDOMNode*)pElement_5);

	IXZMElementPtr pElement_6 = NULL;
	pElement_6 = CreateElement(pIXZMDocumentPtr,_T("��"),_T("600"));
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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�-IXZMElementPtr"));



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
	pRootElement = CreateRoot(pIXZMDocumentPtr,_T("���ڵ�-IXZMElementPtr"));



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
	//����<person>�ڵ�.  
	MSXML2::IXMLDOMNodeListPtr pPersonNodes = pRootNode->selectNodes(_T("Ԫ��")); 
	long size = pPersonNodes->length;
	for (int i = 0; i<size; i++) {
		MSXML2::IXMLDOMNodePtr pPersonNode = pPersonNodes->item[i];
		OutputDebugString(pPersonNode->text);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////              ��װ���ڲ����ĺ���            /////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �ĵ��ӿ�ʵ����
//
// pIXZMDocumentPtr
//////////////////////////////////////////////////////////////////////////
BOOL CMsXmlBase::CreateDoc(IXZMDocumentPtr& pIXZMDocumentPtr){
	HRESULT hr = pIXZMDocumentPtr.CreateInstance("Msxml2.DOMDocument.6.0");//__uuidof(DOMDocument40)
	if (FAILED(hr))
	{
		printf("�޷�����DOMDocument40���������Ƿ�װ����ʼ����MsXml Parser�⣡");
		return FALSE;
	}
	else{
		return TRUE;
	}
}

// ��������
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
// ����Ԫ��
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
// �������ڵ�
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
// �����ڵ�
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
// ����ע��
//
// pIXZMDocumentPtr
// pCommentName
//////////////////////////////////////////////////////////////////////////
IXZMCommentPtr CMsXmlBase::CreateComment(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pCommentName){
	IXZMCommentPtr pComment = 
		pIXZMDocumentPtr->createComment((_bstr_t)pCommentName);
	return pComment;
}
// ��������
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
/////////              ��װ���ڲ����ĺ���            /////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////