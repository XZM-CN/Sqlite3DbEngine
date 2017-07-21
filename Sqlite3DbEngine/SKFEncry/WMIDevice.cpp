// WMIDevice.cpp : CWMIDevice ��ʵ��

#include "stdafx.h"
#include "WMIDevice.h"
#include <Wbemidl.h>
#include <algorithm>
#include <atlconv.h>
#include <ntddndis.h>
#include <WinIoCtl.h>
#include <strsafe.h>
#include "BaseFuncLib.h"
#include "WMIDevice.h"

#pragma warning( disable : 4995 )

#pragma comment (lib, "comsuppw.lib")
#pragma comment (lib, "wbemuuid.lib")

typedef struct T_WQL_QUERY
{
	CHAR*	szSelect;		// SELECT���
	WCHAR*	szProperty;		// �����ֶ�
} HXT_WQL_QUERY;

// WQL��ѯ���
const HXT_WQL_QUERY g_szWQLQuery[] = {
	// ����ԭ��MAC��ַ
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",
	L"PNPDeviceID",

	// Ӳ�����к�
	"SELECT SerialNumber FROM Win32_PhysicalMedia WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",

	// �������к�
	"SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",	

	// ������ID
	"SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)",
	L"ProcessorId",

	// BIOS���к�
	"SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",

	// U��
	"SELECT * FROM Win32_USBHub",
	L"DeviceID",

	// �߼���
	"SELECT * FROM Win32_LogicalDisk",
	L"DeviceID",
};

// CWMIDevice

BOOL CWMIDevice::WMI_DoWithPNPDeviceID( const TCHAR *PNPDeviceID, TCHAR *MacAddress, UINT uSize )
{
	BOOL	isOK = FALSE;
	TCHAR	DevicePath[MAX_PATH];
	HANDLE	hDeviceFile = NULL;	

	memset(DevicePath,NULL,MAX_PATH*sizeof(TCHAR));

	// �����豸·����
	StringCchCopy( DevicePath, MAX_PATH, TEXT("\\\\.\\Global\\") );
	StringCchCat( DevicePath, MAX_PATH, PNPDeviceID );
	StringCchCat( DevicePath, MAX_PATH, TEXT("#{ad498944-762f-11d0-8dcb-00c04fc3358c}") );
	// ����PNPDeviceID���еġ�\���滻�ɡ�#�����Ի���������豸·����
	std::replace( DevicePath + 4, DevicePath + 4 + _tcslen(PNPDeviceID), TEXT('\\'), TEXT('#') ); 

	// ��ȡ�豸���
	hDeviceFile = CreateFile( DevicePath,0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,OPEN_EXISTING,0,NULL);
	if( hDeviceFile != INVALID_HANDLE_VALUE )
	{	
		ULONG	dwID;
		BYTE	ucData[8];
		DWORD	dwByteRet;		

		// ��ȡ����ԭ��MAC��ַ
		dwID = OID_802_3_PERMANENT_ADDRESS;
		isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );
		if( isOK )
		{	
			// ���ֽ�����ת����16�����ַ���
			for( DWORD i = 0; i < dwByteRet; i++ )
			{
				StringCchPrintf( MacAddress + (i << 1), uSize - (i << 1), TEXT("%02X"), ucData[i] );
			}
		}

		::CloseHandle( hDeviceFile );
		hDeviceFile = NULL;
	}

	return isOK;
}

BOOL CWMIDevice::WMI_DoWithHarddiskSerialNumber( TCHAR *SerialNumber, UINT uSize )
{
	UINT iLen = 0,i = 0;

	iLen = (UINT )_tcslen( SerialNumber );
	// InterfaceType = "IDE"
	if( iLen == 40 )	
	{	
		// ��Ҫ��16���Ʊ��봮ת��Ϊ�ַ���
		TCHAR ch, szBuf[32];
		BYTE b;		

		for( i = 0; i < 20; i++ )
		{	// ��16�����ַ�ת��Ϊ��4λ
			ch = SerialNumber[i * 2];
			if( (ch >= '0') && (ch <= '9') )
			{
				b = ch - '0';
			}
			else if( (ch >= 'A') && (ch <= 'F') )
			{
				b = ch - 'A' + 10;
			}
			else if( (ch >= 'a') && (ch <= 'f') )
			{
				b = ch - 'a' + 10;
			}
			else
			{	// �Ƿ��ַ�
				break;
			}

			b <<= 4;

			// ��16�����ַ�ת��Ϊ��4λ
			ch = SerialNumber[i * 2 + 1];
			if( (ch >= '0') && (ch <= '9') )
			{
				b += ch - '0';
			}
			else if( (ch >= 'A') && (ch <= 'F') )
			{
				b += ch - 'A' + 10;
			}
			else if( (ch >= 'a') && (ch <= 'f') )
			{
				b += ch - 'a' + 10;
			}
			else
			{	// �Ƿ��ַ�
				break;
			}

			szBuf[i] = b;
		}

		if( i == 20 )
		{
			// ת���ɹ�
			szBuf[i] = L'\0';
			StringCchCopy( SerialNumber, uSize, szBuf );
			iLen = (UINT )_tcslen( SerialNumber );
		}
	}

	// ÿ2���ַ�����λ��
	for( i = 0; i < iLen; i += 2 )
	{
		std::swap( SerialNumber[i], SerialNumber[i+1] );
	}

	// ȥ���ո�
	std::remove( SerialNumber, SerialNumber + _tcslen(SerialNumber) + 1, L' ' );
	return TRUE;
}

BOOL CWMIDevice::WMI_DoWithProperty( INT iQueryType, TCHAR *szProperty, UINT uSize )
{
	BOOL isOK = TRUE;

	switch( iQueryType )
	{
	case WMITYPE_NETMACADDR:// ����ԭ��MAC��ַ		
		isOK = WMI_DoWithPNPDeviceID( szProperty, szProperty, uSize );
		break;
	case WMITYPE_HARDID:	// Ӳ�����к�
		isOK = WMI_DoWithHarddiskSerialNumber( szProperty, uSize );
		break;
	default:
		break;
	}

	return isOK;
}

STDMETHODIMP CWMIDevice::Connect(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL != m_spiSvc)
		return S_OK;
	HRESULT hRet = ::CoCreateInstance( 
		CLSID_WbemLocator,NULL, 
		CLSCTX_INPROC_SERVER,IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&m_spiLoc));
	if(FAILED(hRet))
	{
	}
	if(SUCCEEDED(hRet) && NULL != m_spiLoc)
	{
		hRet = m_spiLoc->ConnectServer(_bstr_t( L"ROOT\\CIMV2" ),
			NULL,NULL,NULL,0,NULL,NULL,&m_spiSvc);
		if(SUCCEEDED(hRet))
		{
			// �����������İ�ȫ����
			hRet = CoSetProxyBlanket(m_spiSvc,
				RPC_C_AUTHN_WINNT,RPC_C_AUTHZ_NONE,NULL,
				RPC_C_AUTHN_LEVEL_CALL,
				RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE);
		}
	}
	ATLASSERT(m_spiSvc);
	return hRet;
}

STDMETHODIMP CWMIDevice::Query(EWMIType eWMIType,BSTR bstrKey,SHORT* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	m_QueryIDList.clear();
	HRESULT hRet(E_FAIL);
	if(NULL == m_spiSvc)
		hRet = this->Connect();
	if(NULL == m_spiSvc)
	{
		return hRet;
	}
	CComPtr <IEnumWbemClassObject> spiEnumerator = NULL;
	hRet = m_spiSvc->ExecQuery(bstr_t("WQL"), 
		bstr_t( g_szWQLQuery[eWMIType].szSelect ),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,&spiEnumerator);
	if(FAILED(hRet) || NULL == spiEnumerator)
	{
		return hRet;
	}
	CComVariant vtProperty;		
	ULONG uReturn = 0;
	CString strQueryVal(_T(""));

	// ѭ��ö�����еĽ������  
	while( NULL != spiEnumerator )
	{
		uReturn = 0;
		CComPtr <IWbemClassObject> spiClsObj = NULL;
		hRet = spiEnumerator->Next(WBEM_INFINITE,1, 
			&spiClsObj,&uReturn);
		if( uReturn == 0 )
			break;
		VariantInit( &vtProperty );	
		spiClsObj->Get(g_szWQLQuery[eWMIType].szProperty,0,&vtProperty,NULL,NULL);
		strQueryVal = vtProperty.bstrVal;
		VariantClear(&vtProperty);
		// ������ֵ����һ���Ĵ���
		WMI_DoWithProperty( eWMIType, (LPTSTR)(LPCTSTR)strQueryVal, 128 );
		strQueryVal.Replace(_T(" "),_T(""));
		strQueryVal.Replace(_T(":"),_T(""));
		m_QueryIDList.push_back(strQueryVal);
		strQueryVal.Empty();
		/// ȡ����
		VariantClear(&vtProperty);
		spiClsObj->Get(_T("Name"),0,&vtProperty,NULL,NULL);
		strQueryVal = vtProperty.bstrVal;
		VariantClear(&vtProperty);
		m_QueryNameList.push_back(strQueryVal);
		strQueryVal.Empty();

		///��ȡָ�����ֵ�ֵ
		if(NULL != bstrKey)
		{
			VariantClear(&vtProperty);
			spiClsObj->Get(bstrKey,0,&vtProperty,NULL,NULL);
			vtProperty.ChangeType(VT_BSTR);
			strQueryVal = vtProperty.bstrVal;
			VariantClear(&vtProperty);
			strQueryVal.Replace(_T(" "),_T(""));
			strQueryVal.Replace(_T(":"),_T(""));
			m_QueryKeyList.push_back(strQueryVal);
			strQueryVal.Empty();
		}

		spiClsObj = NULL;
	}
	spiEnumerator = NULL;

	(*pVal) = (SHORT)m_QueryIDList.size();
	return S_OK;
}

STDMETHODIMP CWMIDevice::GetValue(SHORT nIndex,BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal || nIndex < 0)
		return E_POINTER;
	if((unsigned )nIndex >= m_QueryIDList.size())
		return E_INVALIDARG;
	CComBSTR bstrVal(m_QueryIDList[nIndex]);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CWMIDevice::GetKeyValue(SHORT nIndex,BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal || nIndex < 0)
		return E_POINTER;
	if((unsigned )nIndex >= m_QueryKeyList.size())
		return E_INVALIDARG;
	CComBSTR bstrVal(m_QueryKeyList[nIndex]);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CWMIDevice::GetName(SHORT nIndex,BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal || nIndex < 0)
		return E_POINTER;
	if((unsigned )nIndex >= m_QueryNameList.size())
		return E_INVALIDARG;
	CComBSTR bstrVal(m_QueryNameList[nIndex]);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CWMIDevice::Close(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_QueryNameList.clear();
	m_QueryIDList.clear();
	m_QueryKeyList.clear();
	if(NULL != m_spiSvc)
		m_spiSvc = NULL;
	if(NULL != m_spiLoc)
		m_spiLoc = NULL;
	return S_OK;
}