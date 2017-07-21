// AccountMgr.cpp : CAccountMgr ��ʵ��

#include "stdafx.h"
#include "AccountMgr.h"
#include "SKFHelper.h"
#include "SqliteHelper.h"
#include "openssl/aes.h"

#define	TDHXKJ_ACCOUNTSTATUS_LOCK	1

// CAccountMgr
CString CAccountMgr::CreateTable(CComPtr <ISafeService>& spiSafeService)
{
	CString strAdminPW;
	if(NULL == spiSafeService)
		return strAdminPW;
	strAdminPW = GetSafePW(spiSafeService,m_strDataPW,ACCOUNTTYPE_SA);
	if(m_strDataPW.IsEmpty())
	{
		/// û�в��뿨
		if(m_strLastErr.IsEmpty())
		{
#ifdef TDHXKJ_VERSION_NOUSB
			m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
			m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
		}
#ifdef APP_LOG_ENABLE
//		WRITELOGTOFILE(m_strLastErr);
#endif
		return strAdminPW;
	}

	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("��ʼ��ʱ�����ݿ�ʧ��"));
#endif
		return strAdminPW;
	}
	HRESULT hRet(S_OK);
	CString strSqlCmd(_T(""));
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	CString strTableName(SQLITEDB_TABLE_ACCOUNT);
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼�����˺���Ϣ��LoginID/ShowName/EncodePassword/CheckCode/LoginTime/UserRight/Status(״̬�������¼�Ƿ�����)/UserType/SyncFlagͬ�����
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			LID VARCHAR DEFAULT '' NOT NULL PRIMARY KEY UNIQUE,\
			SName VARCHAR DEFAULT '' NOT NULL,\
			EPass VARCHAR DEFAULT '' NOT NULL,\
			CCode VARCHAR DEFAULT '' NOT NULL,\
			LTime DOUBLE DEFAULT '' NOT NULL,\
			URight INTEGER DEFAULT '0' NOT NULL,\
			Status INTEGER DEFAULT '0' NOT NULL,\
			UType INTEGER DEFAULT '0' NOT NULL,\
			SyncFlag INTEGER DEFAULT '0' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		if(SUCCEEDED(hRet))
		{
#ifdef APP_LOG_ENABLE
//			WRITELOGTOFILE(_T("����ϵͳ�˺ű�ɹ�"));
#endif
		}
		strSqlCmd.Empty();
	}
	strTableName.Empty();
	/// ����U�̰�������
	strTableName = SQLITEDB_TABLE_USBWHITE;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼U�̰�������Ϣ��UID/UDisk/MakerName/AddTime/Size/UType(�����Լ���U�̻��������ģ�0���Լ���ɾ��Ҫ�ر������û�)
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			UID VARCHAR DEFAULT '' NOT NULL PRIMARY KEY UNIQUE,\
			UDisk VARCHAR DEFAULT '' NOT NULL,\
			MName VARCHAR DEFAULT '' NOT NULL,\
			ATime DOUBLE DEFAULT '' NOT NULL,\
			Size INTEGER DEFAULT '0' NOT NULL,\
			UType INTEGER DEFAULT '0' NOT NULL,\
			SyncFlag INTEGER DEFAULT '0' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	strTableName.Empty();
	/// �����ļ���������
	strTableName = SQLITEDB_TABLE_FILEWHITE;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼�ļ���������Ϣ��FileID/FullPath/CorpName/Version/FileType/Size(�ļ���С)/Flag(���0ɾ��1���2����32�Լ��ĳ���)/UpdateTime/Sha/SyncFlag/Desc(����)
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			FID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
			FullPath VARCHAR DEFAULT '' NOT NULL,\
			CorpName VARCHAR DEFAULT '' NOT NULL,\
			Version VARCHAR DEFAULT '' NOT NULL,\
			Desc VARCHAR DEFAULT '' NOT NULL,\
			Type INTEGER DEFAULT '0' NOT NULL,\
			Size INTEGER DEFAULT '0' NOT NULL,\
			Flag INTEGER DEFAULT '1' NOT NULL,\
			UTime DOUBLE DEFAULT '' NOT NULL,\
			Sha BLOB DEFAULT '' NOT NULL,\
			SyncFlag INTEGER DEFAULT '0' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	strTableName.Empty();
	/// ����ϵͳ���÷����
	strTableName = SQLITEDB_TABLE_STOPSERVICE;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼ϵͳ���÷���� SID��ʶ/SerName��������/ImagePath������·��/ATime���ʱ��/Startԭ������������/Status״̬
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			SID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
			SerName VARCHAR DEFAULT '' NOT NULL,\
			ImagePath VARCHAR DEFAULT '' NOT NULL,\
			ATime DOUBLE DEFAULT '' NOT NULL,\
			Start INTEGER DEFAULT '0' NOT NULL,\
			Status INTEGER DEFAULT '0' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	strTableName.Empty();
	//������ʱ�û���
	strTableName = SQLITEDB_TABLE_TEMP_ACCOUNT;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		OutputDebugString(_T("������ʱ�˺ű�\n"));
		///��¼U�̰�������Ϣ��UID/UDisk/MakerName/AddTime/Size/UType(�����Լ���U�̻��������ģ�0���Լ���ɾ��Ҫ�ر������û�)
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
							LID VARCHAR DEFAULT '' NOT NULL,\
							STempName VARCHAR DEFAULT '' NOT NULL PRIMARY KEY UNIQUE,\
							SName VARCHAR DEFAULT '' NOT NULL,\
							CTempCode VARCHAR DEFAULT '' NOT NULL,\
							CCode VARCHAR DEFAULT '' NOT NULL,\
							LTime DOUBLE DEFAULT '' NOT NULL,\
							URight INTEGER DEFAULT '0' NOT NULL,\
							Status INTEGER DEFAULT '0' NOT NULL,\
							UType INTEGER DEFAULT '0' NOT NULL,\
							SyncFlag INTEGER DEFAULT '0' NOT NULL);"),\
							strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	strTableName.Empty();

	if(NULL != spiSqlite3Connect)
	{
		spiSqlite3Connect->Close();
		spiSqlite3Connect  = NULL;
	}
	return strAdminPW;
}

CComPtr <ISqlite3Connect> CAccountMgr::GetConnect(const CString& strPW,BOOL bReadFlag)
{
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL == spiConnectHelper)
		return NULL;/// �����ӿ�ʧ��
	HRESULT hRet(E_FAIL);
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CString strDataFile = CBaseFuncLib::GetAppDataDir()+TDHX_SQLITEDB_SYSFILE;
	if(!bReadFlag || !CBaseFuncLib::IsPathExist(strDataFile))
	{
		try
		{
			hRet = spiConnectHelper->OpenDB(CComBSTR(strDataFile),VARIANT_TRUE,2*SQLITE_OPEN_DEFAULTOVERTIME,CComBSTR(strPW),&spiSqlite3Connect);
		}
		catch( ... )
		{
		}
		if(FAILED(hRet))
		{
			CComBSTR bstrErrInfo;
			spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
			/// д��־
			if(bstrErrInfo.Length())
			{
				m_strLastErr = bstrErrInfo.m_str;
				WRITELOGTOFILE(bstrErrInfo.m_str);
			}
#endif
			bstrErrInfo.Empty();
		}
		spiConnectHelper = NULL;
		return spiSqlite3Connect;
	}
	CString strReadFile(_T(""));
	CString strModuleName = CBaseFuncLib::GetModuleName(NULL);
	if(0 == strModuleName.CompareNoCase(TDHXKJ_HOSTSERVICE))
		strReadFile = CBaseFuncLib::GetTmpPath()+TDHX_SQLITEDB_SYSTEMPFILE;
	else
		strReadFile = CBaseFuncLib::GetTmpPath()+TDHX_SQLITEDB_SYSFILE;
	BOOL bNeedCopy = FALSE;
	if(!CBaseFuncLib::IsPathExist(strReadFile) || !CBaseFuncLib::GetFileSize(strReadFile))
		bNeedCopy = TRUE;
	else
	{
		COleDateTime FileTime1 = CBaseFuncLib::GetFileWriteTime(strDataFile);
		COleDateTime FileTime2 = CBaseFuncLib::GetFileWriteTime(strReadFile);
		if((FileTime1-FileTime2).GetTotalSeconds() > 0.01)
			bNeedCopy = TRUE;
	}
	BOOL bCopyFlag = FALSE;
	if(bNeedCopy)
	{
		int nIndex = 5;
		/// ���Ƶ���ʱĿ¼ֻ��ʹ��
		bCopyFlag = ::CopyFile(strDataFile,strReadFile,FALSE);
		DWORD dwErrCode = ::GetLastError();
		while(ERROR_ACCESS_DENIED != dwErrCode && !bCopyFlag && nIndex > 0)
		{
			::Sleep(200);
			nIndex--;
			bCopyFlag = ::CopyFile(strDataFile,strReadFile,FALSE);
			dwErrCode = ::GetLastError();
		}
	}
	else
		bCopyFlag = TRUE;
	if(!bNeedCopy || (bNeedCopy && bCopyFlag))
		hRet = spiConnectHelper->OpenDB(CComBSTR(strReadFile),VARIANT_FALSE,SQLITE_OPEN_DEFAULTOVERTIME,CComBSTR(strPW),&spiSqlite3Connect);
	else
		hRet = spiConnectHelper->OpenDB(CComBSTR(strDataFile),VARIANT_FALSE,SQLITE_OPEN_DEFAULTOVERTIME,CComBSTR(strPW),&spiSqlite3Connect);
	if(FAILED(hRet))
	{
		CComBSTR bstrErrInfo;
		spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
		if(bstrErrInfo.Length())
		{
			m_strLastErr = bstrErrInfo.m_str;
#ifdef APP_LOG_ENABLE
			/// д��־
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
	}
	spiConnectHelper = NULL;
	return spiSqlite3Connect;
}

STDMETHODIMP CAccountMgr::get_AccountType(EAccountType* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_eAccountType;
	return S_OK;
}

STDMETHODIMP CAccountMgr::CreateAuth(BSTR bstrDeviceID,BSTR bstrFilePath)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrDeviceID || NULL == bstrFilePath)
		return E_FAIL;
	CString strDeviceID(bstrDeviceID);
	if(strDeviceID.IsEmpty())
	{
		m_strLastErr = _T("�豸��Ϊ�գ�");
		return E_FAIL;
	}

#ifndef TDHXKJ_VERSION_NOUSB
	if(USEAUTHTYPE_AUTHMGR != (m_nRight & USEAUTHTYPE_AUTHMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	CComBSTR bstrVal,bstrDisk;
	HRESULT hRet = spiSafeService->get_CID(VARIANT_TRUE,&bstrDisk,&bstrVal);
	if(FAILED(hRet) || !bstrVal.Length())
	{
		spiSafeService = NULL;
		m_strLastErr = _T("û���ҵ���ȫU�����޷�������Ȩ");
		return E_FAIL;
	}
	CString strCID = bstrVal.m_str;
	bstrVal.Empty();
#ifndef TDHXKJ_VERSION_NOUSB
	CString strInstallDisk;
	CComBSTR bstrInstallDisk;
	hRet = spiSafeService->GetInstallDisk(VARIANT_TRUE,&bstrInstallDisk);
	if(bstrInstallDisk.Length())
		strInstallDisk = bstrInstallDisk.m_str;
	bstrInstallDisk.Empty();
	if(0 != strInstallDisk.CompareNoCase(strCID))
	{
		m_strLastErr = _T("û�в��밲װU����");
		return E_FAIL;
	}
#endif

	CComPtr<ISafeCard> spiSafeCard = NULL;
	hRet = spiSafeService->get_SafeCard(&spiSafeCard);
	if(NULL == spiSafeCard)
	{
		spiSafeService = NULL;
		return E_FAIL;
	}
	hRet = spiSafeCard->put_CurDisk(bstrDisk);

	short nReadLen = TDHXKJ_SKFAPP_FILELEN;
	BYTE BufData[TDHXKJ_SKFAPP_FILELEN] = {0};
	VARIANT_BOOL bRetFlag = VARIANT_FALSE;
	CComPtr <IJsonService> spiJsonService = NULL;
	hRet = spiSafeService->StringSha1(CComBSTR(strCID+_T("_HXSafe")),&bstrVal);
	CString strSha1 = bstrVal.m_str;
	bstrVal.Empty();
	strSha1.Delete(0,16);
	CComBSTR bstrAppName(TDHXKJ_SKFAPP_NAME),bstrUserPin(strSha1.Left(8));
	ULONG nAppHandle = 0;
	hRet = spiSafeCard->OpenApplication(CComBSTR(TDHXKJ_SKFAPP_NAME),&nAppHandle);
	if(FAILED(hRet))
	{
		spiSafeCard = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("��ȫU���쳣��");
		return E_FAIL;
	}
	SHORT nTryCount = 0;
	hRet = spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,bstrUserPin,&nTryCount);
	if(FAILED(hRet))
	{
		spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
		spiSafeCard = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("��ȫU���쳣��");
		return E_FAIL;
	}
	/// ���ȱʡ����
	hRet = spiSafeCard->ReadAppFile(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONFIGFILE),0,&nReadLen,BufData);
	if(FAILED(hRet))
	{
		/// ��ȡ������Ϣ
		CComBSTR bstrErrInfo;
		spiSafeCard->get_LastErrInfo(&bstrErrInfo);
		m_strLastErr = bstrErrInfo.m_str;
#ifdef APP_LOG_ENABLE
		/// д��־
		WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
		bstrErrInfo.Empty();
		spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
		spiSafeCard = NULL;
		spiSafeService = NULL;
		return E_FAIL;
	}
	CString strData(BufData);
	strData.TrimLeft();
	strData.TrimRight();
	if(!strData.IsEmpty())
	{
		if(NULL == spiJsonService)
			spiJsonService = CDbHelper::GetJsonService();
		if(NULL != spiJsonService)
		{
			spiJsonService->put_CodingType(CODINGTYPE_ANSI);
			hRet = spiJsonService->ParseString(CComBSTR(strData),&bRetFlag);
		}
	}
	spiSafeCard->ClearSecureState(nAppHandle);
	spiSafeCard->CloseApplication(nAppHandle);
	spiSafeCard = NULL;
	nAppHandle = 0;
	if(VARIANT_FALSE == bRetFlag)
	{
		spiJsonService = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("��ȫU���쳣��");
		return E_FAIL;
	}
	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	hRet = spiSafeService->get_CryptoStor(&spiCryptoStor);
	if(NULL == spiCryptoStor)
	{
		spiJsonService = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("��ȫU���Ƿ���");
		return E_FAIL;
	}

	strCID.Empty();
	CString strDBPW;
	spiJsonService->GetStringValue(CComBSTR(_T("DataPW")),&bstrVal);
	if(bstrVal.Length())
		strDBPW = bstrVal.m_str;
	bstrVal.Empty();
	ULONG nAuthNode = 0;
	spiJsonService->GetStringValue(CComBSTR(_T("AuthNode")),&bstrVal);
	if(bstrVal.Length())
	{
		nAuthNode = CBaseFuncLib::StrToNum(bstrVal.m_str);
	}
	bstrVal.Empty();
	CComVariant varCustomerID;
	spiJsonService->GetVariantValue(CComBSTR(_T("CustomerID")),&varCustomerID);
	varCustomerID.ChangeType(VT_I4);

	DWORD dwSize = 0;
	spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);

	CString strAuthFile(CBaseFuncLib::GetTmpPath(FALSE)+TDHX_SQLITEDB_AUTHFILE);
	if(CBaseFuncLib::IsPathExist(strAuthFile))
		::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_NORMAL);
	/// �����ȶ�ȡ
	hRet = spiCryptoStor->ReadOnlyFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE),CComBSTR(strAuthFile));
	/// ���Դ�
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL != spiConnectHelper)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(strDBPW),&spiSqlite3Connect);
		if(NULL == spiSqlite3Connect)
		{
			CComBSTR bstrErrInfo;
			spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		spiConnectHelper = NULL;
	}
	if(NULL == spiSqlite3Connect)
	{
		spiJsonService = NULL;
		spiCryptoStor->CloseDisk();
		spiCryptoStor = NULL;
		spiSafeService = NULL;
		return hRet;
	}
	ULONG nUsedCount = 0;
	/// ������Ȩ��
	CString strSqlCmd(_T(""));
	CString strTableName = SQLITEDB_TABLE_AUTHLIST;
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼�����Ȩ��ϢAuthAutoID/DeviceID/AuthCode/CorpID/CorpName/AuthTime
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			AID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
			DID VARCHAR DEFAULT '' NOT NULL,\
			Code VARCHAR DEFAULT '' NOT NULL,\
			CorpID INTEGER DEFAULT '0' NOT NULL,\
			CorpName VARCHAR DEFAULT '' NOT NULL,\
			ATime DOUBLE DEFAULT '' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	else
	{
		strSqlCmd.Format(_T("SELECT Code FROM [%s] WHERE CorpID=%ld"),\
			strTableName,varCustomerID.lVal);
		nUsedCount = CDbHelper::GetRecordCount(spiSqlite3Connect,strSqlCmd);
	}

	CString strValidDay,strPacket,strCustomerName;
	spiJsonService->GetStringValue(CComBSTR(_T("ValidDay")),&bstrVal);
	if(bstrVal.Length())
		strValidDay = bstrVal.m_str;
	bstrVal.Empty();
	spiJsonService->GetStringValue(CComBSTR(_T("AuthPacket")),&bstrVal);
	if(bstrVal.Length())
		strPacket = bstrVal.m_str;
	bstrVal.Empty();
	spiJsonService->GetStringValue(CComBSTR(_T("CustomerName")),&bstrVal);
	if(bstrVal.Length())
		strCustomerName = bstrVal.m_str;
	bstrVal.Empty();

	LONG nAuthID = 0;
	CString strAuthCode;
	strSqlCmd.Format(_T("SELECT AID,Code FROM [%s] WHERE DID='%s' AND CorpID=%ld"),\
		strTableName,strDeviceID,varCustomerID.lVal);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			bstrVal.Empty();
			spiSqlite3Connect->GetValueInt(0,&nAuthID);
			spiSqlite3Connect->GetValueString(1,&bstrVal);
			strAuthCode = bstrVal.m_str;
			bstrVal.Empty();
			break;
		}
	}
	strSqlCmd.Empty();
	/// ������Ȩ��
	CString strCode;
	strCode.Format(_T("HXSafe%ld_%s_%s_%s"),varCustomerID.lVal,strDeviceID,strValidDay,strPacket);
	spiSafeService->StringSha1(CComBSTR(strCode),&bstrVal);
	strCode.Empty();
	strCode = bstrVal.m_str;
	bstrVal.Empty();
	CString strNewAuthCode;
	strNewAuthCode.Format(_T("HX%04d-%s-%s-%s"),varCustomerID.lVal,strCode.Left(5),strCode.Right(5),strDeviceID.Right(5));
	strNewAuthCode.MakeUpper();
	if(!nAuthID)
	{
		if(nAuthNode > nUsedCount)
		{
			COleDateTime curTime(COleDateTime::GetCurrentTime());
			/// û�г�����Ȩ�ڵ���������
			strSqlCmd.Format(_T("INSERT INTO [%s] (DID,Code,CorpID,CorpName,ATime) \
				VALUES(\'%s\',\'%s\',%ld,\'%s\',%f);"),
				strTableName,strDeviceID,strNewAuthCode,varCustomerID.lVal,strCustomerName,curTime.m_dt);
		}
		else
		{
			spiJsonService = NULL;
			spiCryptoStor->CloseDisk();
			spiCryptoStor = NULL;
			spiSafeService = NULL;
			spiSqlite3Connect->Close();
			spiSqlite3Connect = NULL;
			m_strLastErr = _T("��Ȩ�����Ѿ����꣬�����ٶ����豸��Ȩ��");
#ifdef APP_LOG_ENABLE
			/// д��־
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	else
	{
#ifndef _DEBUG
		if(0 != strNewAuthCode.CompareNoCase(strAuthCode))
#endif
		{
			/// ��Ȩ������
			COleDateTime curTime(COleDateTime::GetCurrentTime());
			strSqlCmd.Format(_T("UPDATE [%s] SET Code=\'%s\',ATime=%f WHERE DID='%s' AND CorpID=%ld"),\
				strTableName,strNewAuthCode,curTime.m_dt,strDeviceID,varCustomerID.lVal);
		}
	}
	if(!strSqlCmd.IsEmpty())
	{
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		if(!nAuthID && SUCCEEDED(hRet))
		{
			strSqlCmd.Empty();
			strSqlCmd.Format(_T("select last_insert_rowid() from %s"),strTableName);
			hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
			hRet = spiSqlite3Connect->NextRow();
			if(SUCCEEDED(hRet))
			{
				spiSqlite3Connect->GetValueInt(0,&nAuthID);
			}
		}
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	if(SUCCEEDED(hRet) && !strSqlCmd.IsEmpty())
	{
		/// ������Ȩ�ɹ���д������������
		hRet = spiCryptoStor->WriteInFile(CComBSTR(strAuthFile),CComBSTR(TDHX_SQLITEDB_AUTHFILE),VARIANT_FALSE);
	}
	strSqlCmd.Empty();
	spiCryptoStor->CloseDisk();
	spiCryptoStor = NULL;

	CComPtr <ISoftEncry> spiSoftEncry = NULL;
	spiSafeService->get_SoftEncry(&spiSoftEncry);
	BOOL bSafeFlag = FALSE;
	if(NULL != spiSoftEncry)
	{
		unsigned char szVI[AES_BLOCK_SIZE] = "HX2016SafeGuard";

		/// д������
		spiJsonService->put_StringValue(CComBSTR(_T("InstallDisk")),CComBSTR(_T("")));
		spiJsonService->put_IntValue(CComBSTR(_T("AuthID")),nAuthID);
		spiJsonService->put_StringValue(CComBSTR(_T("DID")),CComBSTR(strDeviceID));
		spiJsonService->put_StringValue(CComBSTR(_T("AuthCode")),CComBSTR(strNewAuthCode));
		spiJsonService->get_ObjectString(&bstrVal);
		char *pAuthBuf = NULL;
		int nAuthLen = CBaseFuncLib::Us2ToChar(bstrVal.m_str,&pAuthBuf);
		bstrVal.Empty();

		ULONG nOutLen = 0;
		BYTE *pBuf = NULL;
		BYTE szKey[AES_BLOCK_SIZE*2+1] = "01234567899876543210012345678901";
		if(NULL != pAuthBuf)
		{
			hRet = spiSoftEncry->AesCbcEnc((BYTE* )pAuthBuf,nAuthLen-1,szKey,szVI,&nOutLen,&pBuf);
			delete []pAuthBuf;
			pAuthBuf = NULL;
		}
		/// �����Ȩ�ļ�
		bSafeFlag = CBaseFuncLib::WriteToFile(bstrFilePath,pBuf,nOutLen);
		hRet = spiSoftEncry->ReleaseBuf(pBuf);
		pBuf = NULL;
		spiSoftEncry = NULL;
	}

	spiSafeService = NULL;
	if(!bSafeFlag)
	{
		m_strLastErr = _T("д��Ȩ�ļ�ʧ�ܡ�");
		return E_FAIL;
	}
#endif
	return S_OK;
}

STDMETHODIMP CAccountMgr::ImportAuth(BSTR bstrFilePath,BYTE* pbKey,IDispatch **ppAuthInfo)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrFilePath || NULL == ppAuthInfo)
		return E_FAIL;
	if(USEAUTHTYPE_AUTHMGR != (m_nRight & USEAUTHTYPE_AUTHMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	CComPtr <ISoftEncry> spiSoftEncry = NULL;
	if(NULL != spiSafeService)
		spiSafeService->get_SoftEncry(&spiSoftEncry);
	if(NULL == spiSoftEncry || NULL == spiSafeService)
	{
		m_strLastErr = _T("���������������ʧ�ܣ�");
		return E_FAIL;
	}
	HRESULT hRet(E_FAIL);
#ifndef TDHXKJ_VERSION_NOUSB
	CString strInstallDisk,strCID;
	CComBSTR bstrInstallDisk;
	hRet = spiSafeService->GetInstallDisk(VARIANT_TRUE,&bstrInstallDisk);
	if(bstrInstallDisk.Length())
		strInstallDisk = bstrInstallDisk.m_str;
	bstrInstallDisk.Empty();
	/// �����ҷǰ�װU��ʹ��
	CComBSTR bstrCID,bstrDisk;
	hRet = spiSafeService->get_CID(VARIANT_FALSE,&bstrDisk,&bstrCID);
	if(SUCCEEDED(hRet) && bstrCID.Length())
	{
		strCID = bstrCID.m_str;
		bstrCID.Empty();
	}
//#ifndef _DEBUG
	if(0 == strInstallDisk.CompareNoCase(strCID))
	{
		m_strLastErr = _T("��װU������ִ�е�����Ȩ������");
		return E_FAIL;
	}
//#endif
#endif
	CString strAuthFile = CBaseFuncLib::GetAppDataDir()+TDHXKJ_SKFAPP_AUTHFILE;
	if(0 == strAuthFile.CompareNoCase(bstrFilePath))
	{
		m_strLastErr = _T("���ܵ����Ѿ�ʹ�õ���Ȩ�ļ���");
		return E_FAIL;
	}
	BYTE* pBufData = NULL;
	DWORD dwLen = CBaseFuncLib::ReadAllData(bstrFilePath,&pBufData);
	if(NULL == pBufData)
	{
		spiSoftEncry = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("��ȡ��Ȩ�ļ�����ʧ�ܣ�");
		return E_FAIL;
	}
	unsigned char szKey[AES_BLOCK_SIZE*2+1] = "01234567899876543210012345678901";
	unsigned char szVI[AES_BLOCK_SIZE] = "HX2016SafeGuard";

	ULONG nOutLen = 0;
	BYTE *pBuf = NULL;
	if(NULL != pbKey)
		hRet = spiSoftEncry->AesCbcDes(pBufData,dwLen,pbKey,szVI,&nOutLen,&pBuf);
	else
		hRet = spiSoftEncry->AesCbcDes(pBufData,dwLen,szKey,szVI,&nOutLen,&pBuf);
	CComPtr <IJsonService> spiJsonService = NULL;
	if(NULL == pBuf)
	{
		spiSoftEncry = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("��Ȩ���ݽ���ʧ�ܣ�");
		return hRet;
	}
	spiJsonService = CDbHelper::GetJsonService();
	/// ͨ��JSON����
	VARIANT_BOOL bLoadFlag = VARIANT_FALSE;
	if(NULL != spiJsonService)
	{
		spiJsonService->put_CodingType(CODINGTYPE_ANSI);
		hRet = spiJsonService->ParseString(CComBSTR(CString(pBuf)),&bLoadFlag);
	}
	spiSoftEncry->ReleaseBuf(pBuf);
	spiSoftEncry = NULL;
	pBuf = NULL;
	if(VARIANT_FALSE == bLoadFlag)
		spiJsonService = NULL;
	if(NULL == spiJsonService)
	{
		spiSafeService = NULL;
		m_strLastErr = _T("��Ȩ���ݽ���ʧ�ܣ�");
		return hRet;
	}
	BOOL bSaveFlag = TRUE;
	LONGLONG nVal = 0;
	spiJsonService->GetIntValue(CComBSTR(_T("AuthPacket")),&nVal);
	ULONG nAuthPacket = (ULONG)nVal;
#ifdef TDHXKJ_VRSION_SINGLE
	if(HXPACKETTYPE_SERVER == (nAuthPacket & HXPACKETTYPE_SERVER))
	{
		bSaveFlag = FALSE;
	}
#endif
#ifdef TDHXKJ_VRSION_NET
	if(HXPACKETTYPE_SERVER != (nAuthPacket & HXPACKETTYPE_SERVER))
	{
		bSaveFlag = FALSE;
	}
#endif
	CComBSTR bstrVal;
	CString strDID,strUniID,strAuthID;
	spiJsonService->GetStringValue(CComBSTR(_T("AuthID")),&bstrVal);
	if(bstrVal.Length())
	{
		strAuthID = bstrVal.m_str;
		bstrVal.Empty();
		/// �ж��Ƿ��Ѿ����Ƴ���Ȩ����
		CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
		CString strRegVal;
		regKey.GetRegStringVal(_T(""),strRegVal);
		if(0 == strRegVal.CompareNoCase(strAuthID))
		{
			/// �Ѿ���������Ȩ�������ٵ���ʹ��
			m_strLastErr = _T("���ܵ����Ѿ��Ƴ�����Ȩ�ļ���");
			bSaveFlag = FALSE;
		}
	}
	spiJsonService->GetStringValue(CComBSTR(_T("DID")),&bstrVal);
	if(bstrVal.Length())
	{
		strDID = bstrVal.m_str;
		bstrVal.Empty();
	}
	if(NULL != spiSafeService)
	{
		spiSafeService->GetUniqueID(&bstrVal);
		strUniID = bstrVal.m_str;
		bstrVal.Empty();
	}
	if(bSaveFlag && 0 == strDID.CompareNoCase(strUniID))
	{
		m_bAuthValid = VARIANT_FALSE;
		/// �Ϸ�����Ȩ
		spiJsonService->GetStringValue(CComBSTR(_T("AuthCode")),&bstrVal);
		if(bstrVal.Length())
		{
			CheckAuthValid(spiSafeService,spiJsonService,bstrVal.m_str);
			if(VARIANT_TRUE == m_bAuthValid)
			{
				/// �������Ȩ��д��ע���
#ifndef _DEBUG
				CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE);
				regKey.SetRegStringVal(strDID,bstrVal.m_str);
#endif
				bstrVal.Empty();
			}
			else
			{
				m_strLastErr = _T("��Ч��Ȩ��");
				bSaveFlag = FALSE;
			}
		}
		else
		{
			m_strLastErr = _T("��Ȩ��Ϊ�գ�");
			bSaveFlag = FALSE;
		}
	}
	else
	{
		if(bSaveFlag)
		{
			m_strLastErr = _T("��Ȩ��Ϣ��ƥ�䣡");
			bSaveFlag = FALSE;
		}
	}

	if(bSaveFlag)
	{
		spiJsonService->QueryInterface(__uuidof (IDispatch),(void **)ppAuthInfo);
#ifndef TDHXKJ_VERSION_NOUSB
		CComBSTR bstrVal;
		spiJsonService->get_ObjectString(&bstrVal);
		/// д�밲ȫU�����������U��
		SaveAuthToCard(bstrDisk,strCID,bstrVal.m_str,spiSafeService);
		bstrVal.Empty();
		bstrDisk.Empty();
		::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_NORMAL);
		bSaveFlag = ::CopyFile(bstrFilePath,strAuthFile,FALSE);
#else
		::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_NORMAL);
		bSaveFlag = ::CopyFile(bstrFilePath,strAuthFile,FALSE);
#endif
	}
	spiSafeService = NULL;
	spiJsonService = NULL;
	if(!bSaveFlag)
		return E_FAIL;
	return S_OK;
}

void CAccountMgr::CheckAuthValid(CComPtr <ISafeService>& spiSafeService,\
		CComPtr <IJsonService>& spiJsonService,const CString& strAuthCode)
{
	m_bAuthValid = VARIANT_FALSE;
	if(NULL == spiSafeService || NULL == spiJsonService || strAuthCode.IsEmpty())
		return;
	CComBSTR bstrVal;
	CString strUniID;
	CString strModuleName = CBaseFuncLib::GetModuleName(NULL);
	/// ˫���ӵ�¼ʱ���������Ϊû�г�ʼ��COMʹ�û���
	if(0 == strModuleName.CompareNoCase(_T("winlogon.exe")))
	{
		/// ��ȡע����¼���豸��
		CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
		regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strUniID);
	}
	else
	{
		spiSafeService->GetUniqueID(&bstrVal);
		strUniID = bstrVal.m_str;
		bstrVal.Empty();
	}

	CString strValidDay,strPacket;
	CComVariant varCustomerID;
	spiJsonService->GetVariantValue(CComBSTR(_T("CustomerID")),&varCustomerID);
	varCustomerID.ChangeType(VT_I4);
	m_nCorpID = varCustomerID.lVal;
	spiJsonService->GetStringValue(CComBSTR(_T("ValidDay")),&bstrVal);
	if(bstrVal.Length())
		strValidDay = bstrVal.m_str;
	bstrVal.Empty();
	spiJsonService->GetStringValue(CComBSTR(_T("AuthPacket")),&bstrVal);
	if(bstrVal.Length())
	{
		strPacket = bstrVal.m_str;
		m_nAuthPacket = CBaseFuncLib::StrToNum(strPacket); 
	}
	bstrVal.Empty();
	LONGLONG nVal = 0;
	spiJsonService->GetIntValue(CComBSTR(_T("ValidDay")),&nVal);
	COleDateTimeSpan spanTime((ULONG)nVal,0,0,0);
	COleDateTime startTime(2016,1,1,0,0,0);
	COleDateTime AuthTime = startTime + spanTime;

	CString strCode,strCheckCode;
	strCode.Format(_T("HXSafe%ld_%s_%s_%s"),varCustomerID.lVal,strUniID,strValidDay,strPacket);
	spiSafeService->StringSha1(CComBSTR(strCode),&bstrVal);
	strCode.Empty();
	strCode = bstrVal.m_str;
	bstrVal.Empty();
	strCheckCode.Format(_T("HX%04d-%s-%s-%s"),varCustomerID.lVal,strCode.Left(5),strCode.Right(5),strUniID.Right(5));
	strCheckCode.MakeUpper();

	//CString strstr1;
	//strstr1.Format(_T("���¼������֤�룺%s\n"),strCheckCode);
	//OutputDebugString(strstr1);

	//CString strstr2;
	//strstr2.Format(_T("������֤��Ļ������ݣ�CID=%04d,Code=%s,uniID=%s\n"),varCustomerID.lVal,strCode,strUniID);
	//OutputDebugString(strstr2);

	if(0 != strAuthCode.CompareNoCase(strCheckCode))
		return;///�Ƿ���Ȩ��
	COleDateTime curTime = COleDateTime::GetCurrentTime();
	if(curTime < AuthTime)
	{
		/// ��Ч����
		m_bAuthValid = VARIANT_TRUE;
	}
}

STDMETHODIMP CAccountMgr::RefreshAuth(VARIANT_BOOL* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	/// ǿ��ˢ��
	m_strDataPW.Empty();
	m_bAuthValid = VARIANT_FALSE;
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL != spiSafeService)
	{
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
	}
	return S_OK;
}

STDMETHODIMP CAccountMgr::UnLock(BSTR bstrID)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrID)
		return E_FAIL;
	CString strLog(_T("�����˺�"));
	strLog += bstrID;
	/// ��¼�����־
	CDbHelper::WriteSysLog(ACCOUNTTYPE_SA,TDHX_ACCOUNT_SA,m_strCurName,strLog);
	strLog.Empty();

	if(USEAUTHTYPE_ACCOUNTMGR != (m_nRight & USEAUTHTYPE_ACCOUNTMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}
	if(VARIANT_TRUE == m_bReadOnly)
	{
		m_strLastErr = _T("�˲���������밲ȫU���ſ��Խ��У�");
		return E_FAIL;
	}
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
	{
		return E_FAIL;
	}
	CString strSqlCmd(_T(""));

	LONG nStatus = 0;
	strSqlCmd.Format(_T("SELECT Status FROM [%s] WHERE %s='%s'"),\
		SQLITEDB_TABLE_ACCOUNT,_T("LID"),CString(bstrID));
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueInt(0,&nStatus);
			break;
		}
	}
	strSqlCmd.Empty();
	if(nStatus > 0)
	{
		nStatus -= TDHXKJ_ACCOUNTSTATUS_LOCK;
		strSqlCmd.Format(_T("UPDATE [%s] SET Status=%d WHERE LID='%s'"),\
				SQLITEDB_TABLE_ACCOUNT,nStatus,CString(bstrID));
		/// �޸ĵ����ݿⱣ��
		HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		/// ��¼�����־
		if(SUCCEEDED(hRet))
			CDbHelper::WriteSysLog(m_eAccountType,TDHX_ACCOUNT_SA,m_strCurName,CString(bstrID)+_T("�˺Ž����ɹ�"));
		strSqlCmd.Empty();
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return S_OK;
}

STDMETHODIMP CAccountMgr::RemoveAuth(BSTR bstrDeviceID)
{
	// TODO: �ڴ����ʵ�ִ���
	if(USEAUTHTYPE_AUTHMGR != (m_nRight & USEAUTHTYPE_AUTHMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}

	CString strDeviceID;
	if(NULL != bstrDeviceID)
		strDeviceID = bstrDeviceID;
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
	{
		m_strLastErr = _T("�����������ʧ�ܣ�");
		return E_FAIL;
	}
	CComBSTR bstrVal;
	if(strDeviceID.IsEmpty())
	{
		spiSafeService->GetUniqueID(&bstrVal);
		strDeviceID = bstrVal.m_str;
		bstrVal.Empty();
	}
	if(strDeviceID.IsEmpty())
	{
		spiSafeService = NULL;
		m_strLastErr = _T("����豸IDʧ�ܣ�");
		return E_FAIL;
	}
	LONG nAuthID = 0;
	HRESULT hRet;
#ifndef TDHXKJ_VERSION_NOUSB
	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	hRet = spiSafeService->get_CryptoStor(&spiCryptoStor);
	if(NULL == spiCryptoStor)
	{
		spiSafeService = NULL;
		m_strLastErr = _T("��ȡ˽�ܽӿ�ʧ�ܣ�");
		return E_FAIL;
	}
	hRet = spiCryptoStor->EnumDisk(&bstrVal);
	if(!bstrVal.Length())
	{
		spiCryptoStor = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("ö�ٰ�ȫU��ʧ�ܣ�");
		return hRet;
	}
	DWORD dwSize = 0;
	spiCryptoStor->PutCurDisk(bstrVal,&dwSize);

	CString strAuthFile(CBaseFuncLib::GetTmpPath(FALSE)+TDHX_SQLITEDB_AUTHFILE);
	::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_NORMAL);
	/// �����ȶ�ȡ
	hRet = spiCryptoStor->ReadOnlyFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE),CComBSTR(strAuthFile));
	/// ���Դ�
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(SUCCEEDED(hRet) && NULL != spiConnectHelper)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_FALSE,(short)15,CComBSTR(m_strDataPW),&spiSqlite3Connect);
		if(NULL == spiSqlite3Connect)
		{
			CComBSTR bstrErrInfo;
			spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		spiConnectHelper = NULL;
	}
	if(NULL == spiSqlite3Connect)
	{
		spiCryptoStor->CloseDisk();
		spiCryptoStor = NULL;
		spiSafeService = NULL;
		m_strLastErr = _T("�����ݿ�ʧ�ܣ�");
		return hRet;
	}
	CString strOldCode(_T("")),strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT Code,AID FROM [%s] WHERE DID='%s' AND CorpID=%ld"),\
		SQLITEDB_TABLE_AUTHLIST,strDeviceID,m_nCorpID);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			bstrVal.Empty();
			spiSqlite3Connect->GetValueString(0,&bstrVal);
			strOldCode = bstrVal.m_str;
			bstrVal.Empty();
			spiSqlite3Connect->GetValueInt(1,&nAuthID);
			break;
		}
	}
	strSqlCmd.Empty();
	if(strOldCode.GetLength())
	{
		strSqlCmd.Format( _T("DELETE FROM [%s] WHERE DID='%s' AND CorpID=%ld"), \
				SQLITEDB_TABLE_AUTHLIST,strDeviceID,m_nCorpID);
		/// �޸ĵ����ݿⱣ��
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	else
	{
		hRet = E_FAIL;
		m_strLastErr = _T("û���ҵ���Ӧ����Ȩ��Ϣ��");
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	if(strOldCode.GetLength() && SUCCEEDED(hRet))
	{
		hRet = spiCryptoStor->WriteInFile(CComBSTR(strAuthFile),CComBSTR(TDHX_SQLITEDB_AUTHFILE),VARIANT_FALSE);
#ifndef _DEBUG
		CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE);
		regKey.SetRegStringVal(_T(""),CBaseFuncLib::NumToStr(nAuthID));
		regKey.DelKeyName(strDeviceID);
		regKey.DelKeyName(PRODUCT_COMMAN_DEVICEID);
#endif
		/// ɾ�����ɵ���ʱ��Ȩ�ļ�
		CString strAuthFile = CBaseFuncLib::GetAppDataDir()+TDHXKJ_SKFAPP_AUTHFILE;
		::DeleteFile(strAuthFile);
	}
	else
		hRet = E_FAIL;
	spiCryptoStor->CloseDisk();
	spiCryptoStor = NULL;
#else
	CComPtr <IJsonService> spiJsonService = NULL;
	CString strAuthFile = CBaseFuncLib::GetAppDataDir()+TDHXKJ_SKFAPP_AUTHFILE;
	if(!CBaseFuncLib::IsPathExist(strAuthFile))
	{
		CString strInsAuthFile = CComHelper::GetAppInsPath()+TDHXKJ_SKFAPP_AUTHFILE;
		::CopyFile(strInsAuthFile,strAuthFile,FALSE);
	}
	hRet = GetFileAuthInfo(CComBSTR(strAuthFile),NULL,(IDispatch **)&spiJsonService,TRUE);
	if(NULL != spiJsonService)
	{
		spiJsonService->GetStringValue(CComBSTR(_T("AuthID")),&bstrVal);
		if(bstrVal.Length())
		{
			nAuthID = CBaseFuncLib::StrToNum(bstrVal.m_str);
			bstrVal.Empty();
		}
		spiJsonService = NULL;
		::DeleteFile(strAuthFile);
	}
#ifndef _DEBUG
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE);
	regKey.SetRegStringVal(_T(""),CBaseFuncLib::NumToStr(nAuthID));
	regKey.DelKeyName(strDeviceID);
	regKey.DelKeyName(PRODUCT_COMMAN_DEVICEID);
#endif

#endif
	spiSafeService = NULL;
	return hRet;
}

HRESULT CAccountMgr::GetFileAuthInfo(BSTR bstrFilePath,\
	BYTE* pbKey,IDispatch **ppAuthInfo,BOOL bGetPW)
{
	CComPtr <ISoftEncry> spiSoftEncry = NULL;
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL != spiSafeService)
		spiSafeService->get_SoftEncry(&spiSoftEncry);
	if(NULL == spiSoftEncry || NULL == spiSafeService)
		return E_FAIL;
	HRESULT hRet(E_FAIL);
	BYTE* pBufData = NULL;
	DWORD dwLen = CBaseFuncLib::ReadAllData(bstrFilePath,&pBufData);
	if(NULL == pBufData)
		return E_FAIL;
	unsigned char szVI[AES_BLOCK_SIZE] = "HX2016SafeGuard";
	unsigned char szKey[AES_BLOCK_SIZE*2+1] = "01234567899876543210012345678901";

	ULONG nOutLen = 0;
	BYTE *pBuf = NULL;
	if(NULL != pbKey)
		hRet = spiSoftEncry->AesCbcDes(pBufData,dwLen,pbKey,szVI,&nOutLen,&pBuf);
	else
		hRet = spiSoftEncry->AesCbcDes(pBufData,dwLen,szKey,szVI,&nOutLen,&pBuf);
	if(NULL == pBuf)
	{
		spiSoftEncry = NULL;
		return hRet;
	}
	CComPtr <IJsonService> spiJsonService = CDbHelper::GetJsonService();
	/// ͨ��JSON����
	VARIANT_BOOL bLoadFlag = VARIANT_FALSE;
	if(NULL != spiJsonService)
	{
		spiJsonService->put_CodingType(CODINGTYPE_ANSI);
		CString strAuthInfo(pBuf);
		hRet = spiJsonService->ParseString(CComBSTR(strAuthInfo),&bLoadFlag);
		strAuthInfo.Empty();
	}
	if(VARIANT_FALSE == bLoadFlag)
		spiJsonService = NULL;
	spiSoftEncry->ReleaseBuf(pBuf);
	spiSoftEncry = NULL;
	pBuf = NULL;
	if(NULL == spiJsonService)
		return hRet;
	if(!bGetPW)
	{
		/// �������
		spiJsonService->put_StringValue(CComBSTR(_T("DataPW")),CComBSTR(_T("")));
		spiJsonService->put_StringValue(CComBSTR(_T("AdminPW")),CComBSTR(_T("")));
	}
	else
	{
		CComBSTR bstrVal;
		CString strAuthCode;

		CString strModuleName = CBaseFuncLib::GetModuleName(NULL);
		CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
		/// ˫���ӵ�¼ʱ���������Ϊû�г�ʼ��COMʹ�û���
		if(0 == strModuleName.CompareNoCase(_T("winlogon.exe")))
		{
			/// ��ȡע����¼���豸��
			CString strDeviceID;
			regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strDeviceID);
			regKey.GetRegStringVal(strDeviceID,strAuthCode);
		}
		else
		{
			spiSafeService->GetUniqueID(&bstrVal);
			regKey.GetRegStringVal(bstrVal.m_str,strAuthCode);
			bstrVal.Empty();
		}
		/// �����Ч��
		if(strAuthCode.IsEmpty())
		{
			spiJsonService->GetStringValue(CComBSTR(_T("AuthCode")),&bstrVal);
			strAuthCode = bstrVal.m_str;
			bstrVal.Empty();
		}
		if(strAuthCode.GetLength())
		{
			CheckAuthValid(spiSafeService,spiJsonService,strAuthCode);
			strAuthCode.Empty();
		}
		spiSafeService = NULL;
	}
	hRet = spiJsonService->QueryInterface(__uuidof (IDispatch),(void **)ppAuthInfo);
	spiJsonService = NULL;
	spiSafeService = NULL;
	return hRet;
}

STDMETHODIMP CAccountMgr::GetAuthInfo(BSTR bstrFilePath,BSTR bstrDiskName,BYTE* pbKey,IDispatch **ppAuthInfo)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == ppAuthInfo)
		return E_POINTER;
	BOOL bGetFlag = FALSE;
	CString strExe = CBaseFuncLib::GetModuleName(NULL);
	if(0 == strExe.CompareNoCase(TDHXKJ_SAFETOOL))
		bGetFlag = TRUE;
	CString strAuthFile(bstrFilePath);
#ifndef TDHXKJ_VERSION_NOUSB
	if(strAuthFile.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		/// ������ʱ��Ȩ�ļ�
		CComPtr<ISafeCard> spiSafeCard = NULL;
		HRESULT hRet = spiSafeService->get_SafeCard(&spiSafeCard);
		if(NULL == spiSafeCard)
		{
			spiSafeService = NULL;
			return hRet;
		}
		CComBSTR bstrVal;
		short nReadLen = TDHXKJ_SKFAPP_FILELEN;
		BYTE BufData[TDHXKJ_SKFAPP_FILELEN] = {0};
		VARIANT_BOOL bRetFlag = VARIANT_FALSE;
		CComPtr <IJsonService> spiJsonService = NULL;
		CComBSTR bstrDisk(bstrDiskName);
		hRet = spiSafeService->get_CID(VARIANT_TRUE,&bstrDisk,&bstrVal);
		if(FAILED(hRet) || !bstrVal.Length())
		{
			/// û�ҵ����������ʱ·���Ƿ���ڱ��ݵ�����
			spiSafeCard = NULL;
			spiSafeService = NULL;
			m_strLastErr = _T("û���ҵ���ȫU�����޷���ȡ��Ȩ��Ϣ");
			return hRet;
		}
		spiSafeCard->put_CurDisk(bstrDisk);
		CString strCID(bstrVal.m_str);
		bstrVal.Empty();
		strCID += _T("_HXSafe");
		hRet = spiSafeService->StringSha1(CComBSTR(strCID),&bstrVal);
		strCID.Empty();
		CString strSha1 = bstrVal.m_str;
		bstrVal.Empty();
		CComBSTR bstrAppName(TDHXKJ_SKFAPP_NAME),bstrAdminPin(strSha1.Left(3)),bstrUserPin;
		bstrAdminPin.Append(strSha1.Right(5));
		strSha1.Delete(0,16);
		bstrUserPin.Append(strSha1.Left(8));
		ULONG nAppHandle = 0;
		hRet = spiSafeCard->OpenApplication(CComBSTR(TDHXKJ_SKFAPP_NAME),&nAppHandle);
		if(FAILED(hRet))
		{
			spiSafeCard = NULL;
			spiSafeService = NULL;
			return hRet;
		}
		SHORT nTryCount = 0;
		hRet = spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,bstrUserPin,&nTryCount);
		if(FAILED(hRet))
		{
			LONG nErrCode = 0;
			spiSafeCard->get_LastErrCode(&nErrCode);
			if(nErrCode)
			{
				CComBSTR bstrErrInfo;
				spiSafeCard->get_LastErrInfo(&bstrErrInfo);
				if(bstrErrInfo.Length())
					m_strLastErr = bstrErrInfo.m_str;
				spiSafeCard->CloseApplication(nAppHandle);
				nAppHandle = 0;
				spiSafeCard = NULL;
				spiSafeService = NULL;
				return hRet;
			}
		}
		/// ���ȱʡ����
		hRet = spiSafeCard->ReadAppFile(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONFIGFILE),0,&nReadLen,BufData);
		if(FAILED(hRet))
		{
			/// ��ȡ������Ϣ
			CComBSTR bstrErrInfo;
			spiSafeCard->get_LastErrInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
			/// д��־
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
			spiSafeCard->ClearSecureState(nAppHandle);
			spiSafeCard->CloseApplication(nAppHandle);
			spiSafeCard = NULL;
			nAppHandle = 0;
			spiSafeService = NULL;
			return hRet;
		}
		CString strData(BufData);
		strData.TrimLeft();
		strData.TrimRight();
		if(!strData.IsEmpty())
		{
			if(NULL == spiJsonService)
				spiJsonService = CDbHelper::GetJsonService();
			if(NULL != spiJsonService)
			{
				spiJsonService->put_CodingType(CODINGTYPE_ANSI);
				hRet = spiJsonService->ParseString(CComBSTR(strData),&bRetFlag);
			}
		}
		spiSafeCard->ClearSecureState(nAppHandle);
		spiSafeCard->CloseApplication(nAppHandle);
		spiSafeCard = NULL;
		nAppHandle = 0;
		spiSafeService = NULL;
		if(VARIANT_TRUE == bRetFlag)
			hRet = spiJsonService->QueryInterface(__uuidof (IDispatch),(void **)ppAuthInfo);
		spiJsonService = NULL;
		return hRet;
	}
#endif
	HRESULT hRet = GetFileAuthInfo(bstrFilePath,pbKey,ppAuthInfo,bGetFlag);
	return hRet;
}

CString CAccountMgr::GetSafePW(CComPtr <ISafeService>& spiSafeService,CString& strDBPW,EAccountType eAccountType)
{
	CString strDefaultPW(_T(""));
	if(NULL == spiSafeService)
		return strDefaultPW;
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
#ifdef TDHXKJ_VERSION_NOUSB
	CComPtr <IJsonService> spiJsonService = NULL;
	CString strAuthFile = CBaseFuncLib::GetAppDataDir()+TDHXKJ_SKFAPP_AUTHFILE;
	if(!CBaseFuncLib::IsPathExist(strAuthFile))
	{
		CString strInsAuthFile = CComHelper::GetAppInsPath()+TDHXKJ_SKFAPP_AUTHFILE;
		::CopyFile(strInsAuthFile,strAuthFile,FALSE);
	}
	HRESULT hRet = GetFileAuthInfo(CComBSTR(strAuthFile),NULL,(IDispatch **)&spiJsonService,TRUE);
	if(NULL == spiJsonService)
		return strDefaultPW;
	VARIANT_BOOL bRetFlag = VARIANT_TRUE;
#else
	BOOL bSyncFlag = FALSE;
	short nReadLen = TDHXKJ_SKFAPP_FILELEN;
	BYTE BufData[TDHXKJ_SKFAPP_FILELEN] = {0};
	VARIANT_BOOL bRetFlag = VARIANT_FALSE;
	CComPtr <IJsonService> spiJsonService = NULL;
	CComBSTR bstrVal,bstrDiskName;
	HRESULT hRet = spiSafeService->get_CID(VARIANT_TRUE,&bstrDiskName,&bstrVal);
	if(SUCCEEDED(hRet) && ACCOUNTTYPE_SA == eAccountType && bstrVal.Length())
	{
		CString strInstallDisk;
		regKey.GetRegStringVal(PRODUCT_COMMAN_INSDISK,strInstallDisk);
		if(0 != strInstallDisk.CompareNoCase(bstrVal.m_str))
		{
			/// ��������Ա������У�鰲װU��
			m_strLastErr = _T("�˲���Ҫ��ʹ�ó�������Ա���õİ�ȫU����");
			return strDefaultPW;
		}
	}
	if(FAILED(hRet) || !bstrVal.Length())
	{
		spiSafeService->CloseSafeCard();
		/// û�ҵ����������ʱ·���Ƿ���ڱ��ݵ�����
		CString strAuthFile = CBaseFuncLib::GetAppDataDir()+TDHXKJ_SKFAPP_AUTHFILE;
		if(CBaseFuncLib::IsPathExist(strAuthFile))
		{
			/// ���ñ�������֧��������У��൱��ֻ��ģʽ�����ܽ��а�ȫ������Ŀ�޸ĵȲ���
			m_bReadOnly = VARIANT_TRUE;
			hRet = GetFileAuthInfo(CComBSTR(strAuthFile),NULL,(IDispatch **)&spiJsonService,TRUE);
			if(SUCCEEDED(hRet))
			{
				bRetFlag = VARIANT_TRUE;
#ifdef APP_LOG_ENABLE
				/// д��־
//				WRITELOGTOFILE(_T("û�з��ְ�ȫU�����������޿�ģʽ"));
#endif
			}
		}
		if(NULL == spiJsonService)
		{
			m_strLastErr = _T("û�в��밲ȫU������Ȩ�ļ���Ч��");
			return strDefaultPW;
		}
	}
	else
	{
		CComPtr<ISafeCard> spiSafeCard = NULL;
		hRet = spiSafeService->get_SafeCard(&spiSafeCard);
		if(NULL == spiSafeCard)
			return strDefaultPW;
		spiSafeCard->put_CurDisk(bstrDiskName);
		CString strCID(bstrVal.m_str);
		bstrVal.Empty();
		strCID += _T("_HXSafe");
		hRet = spiSafeService->StringSha1(CComBSTR(strCID),&bstrVal);
		strCID.Empty();
		CString strSha1 = bstrVal.m_str;
		bstrVal.Empty();
		CComBSTR bstrAppName(TDHXKJ_SKFAPP_NAME),bstrUserPin;
		strSha1.Delete(0,16);
		bstrUserPin.Append(strSha1.Left(8));
		ULONG nAppHandle = 0;
		hRet = spiSafeCard->OpenApplication(CComBSTR(TDHXKJ_SKFAPP_NAME),&nAppHandle);
		if(FAILED(hRet))
		{
			spiSafeCard = NULL;
			spiSafeService->CloseSafeCard();
			return strDefaultPW;
		}
		SHORT nTryCount = 0;
		hRet = spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,bstrUserPin,&nTryCount);
		if(FAILED(hRet))
		{
			spiSafeCard->CloseApplication(nAppHandle);
			nAppHandle = 0;
			spiSafeCard = NULL;
			spiSafeService->CloseSafeCard();
			return strDefaultPW;
		}
		/// ���ȱʡ����
		hRet = spiSafeCard->ReadAppFile(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONFIGFILE),0,&nReadLen,BufData);
		if(FAILED(hRet) || !nReadLen)
		{
			/// ��ȡ������Ϣ
			CComBSTR bstrErrInfo;
			spiSafeCard->get_LastErrInfo(&bstrErrInfo);
			if(bstrErrInfo.Length())
				m_strLastErr = bstrErrInfo.m_str;
			else
				m_strLastErr = _T("��ȡ��Ȩ�����쳣");
#ifdef APP_LOG_ENABLE
			/// д��־
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		else
		{
			CString strData(BufData);
			strData.TrimLeft();
			strData.TrimRight();
			if(!strData.IsEmpty())
			{
				bSyncFlag = TRUE;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					spiJsonService->put_CodingType(CODINGTYPE_ANSI);
					hRet = spiJsonService->ParseString(CComBSTR(strData),&bRetFlag);
					if(VARIANT_FALSE == bRetFlag)
					{
#ifdef APP_LOG_ENABLE
						WRITELOGTOFILE(strData);
#endif
					}
				}
			}
		}
		spiSafeCard->ClearSecureState(nAppHandle);
		spiSafeCard->CloseApplication(nAppHandle);
		spiSafeCard = NULL;
		spiSafeService->CloseSafeCard();
		nAppHandle = 0;
	}
#endif
	if(VARIANT_TRUE == bRetFlag && NULL != spiJsonService)
	{
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(CComBSTR(_T("AdminPW")),&bstrVal);
		if(bstrVal.Length())
			strDefaultPW = bstrVal.m_str;
		bstrVal.Empty();
		spiJsonService->GetStringValue(CComBSTR(_T("DataPW")),&bstrVal);
		if(bstrVal.Length())
			strDBPW = bstrVal.m_str;
		bstrVal.Empty();

		CString strAuthCode;
		CString strModuleName = CBaseFuncLib::GetModuleName(NULL);
		/// ˫���ӵ�¼ʱ���������Ϊû�г�ʼ��COMʹ�û���
		if(0 == strModuleName.CompareNoCase(_T("winlogon.exe")))
		{
			/// ��ȡע����¼���豸��
			CString strDeviceID;
			regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strDeviceID);
			regKey.GetRegStringVal(strDeviceID,strAuthCode);
			spiJsonService->put_StringValue(CComBSTR(_T("DID")),CComBSTR(strDeviceID));
			spiJsonService->put_StringValue(CComBSTR(_T("AuthCode")),CComBSTR(strAuthCode));
		}
		else
		{
			spiSafeService->GetUniqueID(&bstrVal);
			if(bstrVal.Length())
			{
				regKey.GetRegStringVal(bstrVal.m_str,strAuthCode);

				/*CString strstr;
				strstr.Format(_T("��ע����л�ȡ��֤�룺%s\n"),strAuthCode);
				OutputDebugString(strstr);*/

			}
#ifndef TDHXKJ_VERSION_NOUSB
			spiJsonService->put_StringValue(CComBSTR(_T("DID")),bstrVal);
			spiJsonService->put_StringValue(CComBSTR(_T("AuthCode")),CComBSTR(strAuthCode));
#endif
			bstrVal.Empty();
		}
		/// �����Ч��
		if(strAuthCode.IsEmpty())
		{
			spiJsonService->GetStringValue(CComBSTR(_T("AuthCode")),&bstrVal);
			strAuthCode = bstrVal.m_str;
			bstrVal.Empty();
		}
		/*CString strstr1;
		strstr1.Format(_T("�����֤ǰ����֤�룺%s\n"),strAuthCode);
		OutputDebugString(strstr1);*/

		CheckAuthValid(spiSafeService,spiJsonService,strAuthCode);
		strAuthCode.Empty();

#ifndef TDHXKJ_VERSION_NOUSB
		CString strAuthFile = CBaseFuncLib::GetAppDataDir()+TDHXKJ_SKFAPP_AUTHFILE;
		if(bSyncFlag && !CBaseFuncLib::IsPathExist(strAuthFile) && 0 == strModuleName.CompareNoCase(TDHXKJ_MAIN_SAFEAPP))
		{
			/// ������Ȩ����
			unsigned char szVI[AES_BLOCK_SIZE] = "HX2016SafeGuard";
			char *pAuthBuf = NULL;

			CComPtr <ISoftEncry> spiSoftEncry = NULL;
			spiSafeService->get_SoftEncry(&spiSoftEncry);
			if(NULL != spiSoftEncry && NULL != spiJsonService)
			{
				spiJsonService->get_ObjectString(&bstrVal);
				int nAuthLen = CBaseFuncLib::Us2ToChar(bstrVal.m_str,&pAuthBuf);
				bstrVal.Empty();

				ULONG nOutLen = 0;
				BYTE *pBuf = NULL;
				BYTE szKey[AES_BLOCK_SIZE*2+1] = "01234567899876543210012345678901";
				if(NULL != pAuthBuf)
				{
					hRet = spiSoftEncry->AesCbcEnc((BYTE* )pAuthBuf,nAuthLen-1,szKey,szVI,&nOutLen,&pBuf);
					delete []pAuthBuf;
					pAuthBuf = NULL;
				}

				BOOL bSafeFlag = CBaseFuncLib::WriteToFile(strAuthFile,pBuf,nOutLen);
				hRet = spiSoftEncry->ReleaseBuf(pBuf);
				spiSoftEncry = NULL;
#ifndef _DEBUG
				/// ��������
				if(bSafeFlag)
					::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM);
#endif
			}
		}
#endif
	}
	else
	{
		if(NULL == spiJsonService)
			m_strLastErr = _T("��Ȩ���ݶ���Ϊ��");
		else
			m_strLastErr = _T("��Ȩ���ݽ�������");
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(m_strLastErr);
#endif
	}
	spiJsonService = NULL;
	return strDefaultPW;
}

void CAccountMgr::SaveAuthToCard(CComBSTR bstrDiskName,const CString& strCID,\
	const CString& strAuthInfo,CComPtr <ISafeService>& spiSafeService)
{
	if(NULL == spiSafeService)
		return;
	CComPtr<ISafeCard> spiSafeCard = NULL;
	HRESULT hRet = spiSafeService->get_SafeCard(&spiSafeCard);
	if(NULL == spiSafeCard)
		return;
	CComBSTR bstrVal;
	spiSafeCard->put_CurDisk(bstrDiskName);
	hRet = spiSafeService->StringSha1(CComBSTR(strCID+_T("_HXSafe")),&bstrVal);
	CString strSha1 = bstrVal.m_str;
	strSha1.Delete(0,16);
	bstrVal.Empty();
	CComBSTR bstrUserPin(strSha1.Left(8));

	ULONG nAppHandle = 0;
	hRet = spiSafeCard->OpenApplication(CComBSTR(TDHXKJ_SKFAPP_NAME),&nAppHandle);
	if(FAILED(hRet))
	{
		spiSafeCard = NULL;
		return;
	}
	SHORT nTryCount = 0;
	hRet = spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,bstrUserPin,&nTryCount);
	if(FAILED(hRet))
	{
		spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
		spiSafeCard = NULL;
		return;
	}
	char *szBufFile = NULL;
	int nStrLen = CBaseFuncLib::Us2ToChar(strAuthInfo,&szBufFile);
	/// д�ļ�
	if(NULL != szBufFile)
	{
		hRet = spiSafeCard->WriteAppFile(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONFIGFILE),0,(BYTE*)szBufFile,nStrLen-1);
		delete []szBufFile;
		szBufFile = NULL;
	}
	if(FAILED(hRet))
	{
		/// дʧ��
		CComBSTR bstrErrInfo;
		spiSafeCard->get_LastErrInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
		/// д��־
		WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
		bstrErrInfo.Empty();
	}
	spiSafeCard->ClearSecureState(nAppHandle);
	spiSafeCard->CloseApplication(nAppHandle);
	nAppHandle = 0;
	spiSafeCard = NULL;
}

BOOL CAccountMgr::InitAccount(CComPtr <ISafeService>& spiSafeService,CComPtr <ISqlite3Connect>& spiSqlite3Connect,\
	ULONG nAccount,const CString& strSaCCode,const CString& strAdminCCode,const CString& strAuditCCode)
{
	BOOL bSaveFlag = FALSE,bCreateFlag = FALSE;
	HRESULT hRet(E_FAIL);
	if(NULL == spiSqlite3Connect || !nAccount)
		return bSaveFlag;
	CString strSqlCmd(_T(""));
	/// �ж��˺��Ƿ����
	if(ACCOUNTTYPE_SA == (nAccount & ACCOUNTTYPE_SA))
	{
		strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
			_T("CCode"),SQLITEDB_TABLE_ACCOUNT,\
			_T("LID"),TDHX_ACCOUNT_SA);
		CString strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
		if(strValue.IsEmpty())
		{
			strSqlCmd.Empty();
			/// ���ȱʡ��������Ա�˺�
			strSqlCmd.Format(_T("INSERT INTO [%s] (LID,SName,CCode,URight,UType) \
				VALUES(\'%s\',\'%s\',\'%s\',%ld,%d);"),
				SQLITEDB_TABLE_ACCOUNT,TDHX_ACCOUNT_SA,TDHX_ACCOUNT_SANAME,strSaCCode,\
				USEAUTHTYPE_ACCOUNTMGR|USEAUTHTYPE_UNLOAD|USEAUTHTYPE_SELFSAFE|USEAUTHTYPE_AUTHMGR|USEAUTHTYPE_DATABACKUP|USEAUTHTYPE_SAFEDISKMGR,ACCOUNTTYPE_SA);
			hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
			if(SUCCEEDED(hRet))
			{
				bSaveFlag = TRUE;
#ifdef APP_LOG_ENABLE
//				WRITELOGTOFILE(_T("��ʼ��������������Ա�˺ųɹ���"));
#endif
			}
			
		}
		strSqlCmd.Empty();
		strValue.Empty();

		//������ʱ�˺�
		strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
			_T("CCode"),SQLITEDB_TABLE_TEMP_ACCOUNT	,\
			_T("STempName"),TDHX_ACCOUNT_SANAME_TEMP);
		strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
		if(strValue.IsEmpty())
		{
			//��û�д����û�
			bCreateFlag = TRUE;
			OutputDebugString(_T("����Ĭ�ϳ�������Ա�û�\n"));
			strSqlCmd.Empty();
			/// ���ȱʡ��������Ա�˺�
			strSqlCmd.Format(_T("INSERT INTO [%s] (LID,STempName,SName,CTempCode,CCode,URight,UType) \
								VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%ld,%d);"),
								SQLITEDB_TABLE_TEMP_ACCOUNT,TDHX_ACCOUNT_SA,TDHX_ACCOUNT_SANAME_TEMP,TDHX_ACCOUNT_SANAME,strSaCCode,strSaCCode,\
								USEAUTHTYPE_ACCOUNTMGR|USEAUTHTYPE_UNLOAD|USEAUTHTYPE_SELFSAFE|USEAUTHTYPE_AUTHMGR|USEAUTHTYPE_DATABACKUP|USEAUTHTYPE_SAFEDISKMGR,ACCOUNTTYPE_SA);
			hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
			if(SUCCEEDED(hRet))
			{
				bSaveFlag = TRUE;
#ifdef APP_LOG_ENABLE
				//				WRITELOGTOFILE(_T("��ʼ��������������Ա�˺ųɹ���"));
#endif
			}
			
		}
		strSqlCmd.Empty();

	}
	if(ACCOUNTTYPE_ADMIN == (nAccount & ACCOUNTTYPE_ADMIN))
	{
		strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
			_T("CCode"),SQLITEDB_TABLE_ACCOUNT,\
			_T("LID"),TDHX_ACCOUNT_ADMIN);
		CString strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
		if(strValue.IsEmpty())
		{
			strSqlCmd.Empty();
			/// ���ȱʡ����Ա�˺�
			strSqlCmd.Format(_T("INSERT INTO [%s] (LID,SName,CCode,URight,UType) \
				VALUES(\'%s\',\'%s\',\'%s\',%ld,%d);"), 
				SQLITEDB_TABLE_ACCOUNT,TDHX_ACCOUNT_ADMIN,TDHX_ACCOUNT_ADMINNAME,strAdminCCode,\
				USEAUTHTYPE_SAFEGUARD|USEAUTHTYPE_CONTROLMODE|USEAUTHTYPE_FILEWHITEMGR|USEAUTHTYPE_USBMGR|USEAUTHTYPE_DATABACKUP|USEAUTHTYPE_OSLOGIN,ACCOUNTTYPE_ADMIN);
			hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
			if(SUCCEEDED(hRet))
			{
				bSaveFlag = TRUE;
#ifdef APP_LOG_ENABLE
//				WRITELOGTOFILE(_T("��ʼ����������Ա�˺ųɹ���"));
#endif
			}
			
		}
		strSqlCmd.Empty();
		strValue.Empty();

		//������ʱ�˺�
		if (bCreateFlag)
		{
			strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
				_T("CCode"),SQLITEDB_TABLE_TEMP_ACCOUNT,\
				_T("STempName"),TDHX_ACCOUNT_ADMINNAME_TEMP);
			strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
			if(strValue.IsEmpty())
			{
				OutputDebugString(_T("����Ĭ�Ϲ���Ա�û�\n"));
				strSqlCmd.Empty();
				/// ���ȱʡ����Ա�˺�
				strSqlCmd.Format(_T("INSERT INTO [%s] (LID,STempName,SName,CTempCode,CCode,URight,UType) \
									VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%ld,%d);"),
									SQLITEDB_TABLE_TEMP_ACCOUNT,TDHX_ACCOUNT_ADMIN,TDHX_ACCOUNT_ADMINNAME_TEMP,TDHX_ACCOUNT_ADMINNAME,strAdminCCode,strAdminCCode,\
									USEAUTHTYPE_SAFEGUARD|USEAUTHTYPE_CONTROLMODE|USEAUTHTYPE_FILEWHITEMGR|USEAUTHTYPE_USBMGR|USEAUTHTYPE_DATABACKUP|USEAUTHTYPE_OSLOGIN,ACCOUNTTYPE_ADMIN);
				hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
				if(SUCCEEDED(hRet))
				{
					bSaveFlag = TRUE;
#ifdef APP_LOG_ENABLE
					//				WRITELOGTOFILE(_T("��ʼ����������Ա�˺ųɹ���"));
#endif
				}
				
			}
		}
		strSqlCmd.Empty();
	}
	if(ACCOUNTTYPE_AUDIT == (nAccount & ACCOUNTTYPE_AUDIT))
	{
		strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
			_T("CCode"),SQLITEDB_TABLE_ACCOUNT,\
			_T("LID"),TDHX_ACCOUNT_AUDIT);
		CString strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
		if(strValue.IsEmpty())
		{
			strSqlCmd.Empty();
			/// ���ȱʡ���Ա�˺�
			strSqlCmd.Format(_T("INSERT INTO [%s] (LID,SName,CCode,URight,UType) \
				VALUES(\'%s\',\'%s\',\'%s\',%ld,%d);"), 
				SQLITEDB_TABLE_ACCOUNT,TDHX_ACCOUNT_AUDIT,TDHX_ACCOUNT_AUDITNAME,strAuditCCode,\
				USEAUTHTYPE_LOGMGR,ACCOUNTTYPE_AUDIT);
			hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
			if(SUCCEEDED(hRet))
			{
				bSaveFlag = TRUE;
#ifdef APP_LOG_ENABLE
//				WRITELOGTOFILE(_T("��ʼ���������Ա�˺ųɹ���"));
#endif
			}
			
		}

		strSqlCmd.Empty();
		strValue.Empty();

		//������ʱ�˺�
		if (bCreateFlag)
		{
			strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
				_T("CCode"),SQLITEDB_TABLE_TEMP_ACCOUNT,\
				_T("STempName"),TDHX_ACCOUNT_AUDITNAME_TEMP);
			strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
			if(strValue.IsEmpty())
			{
				OutputDebugString(_T("����Ĭ�����Ա�û�\n"));
				strSqlCmd.Empty();
				/// ���ȱʡ���Ա�˺�
				strSqlCmd.Format(_T("INSERT INTO [%s] (LID,STempName,SName,CTempCode,CCode,URight,UType) \
									VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%ld,%d);"), 
									SQLITEDB_TABLE_TEMP_ACCOUNT,TDHX_ACCOUNT_AUDIT,TDHX_ACCOUNT_AUDITNAME_TEMP,TDHX_ACCOUNT_AUDITNAME,strAuditCCode,strAuditCCode,\
									USEAUTHTYPE_LOGMGR,ACCOUNTTYPE_AUDIT);
				hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
				if(SUCCEEDED(hRet))
				{
					bSaveFlag = TRUE;
#ifdef APP_LOG_ENABLE
					//				WRITELOGTOFILE(_T("��ʼ���������Ա�˺ųɹ���"));
#endif
				}
				strSqlCmd.Empty();
			}
		}
	}
#ifndef TDHXKJ_VERSION_NOUSB
	/// �Զ���Ӱ�װU����������
	if(NULL == spiSafeService)
		return bSaveFlag;
	CComBSTR bstrDisk = 0;
	CString strInstallID;
	CComPtr<ISafeCard> spiSafeCard = NULL;
	spiSafeService->get_SafeCard(&spiSafeCard);
	if(NULL != spiSafeCard)
	{
		spiSafeCard->get_CurDisk(&bstrDisk);
		spiSafeCard = NULL;
		CComBSTR bstrVal;
		spiSafeService->get_CID(VARIANT_TRUE,&bstrDisk,&bstrVal);
		if(bstrVal.Length())
			strInstallID = bstrVal.m_str;
		bstrVal.Empty();
	}
	if(strInstallID.GetLength())
	{
		CComPtr<ICryptoStor> spiCryptoStor = NULL;
		spiSafeService->get_CryptoStor(&spiCryptoStor);
		if(NULL != spiCryptoStor)
		{
			ULONG nDiskSize = 0;
			spiCryptoStor->PutCurDisk(bstrDisk,&nDiskSize);
			int nType = 0;
			ULONG nSizeG = nDiskSize/(1024*1024);
			if(nDiskSize%(1024*1024))
				nSizeG++;
			/// ���ж��Ƿ����
			strSqlCmd.Format(_T("SELECT UID FROM [%s] WHERE UID='%s'"),\
				SQLITEDB_TABLE_USBWHITE,strInstallID);
			CString strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
			strSqlCmd.Empty();
			COleDateTime curTime = COleDateTime::GetCurrentTime();
			if(strValue.IsEmpty())
			{
				strSqlCmd.Format(_T("INSERT INTO [%s] (UID,UDisk,MName,ATime,Size,UType) \
					VALUES(\'%s\',\'%s\',\'%s\',%f,%ld,%d);"),
					SQLITEDB_TABLE_USBWHITE,strInstallID,(CString)bstrDisk.m_str,\
					_T("��װU��"),curTime.m_dt,nSizeG,nType);
			}
			else
			{
				strSqlCmd.Format(_T("UPDATE [%s] SET UDisk=\'%s\',MName=\'%s\',Size=%ld,UType=%d WHERE UID='%s'"),\
					SQLITEDB_TABLE_USBWHITE,(CString)bstrDisk.m_str,_T("��װU��"),nSizeG,nType,strInstallID);
			}
			spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		}
	}
#endif
	return bSaveFlag;
}

#ifndef TDHXKJ_VERSION_NOUSB
BOOL CAccountMgr::GetInstallCode(CComPtr <ISafeService>& spiSafeService,CString& strSACode)
{
	BOOL bGetFlag = FALSE;
	if(NULL == spiSafeService)
		return bGetFlag;
	CComBSTR bstrVal;
	spiSafeService->GetInstallDisk(VARIANT_FALSE,&bstrVal);
	if(!bstrVal.Length())
	{
		return bGetFlag;
	}
	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	HRESULT hRet = spiSafeService->get_CryptoStor(&spiCryptoStor);
	if(NULL == spiCryptoStor)
	{
		bstrVal.Empty();
		return bGetFlag;
	}
	/// ��˽������ȡ
	DWORD dwSize = 0;
	hRet = spiCryptoStor->PutCurDisk(bstrVal,&dwSize);
	if(SUCCEEDED(hRet))
	{
		CString strDataFile = CBaseFuncLib::GetTmpPath()+CString(_T("Ins"))+TDHX_SQLITEDB_SYSFILE;
		if(CBaseFuncLib::IsPathExist(strDataFile))
		{
			::SetFileAttributes(strDataFile,FILE_ATTRIBUTE_NORMAL);
			::DeleteFile(strDataFile);
		}
		hRet = spiCryptoStor->ReadOnlyFile(CComBSTR(TDHX_SQLITEDB_SYSFILE),CComBSTR(strDataFile));
		if(SUCCEEDED(hRet) && CBaseFuncLib::IsPathExist(strDataFile))
		{
			/// �����ݿ��ȡ
			CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
			if(NULL != spiConnectHelper)
			{
				try
				{
					CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
					hRet = spiConnectHelper->OpenDB(CComBSTR(strDataFile),VARIANT_FALSE,SQLITE_OPEN_DEFAULTOVERTIME,CComBSTR(m_strDataPW),&spiSqlite3Connect);
					if(NULL != spiSqlite3Connect)
					{
						CString strSqlCmd(_T(""));
						strSqlCmd.Format(_T("SELECT CCode FROM [%s] WHERE %s='%s'"),\
							SQLITEDB_TABLE_ACCOUNT,_T("LID"),TDHX_ACCOUNT_SA);
						spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
						while(SUCCEEDED(spiSqlite3Connect->NextRow()))
						{
							spiSqlite3Connect->GetValueString(0,&bstrVal);
							strSACode = bstrVal.m_str;
							bstrVal.Empty();
							bGetFlag = TRUE;
							break;
						}
						spiSqlite3Connect->Close();
						spiSqlite3Connect = NULL;
					}
				}
				catch( ... )
				{
				}
				if(FAILED(hRet))
				{
					CComBSTR bstrErrInfo;
					spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
					/// д��־
					if(bstrErrInfo.Length())
					{
						WRITELOGTOFILE(bstrErrInfo.m_str);
					}
#endif
					bstrErrInfo.Empty();
				}
				spiConnectHelper = NULL;
			}
		}
		::DeleteFile(strDataFile);
	}
	spiCryptoStor = NULL;
	return bGetFlag;
}
#endif

STDMETHODIMP CAccountMgr::Init(ULONG nAccount,BSTR bstrPW,VARIANT_BOOL bCheckFile)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!nAccount)
		return E_POINTER;
#ifndef _DEBUG
	if(VARIANT_TRUE == bCheckFile)
	{
		CString strDataFile = CBaseFuncLib::GetAppDataDir()+TDHX_SQLITEDB_SYSFILE;
		if(CBaseFuncLib::IsPathExist(strDataFile) && CBaseFuncLib::GetFileSize(strDataFile) > 2048)
			return S_OK;/// �ѽ��ʼ����
	}
#endif
	m_strLastErr.Empty();
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	CString strAdminPW(_T(""));
	/// ��鴴����˳��ȡ��Ĭ�Ϲ���Ա����
	strAdminPW = CreateTable(spiSafeService);
	if(NULL != bstrPW)
	{
		/// �����˳�ʼ������Ա����
		CString strTempPW = bstrPW;
		if(strTempPW.GetLength())
			strAdminPW = strTempPW;
		strTempPW.Empty();
	}

	if(m_strDataPW.IsEmpty())
	{
		spiSafeService = NULL;
#ifdef TDHXKJ_VERSION_NOUSB
		m_strLastErr = _T("�޷������������ݿ⣬�����°�װ����");
#else
		m_strLastErr = _T("�޷������������ݿ⣬��ȷ���Ƿ�����˰�ȫU��");
#endif
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(m_strLastErr);
#endif
		return E_FAIL;/// ��ʼ��ʧ�ܣ�������뿨
	}

	if(strAdminPW.IsEmpty())
	{
		spiSafeService = NULL;
#ifdef TDHXKJ_VERSION_NOUSB
		m_strLastErr = _T("�޷������������ݿ⣬�����°�װ����");
#else
		m_strLastErr = _T("�޷������������ݿ⣬��ȷ���Ƿ�����˰�װU��");
#endif
		return E_FAIL;/// ��ʼ��ʧ��
	}
	CString strCCode(_T(""));
	CComBSTR bstrVal,bstrMD5;
	/// ����MD5
	spiSafeService->StringMD5(CComBSTR(strAdminPW),&bstrMD5);
	spiSafeService->StringSha1(bstrMD5,&bstrVal);
	bstrMD5.Empty();
	if(bstrVal.Length())
		strCCode = bstrVal.m_str;
	bstrVal.Empty();

	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
	{
		spiSafeService = NULL;
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("�������ݿ�ʧ�ܣ���ȷ���Ƿ�����˰�ȫU��"));
#endif
		return E_FAIL;
	}
	CString strSaCode(strCCode);
#ifndef TDHXKJ_VERSION_NOUSB
	/// ���Ի�ȡ�޸ĺ�ĳ�������Ա����
	CString strNewCode;
	BOOL bGetFlag = GetInstallCode(spiSafeService,strNewCode);
	if(bGetFlag)
	{
		strSaCode.Empty();
		strSaCode = strNewCode;
	}
#endif
	BOOL bSaveFlag = InitAccount(spiSafeService,spiSqlite3Connect,nAccount,strSaCode,strCCode,strCCode);
	spiSafeService = NULL;
	if(NULL != spiSqlite3Connect)
	{
#ifndef _DEBUG
		CComBSTR bstrDbPath;
		bstrDbPath.Empty();
		spiSqlite3Connect->get_DbPathFile(&bstrDbPath);
#endif
		spiSqlite3Connect->Close();
		spiSqlite3Connect = NULL;
#ifndef _DEBUG
		/// ��������
		::SetFileAttributes((CString )bstrDbPath.m_str,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM);
#endif
	}
	return S_OK;
}

STDMETHODIMP CAccountMgr::GetAuthCode(ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_nAuthPacket;
	return S_OK;
}

STDMETHODIMP CAccountMgr::get_AuthValid(VARIANT_BOOL* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_bAuthValid;
	return S_OK;
}

STDMETHODIMP CAccountMgr::get_CurLID(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrVal(m_strCurLID);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}
//��ȡ��ǰ�û���
STDMETHODIMP CAccountMgr::get_CurName(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrVal(m_strCurName);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CAccountMgr::get_Right(ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_nRight;
	return S_OK;
}

STDMETHODIMP CAccountMgr::get_ReadOnly(VARIANT_BOOL* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_bReadOnly;
	return S_OK;
}

STDMETHODIMP CAccountMgr::ClearStatus(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_nRight = 0;
	m_eAccountType = ACCOUNTTYPE_UNKNOWN;
	m_bReadOnly = VARIANT_FALSE;

	m_strDataPW.Empty();
	m_strCurLID.Empty();
	m_strCurName.Empty();
	m_strShowName.Empty();
	m_strLastErr.Empty();
	return S_OK;
}

STDMETHODIMP CAccountMgr::Add(EAccountType eType, BSTR bstrID,BSTR bstrName, BSTR bstrPWHash, ULONG nRight)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	/// ��¼�����־
	CDbHelper::WriteSysLog(m_eAccountType,m_strCurLID,m_strCurName,CString(bstrID)+_T("�˺����"));
	/// �ж����ģ��Ȩ�ޣ����˺Ź���ģ��Ͳ�֧��
	if(USEAUTHTYPE_ACCOUNTMGR != (m_nRight & USEAUTHTYPE_ACCOUNTMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}

	return S_OK;
}

STDMETHODIMP CAccountMgr::Del(BSTR bstrID)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	/// ��¼�����־
	CDbHelper::WriteSysLog(m_eAccountType,m_strCurLID,m_strCurName,CString(bstrID) + _T("�˺�ɾ��"));
	/// �ж����ģ��Ȩ��
	if(USEAUTHTYPE_ACCOUNTMGR != (m_nRight & USEAUTHTYPE_ACCOUNTMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}

	if(NULL == bstrID)
		return E_POINTER;
	CString strLID(bstrID);
	strLID.TrimLeft();
	strLID.TrimRight();
	if(strLID.IsEmpty())
		return E_INVALIDARG;
	/// �ж��Ƿ�Ϊ��������Ա�˺�
	if(0 == strLID.CompareNoCase(TDHX_ACCOUNT_SA))
		return E_FAIL;/// ����ɾ��
	/// �����ݿ�����
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	if(m_strDataPW.IsEmpty())
		GetSafePW(spiSafeService,m_strDataPW,ACCOUNTTYPE_SA);
	spiSafeService = NULL;
	if(m_strDataPW.IsEmpty())
	{
		if(m_strLastErr.IsEmpty())
		{
#ifdef TDHXKJ_VERSION_NOUSB
			m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
			m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
		}
		return E_FAIL;
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("�������ݿ�ʧ�ܣ���ȷ���Ƿ�����˰�ȫU��"));
#endif
		return E_FAIL;
	}
	CString strSqlCmd(_T(""));
	strSqlCmd.Format( _T("DELETE FROM [%s] WHERE %s='%s'"), \
			SQLITEDB_TABLE_ACCOUNT,_T("LID"),(CString)bstrID);
	/// �޸ĵ����ݿⱣ��
	HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();
	if(FAILED(hRet))
	{
		CComBSTR bstrVal;
		spiSqlite3Connect->get_LastErrorInfo(&bstrVal);
		m_strLastErr = bstrVal.m_str;
		bstrVal.Empty();
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;

	return S_OK;
}

STDMETHODIMP CAccountMgr::ChangePW(EAccountType eUserType,BSTR bstrID,BSTR bstrNewPWHash,BSTR bstrAccountName)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	if(USEAUTHTYPE_ACCOUNTMGR != (m_nRight & USEAUTHTYPE_ACCOUNTMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	if(m_strDataPW.IsEmpty())
		GetSafePW(spiSafeService,m_strDataPW,ACCOUNTTYPE_SA);
	if(m_strDataPW.IsEmpty())
	{
		spiSafeService = NULL;
		if(m_strLastErr.IsEmpty())
		{
#ifdef TDHXKJ_VERSION_NOUSB
			m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
			m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
		}
		return E_FAIL;
	}
	CComBSTR bstrVal;
	CString strCCode(bstrNewPWHash);
	spiSafeService->StringSha1(bstrNewPWHash,&bstrVal);
	if(bstrVal.Length())
		strCCode = bstrVal.m_str;
	bstrVal.Empty();
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("�������ݿ�ʧ�ܣ���ȷ���Ƿ�����˰�ȫU��"));
#endif
		spiSafeService = NULL;
		return E_FAIL;
	}
	CString strtemp;
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT SName FROM [%s] WHERE %s='%s' AND STempName='%s'"),\
		SQLITEDB_TABLE_TEMP_ACCOUNT,_T("LID"),(CString )bstrID,bstrAccountName);
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		spiSqlite3Connect->GetValueString(0,&bstrVal);
		strtemp = bstrVal.m_str;
		break;
	}

	if (strtemp.IsEmpty())
	{
		m_strLastErr = _T("������Ч�˺ţ�");
		return E_FAIL;
	}

	if(ACCOUNTTYPE_SA == eUserType)
	{
		strSqlCmd.Format(_T("UPDATE [%s] SET CCode=\'%s\',CTempCode=\'%s\' WHERE LID='%s' AND STempName='%s'"),\
			SQLITEDB_TABLE_TEMP_ACCOUNT,strCCode,strCCode,bstrID,bstrAccountName);
		
	}
	else
	{
		strSqlCmd.Format(_T("UPDATE [%s] SET CTempCode=\'%s\' WHERE LID='%s' AND STempName='%s'"),\
			SQLITEDB_TABLE_TEMP_ACCOUNT,strCCode,bstrID,bstrAccountName);
		
	}

	/// �޸ĵ����ݿⱣ��
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();

	/// ��¼�����־
	CDbHelper::WriteSysLog(eUserType,bstrID,bstrAccountName,CString(bstrVal)+_T("�˺��޸�����"));
	bstrVal.Empty();
	strSqlCmd.Empty();
	if(FAILED(hRet))
	{
		spiSqlite3Connect->get_LastErrorInfo(&bstrVal);
		m_strLastErr = bstrVal.m_str;
		bstrVal.Empty();
	}
	spiSqlite3Connect->get_DbPathFile(&bstrVal);
	CString strDataFile = bstrVal.m_str;
	
	if(FAILED(hRet))
	{
		strDataFile.Empty();
		spiSafeService = NULL;
		return hRet;
	}
	if(ACCOUNTTYPE_SA != eUserType)
	{
		/// �ǳ�������Ա���룬�����ݵ�����
		strDataFile.Empty();
		spiSafeService = NULL;
		spiSqlite3Connect->Close();
		spiSqlite3Connect = NULL;
		return hRet;
	}

	//�޸Ĺ���Ա�˺�����
	strSqlCmd.Format(_T("UPDATE [%s] SET CCode=\'%s\' WHERE LID='%s'"),\
		SQLITEDB_TABLE_ACCOUNT,strCCode,bstrID);
	/// �޸ĵ����ݿⱣ��
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();

	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
#ifndef TDHXKJ_VERSION_NOUSB
	/// ��������Ա�޸�����󣬱��뱸�ݵ���ȫU����
	CComBSTR bstrDisk = 0;
	CString strInstallID;
	CComPtr<ISafeCard> spiSafeCard = NULL;
	spiSafeService->get_SafeCard(&spiSafeCard);
	if(NULL != spiSafeCard)
	{
		spiSafeCard->get_CurDisk(&bstrDisk);
		spiSafeCard = NULL;
		spiSafeService->get_CID(VARIANT_TRUE,&bstrDisk,&bstrVal);
		if(bstrVal.Length())
			strInstallID = bstrVal.m_str;
		bstrVal.Empty();
	}
	if(!strInstallID.GetLength())
	{
		spiSafeService = NULL;
		m_strLastErr = _T("��װU��ͨ��ʧ��");
		return S_FALSE;
	}
	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	hRet = spiSafeService->get_CryptoStor(&spiCryptoStor);
	if(NULL != spiCryptoStor)
	{
		/// ���ݵ�˽����
		DWORD dwSize = 0;
		hRet = spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);
		if(SUCCEEDED(hRet))
			hRet = spiCryptoStor->WriteInFile(CComBSTR(strDataFile),CComBSTR(TDHX_SQLITEDB_SYSFILE),VARIANT_FALSE);
		if(FAILED(hRet))
		{
			bstrVal.Empty();
			spiCryptoStor->get_LastErrInfo(&bstrVal);
			m_strLastErr = bstrVal.m_str;
			bstrVal.Empty();
			if(m_strLastErr.IsEmpty())
				m_strLastErr = _T("������װU��ʧ��");
		}
		spiCryptoStor = NULL;
	}
	bstrVal.Empty();
	spiSafeService = NULL;
#endif
	return hRet;
}

STDMETHODIMP CAccountMgr::ChangeRight(BSTR bstrID, ULONG nRight)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	/// ��¼�����־
	CDbHelper::WriteSysLog(m_eAccountType,m_strCurLID,m_strCurName,CString(bstrID)+_T("�˺Ų���Ȩ���޸�"));

	return S_OK;
}

STDMETHODIMP CAccountMgr::Login(IDispatch** ppInfo)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();

	return S_OK;
}

STDMETHODIMP CAccountMgr::ChangeName(BSTR bstrName)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	/// ��¼�����־
	CDbHelper::WriteSysLog(m_eAccountType,m_strCurLID,m_strCurName,_T("�޸��˺���ʾ����"));
	if(USEAUTHTYPE_ACCOUNTMGR != (m_nRight & USEAUTHTYPE_ACCOUNTMGR))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}

	return S_OK;
}

STDMETHODIMP CAccountMgr::get_LastErrorInfo(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_FAIL;
	CComBSTR bstrVal(m_strLastErr);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	m_strLastErr.Empty();
	return S_OK;
}

STDMETHODIMP CAccountMgr::ServiceToken()
{
	// TODO: �ڴ����ʵ�ִ���
	if(ACCOUNTTYPE_ADMIN != m_eAccountType)
		return E_FAIL;
	if(m_strCurLID.IsEmpty())
		return E_FAIL;
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	///�������ݿ����TOKEN
	CHXRegKey regKey(TDHXKJ_HOSTSERVICE_REGPATH,TDHXKJ_CONFIG_NODENAME,HKEY_LOCAL_MACHINE);
	CComBSTR bstrVal;
	spiSafeService->StringSha1(CComBSTR(m_strDataPW),&bstrVal);
	BOOL bSetFlag = regKey.SetRegStringVal(TDHXKJ_CONFIG_DBTOKEN,bstrVal.m_str);
	bstrVal.Empty();
	if(!bSetFlag)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CAccountMgr::GetDataBase(BSTR bstrToken,IDispatch** ppVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == ppVal)
		return E_FAIL;
	m_strLastErr.Empty();
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	if(m_strDataPW.IsEmpty())
	{
		GetSafePW(spiSafeService,m_strDataPW);
		if(m_strDataPW.IsEmpty())
		{
			spiSafeService = NULL;
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
			return E_FAIL;
		}
	}
	BOOL bReadFlag = TRUE;
	if(NULL != bstrToken)
	{
		CComBSTR bstrVal;
		spiSafeService->StringSha1(CComBSTR(m_strDataPW),&bstrVal);
		spiSafeService = NULL;
		CString strToken(bstrToken);
		if(!strToken.IsEmpty() && 0 != strToken.CompareNoCase(bstrVal.m_str))
		{
			m_strLastErr = _T("TokenУ��ʧ�ܡ�");
			bstrVal.Empty();
			return E_FAIL;
		}
		bstrVal.Empty();
		bReadFlag = FALSE;
	}
	else
	{
		if(m_strCurLID.GetLength())
		{
			/// �Ѿ���¼������ȡֻ�����ݿ�
			bReadFlag = FALSE;
		}
	}

	spiSafeService = NULL;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,bReadFlag);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	spiSqlite3Connect->QueryInterface(IID_IDispatch,(LPVOID *)ppVal);
	spiSqlite3Connect = NULL;
	return S_OK;
}

STDMETHODIMP CAccountMgr::get_RunMode(ERunMode* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_eRunMode;
	return S_OK;
}

STDMETHODIMP CAccountMgr::put_RunMode(ERunMode newVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_eRunMode = newVal;
	return S_OK;
}

STDMETHODIMP CAccountMgr::get_Count(SHORT* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	HRESULT hRet(E_FAIL);
	/// ��ѯ���ݿ�
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return hRet;
	if(m_strDataPW.IsEmpty())
	{
		GetSafePW(spiSafeService,m_strDataPW);
		if(m_strDataPW.IsEmpty())
		{
			spiSafeService = NULL;
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
			return hRet;
		}
	}
	spiSafeService = NULL;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,TRUE);
	if(NULL == spiSqlite3Connect)
	{
		spiSafeService = NULL;
		return hRet;
	}
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT LID FROM [%s]"),\
		SQLITEDB_TABLE_ACCOUNT);
	(*pVal) = (SHORT)CDbHelper::GetRecordCount(spiSqlite3Connect,strSqlCmd);
	strSqlCmd.Empty();
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return S_OK;
}

STDMETHODIMP CAccountMgr::get_ShowName(BSTR bstrLID, BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_POINTER;
	m_strLastErr.Empty();
	if(NULL == bstrLID || 0 == m_strCurLID.CompareNoCase(bstrLID))
	{
		CComBSTR bstrVal(m_strShowName);
		bstrVal.CopyTo(pVal);
		bstrVal.Empty();
		return S_OK; 
	}
	HRESULT hRet(E_FAIL);
	/// ��ѯ���ݿ�
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return hRet;
	if(m_strDataPW.IsEmpty())
	{
		GetSafePW(spiSafeService,m_strDataPW);
		if(m_strDataPW.IsEmpty())
		{
			spiSafeService = NULL;
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
//			WRITELOGTOFILE(m_strLastErr);
#endif
			return hRet;
		}
	}
	spiSafeService = NULL;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,TRUE);
	if(NULL == spiSqlite3Connect)
	{
		spiSafeService = NULL;
		return hRet;
	}
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT SName FROM [%s] WHERE %s='%s'"),\
		SQLITEDB_TABLE_ACCOUNT,_T("LID"),(CString )bstrLID);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			CComBSTR bstrVal;
			hRet = spiSqlite3Connect->GetValueString(0,&bstrVal);
			bstrVal.CopyTo(pVal);
			bstrVal.Empty();
			break;
		}
	}
	strSqlCmd.Empty();
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return hRet;
}

STDMETHODIMP CAccountMgr::SetCurrent(EAccountType eUserType,BSTR bstrID,BSTR bstrPWHash,EUseAuthType eAuthType,BSTR bstrCurName,SHORT* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrID || NULL == bstrPWHash || NULL == pVal)
		return E_POINTER;
	(*pVal) = 0;
	/// ��¼��������ز���
	ClearStatus();

	if(m_nTryLoginCount > 5)
	{
		m_strLastErr = _T("�����ɳ��Դ������޷�����֤Ȩ�ޡ�������������ԡ�");
		return E_FAIL;
	}
	CString strTempName(bstrCurName);
	if ((ACCOUNTTYPE_SA == eUserType) && (0 != strTempName.Compare(TDHX_ACCOUNT_SANAME_TEMP)))
	{
		strTempName = TDHX_ACCOUNT_SANAME_TEMP;
	}

	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	/// ��¼��������Ա����ҪУ�鰲װU��
	GetSafePW(spiSafeService,m_strDataPW,eUserType);
	if(m_strDataPW.IsEmpty())
	{
		spiSafeService = NULL;
		if(m_strLastErr.IsEmpty())
		{
#ifdef TDHXKJ_VERSION_NOUSB
			m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
			m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
		}
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(m_strLastErr);
#endif
		return E_FAIL;
	}
	CString strSqlCmd(_T(""));
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
#ifndef TDHXKJ_VERSION_NOUSB
	if(ACCOUNTTYPE_SA == eUserType || ACCOUNTTYPE_ADMIN == eUserType)
	{
		CSTRING_MAP mapDisk;
		CComBSTR bstrVal;
		CComPtr<ISafeCard> spiSafeCard = NULL;
		HRESULT hRet = spiSafeService->get_SafeCard(&spiSafeCard);
		if(NULL != spiSafeCard)
		{
			CString strDisks;
			hRet = spiSafeCard->EnumCard(&bstrVal);
			if(bstrVal.Length())
				strDisks = bstrVal.m_str;
			bstrVal.Empty();
			int nDiskLen = strDisks.GetLength();
			for(int nIndex = 0;nIndex < nDiskLen;nIndex++)
			{
				CComBSTR bstrDisk;
				bstrDisk.Append(strDisks.GetAt(nIndex));
				hRet = spiSafeCard->put_CurDisk(bstrDisk);
				if(FAILED(hRet))
					continue;
				hRet = spiSafeCard->GetCID(bstrDisk,&bstrVal);
				if(bstrVal.Length())
					mapDisk[bstrVal.m_str] = bstrDisk.m_str;
				bstrVal.Empty();
			}
			spiSafeCard = NULL;
		}
		spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
		if(NULL == spiSqlite3Connect)
		{
			spiSafeService = NULL;
			return E_FAIL;
		}
		/// У��U�̰�����
		strSqlCmd.Format(_T("SELECT UID FROM [%s] WHERE UType = 0"),SQLITEDB_TABLE_USBWHITE);
		spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		BOOL bSafeCard = FALSE;
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueString(0,&bstrVal);
			CSTRING_MAP::iterator it = mapDisk.find(bstrVal.m_str);
			if(it != mapDisk.end())
			{
				bSafeCard = TRUE;
				bstrVal.Empty();
				break;
			}
			bstrVal.Empty();
		}
		mapDisk.clear();
		if(!bSafeCard)
		{
			spiSqlite3Connect->Close();
			spiSqlite3Connect = NULL;
			/// û�в���Ϸ��İ�ȫU��
			spiSafeService = NULL;
			/// ��¼�����־
			CDbHelper::WriteSysLog(m_eAccountType,bstrID,strTempName,CString(bstrID)+_T("��¼ʱû�в���Ϸ��İ�ȫU��"));

			m_strLastErr = _T("�˲����������Ϸ��İ�ȫU���ſ��Խ��У�");
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	else
	{
		spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
		if(NULL == spiSqlite3Connect)
		{
			spiSafeService = NULL;
			return E_FAIL;
		}
	}
#else
	spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
	{
		spiSafeService = NULL;
		return E_FAIL;
	}
#endif

	BOOL bExistFlag = FALSE;
	LONG nStatus = 0;
	CString strCCode(_T(""));
	strSqlCmd.Format(_T("SELECT CCode,URight,UType,SName,Status FROM [%s] WHERE %s='%s'"),\
		SQLITEDB_TABLE_ACCOUNT,_T("LID"),(CString )bstrID);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			LONG nVal = 0;
			spiSqlite3Connect->GetValueInt(2,&nVal);
			if(nVal != eUserType)
				break;/// ��¼�˺����Ͳ�ƥ��
			bExistFlag = TRUE;
			m_eAccountType = (EAccountType)nVal;
			CComBSTR bstrVal;
			spiSqlite3Connect->GetValueString(0,&bstrVal);
			strCCode = bstrVal.m_str;
			bstrVal.Empty();
			nVal = 0;
			spiSqlite3Connect->GetValueInt(1,&nVal);
			m_nRight = nVal;
			nVal = 0;
			spiSqlite3Connect->GetValueInt(4,&nVal);
			nStatus = nVal;
			spiSqlite3Connect->GetValueString(3,&bstrVal);
			m_strShowName = bstrVal.m_str;
			bstrVal.Empty();
			break;
		}
	}
	strSqlCmd.Empty();
	/// ��¼���Դ�������3�Σ��ӳٵ�¼
	if(m_nTryLoginCount > 2)
	{
		COleDateTime initTime = COleDateTime(2016,1,1,0,0,0);
		COleDateTime UnLockTime = initTime;
		COleDateTime curTime = COleDateTime::GetCurrentTime();
			strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
			SQLITEDB_OPTIONRECORD_FIELDNAME2,SQLITEDB_TABLE_SYSPARA,\
			SQLITEDB_OPTIONRECORD_FIELDNAME1,(CString )bstrID);
		if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
		{
			while(SUCCEEDED(spiSqlite3Connect->NextRow()))
			{
				spiSqlite3Connect->GetValueDouble(0,&(UnLockTime.m_dt));
				break;
			}
		}
		if((curTime-UnLockTime).GetTotalMinutes() < 3.0)
		{
			spiSqlite3Connect->Close();
			spiSqlite3Connect = NULL;
			spiSafeService = NULL;

			/// ��¼�����־
			CDbHelper::WriteSysLog(m_eAccountType,bstrID,strTempName,CString(bstrID)+_T("�˺Ź���Ƶ����֤Ȩ��"));

			m_strLastErr = _T("ÿ���3���Ӳ���ִ��һ����֤Ȩ��");
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
		///д�����ݿ�
		if(UnLockTime > initTime)
		{
			strSqlCmd.Format(_T("UPDATE [%s] SET %s=\'%f\' WHERE %s='%s'"),\
				SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONRECORD_FIELDNAME2,\
				curTime.m_dt,SQLITEDB_OPTIONRECORD_FIELDNAME1,(CString )bstrID);
		}
		else
		{
			strSqlCmd.Format(_T("INSERT INTO [%s] VALUES (\'%s\',\'%f\');"),\
				SQLITEDB_TABLE_SYSPARA,(CString )bstrID,curTime.m_dt);
		}
		/// ���浽���ݿ⣬��������Ӧ��ʧЧ
		HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}

	CString strLog;
	switch(eAuthType)
	{
	case USEAUTHTYPE_UNLOAD:
		strLog = _T("ִ��ж�ز���");
		break;
	case USEAUTHTYPE_SELFSAFE:
		strLog = _T("�޸�����ȫ����");
		break;
	case USEAUTHTYPE_SAFEGUARD:
		strLog = _T("�޸İ�ȫ��������");
		break;
	case USEAUTHTYPE_CONTROLMODE:
		strLog = _T("�޸ĳ�����������");
		break;
	case USEAUTHTYPE_ACCOUNTMGR:
		strLog = _T("ִ���˺Ź������");
		break;
	case USEAUTHTYPE_FILEWHITEMGR:
		strLog = _T("����������������");
		break;
	case USEAUTHTYPE_SAFEDISKMGR:
		strLog = _T("��ȫU���������");
		break;
	case USEAUTHTYPE_USBMGR:
		strLog = _T("������U�̲���");
		break;
	case USEAUTHTYPE_LOGMGR:
		strLog = _T("��־��Ʋ���");
		break;
	case USEAUTHTYPE_AUTHMGR:
		strLog = _T("�����Ȩ����");
		break;
	case USEAUTHTYPE_DATABACKUP:
		strLog = _T("���ݱ��ݻ�ԭ");
		break;
	default:
		break;
	}

	SHORT nUserType = (SHORT)m_eAccountType;
	strLog += _T("��֤Ȩ��");
	/// ��¼�����־
	CDbHelper::WriteSysLog(nUserType,bstrID,strTempName,strLog);
	strLog.Empty();

	if(TDHXKJ_ACCOUNTSTATUS_LOCK == (TDHXKJ_ACCOUNTSTATUS_LOCK & nStatus))
	{
		spiSafeService = NULL;
		m_nRight = 0;
		m_eAccountType = ACCOUNTTYPE_UNKNOWN;
		m_strShowName.Empty();

		m_nTryLoginCount++;
		(*pVal) = m_nTryLoginCount;

		/// �������˺�
		m_strLastErr = _T("ִ����֤Ȩ��ʧ�ܣ��˺��Ѿ���������");
		spiSqlite3Connect->Close();
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	//2016-11-25 �޸ģ���������ʹ�õ���hash������

	/*CComBSTR bstrVal;
	spiSafeService->StringSha1(bstrPWHash,&bstrVal);*/
	spiSafeService = NULL;
	if(0 != strCCode.CompareNoCase(bstrPWHash))
	{
		EAccountType eAccountType = m_eAccountType;
		m_nRight = 0;
		m_eAccountType = ACCOUNTTYPE_UNKNOWN;
		m_strShowName.Empty();

		/// �������
		m_nTryLoginCount++;
		(*pVal) = m_nTryLoginCount;
	
		/// ���Գ���5��
		if(m_nTryLoginCount > 5)
		{
			if(ACCOUNTTYPE_SA != eAccountType)
			{
				if(bExistFlag)
				{
					strSqlCmd.Format(_T("UPDATE [%s] SET Status=%d WHERE LID='%s'"),\
						SQLITEDB_TABLE_ACCOUNT,TDHXKJ_ACCOUNTSTATUS_LOCK|nStatus,bstrID);
					/// �޸ĵ����ݿⱣ��
					HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
					strSqlCmd.Empty();
				}
				m_strLastErr = _T("ִ����֤Ȩ��ʧ��5�κ��˺ű����������ó�������ԱȨ�޽�������ܼ���ʹ��");
			}
			else
			{
				m_strLastErr = _T("ִ����֤Ȩ��ʧ�ܣ�������������ԡ�");
			}
		}
		else
		{
			if(bExistFlag)
			{
				if(ACCOUNTTYPE_SA != eAccountType)
					m_strLastErr.Format(_T("ִ����֤Ȩ��ʧ�ܣ����ɳ��Դ��� %ld�������ԣ�"),5-m_nTryLoginCount);
				else
					m_strLastErr = _T("ִ����֤Ȩ��ʧ�ܣ������ԣ�");
			}
			else
				m_strLastErr = _T("ִ����֤Ȩ��ʧ�ܣ��˺Ų����ڣ�");
		}

		/// ��¼�����־
		CDbHelper::WriteSysLog(nUserType,bstrID,strTempName,_T("��֤Ȩ��ʧ��"));
		spiSqlite3Connect->Close();
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	else
	{
		if(m_eAccountType == eUserType && (VARIANT_FALSE == m_bReadOnly || USEAUTHTYPE_LOGMGR == eAuthType))
		{
			m_nTryLoginCount = 0;
			m_strCurLID = bstrID;
			//��¼��ǰ�û���
			m_strCurName = strTempName;
			/// ��¼�����־
			CDbHelper::WriteSysLog(nUserType,m_strCurLID,strTempName,_T("��֤Ȩ�޳ɹ�"));
		}
		else
		{
			m_nRight = 0;
			m_eAccountType = ACCOUNTTYPE_UNKNOWN;
			m_strShowName.Empty();

			m_nTryLoginCount++;
			(*pVal) = m_nTryLoginCount;

			m_strLastErr = _T("�˲����������Ϸ��İ�ȫU���ſ��Խ��У�");
			spiSqlite3Connect->Close();
			spiSqlite3Connect = NULL;
			return E_FAIL;
		}
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return S_OK;
}

STDMETHODIMP CAccountMgr::SaveConfig(EHXUseAuthType nAuthType,BSTR bstrNodeName,BSTR bstrNodeValue)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrNodeName || NULL == bstrNodeValue)
		return E_POINTER;
	CString strNodeName(bstrNodeName);
	if(strNodeName.IsEmpty())
		return E_POINTER;
	CString strNodeVal(bstrNodeValue);
	if(0 == strNodeName.CompareNoCase(TDHXKJ_CONFIG_SELFPROTECT) && 0 == strNodeVal.CompareNoCase(_T("1")))
	{
		/// ��������������ҪȨ����֤
		if(m_strDataPW.IsEmpty())
		{
			CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
			if(NULL == spiSafeService)
				return E_FAIL;
			GetSafePW(spiSafeService,m_strDataPW);
			spiSafeService = NULL;
			if(m_strDataPW.IsEmpty())
			{
				if(m_strLastErr.IsEmpty())
				{
#ifdef TDHXKJ_VERSION_NOUSB
					m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
					m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
				}
#ifdef APP_LOG_ENABLE
				WRITELOGTOFILE(m_strLastErr);
#endif
				return E_FAIL;
			}
		}
	}
	else
	{
		if(m_strDataPW.IsEmpty())
			return E_FAIL;/// ��û�е�¼
		if(nAuthType != (m_nRight & nAuthType))
		{
			m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
			return E_FAIL;/// û�в���Ȩ��
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;/// �������ݿ�ʧ��
	CString strSqlCmd(_T(""));
	/// ���ж��ͷŴ���
	strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
		SQLITEDB_OPTIONRECORD_FIELDNAME1,SQLITEDB_TABLE_SYSPARA,\
		SQLITEDB_OPTIONRECORD_FIELDNAME1,strNodeName);
	CString strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
	strSqlCmd.Empty();
	if(strValue.IsEmpty())
	{
		strSqlCmd.Format(_T("INSERT INTO [%s] (%s,%s) \
			VALUES(\'%s\',\'%s\');"),
			SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONRECORD_FIELDNAME1,SQLITEDB_OPTIONRECORD_FIELDNAME2,\
			strNodeName,(CString)bstrNodeValue);
	}
	else
	{
		strSqlCmd.Format(_T("UPDATE [%s] SET %s=\'%s\' WHERE %s='%s'"),\
			SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONRECORD_FIELDNAME2,\
			(CString)bstrNodeValue,SQLITEDB_OPTIONRECORD_FIELDNAME1,strNodeName);
	}
	/// �޸ĵ����ݿⱣ��
	HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	strSqlCmd.Empty();
	return hRet;
}

STDMETHODIMP CAccountMgr::AddUsbWhite(BSTR bstrUsbID,BSTR bstrDiskName,BSTR bstrMakerName,ULONG nSize,SHORT nType)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrUsbID || NULL == bstrDiskName)
		return E_POINTER;
	/// ��¼�����־
	CDbHelper::WriteSysLog(m_eAccountType,m_strCurLID,m_strCurName,_T("���U�̰�����"));
	if(m_strDataPW.IsEmpty())
		return E_FAIL;/// ��û�е�¼
	if(0 == nType)
	{
		if(USEAUTHTYPE_SAFEDISKMGR != (m_nRight & USEAUTHTYPE_SAFEDISKMGR))
		{
			m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
			return E_FAIL;/// û�в���Ȩ��
		}
	}
	else
	{
		if(USEAUTHTYPE_USBMGR != (m_nRight & USEAUTHTYPE_USBMGR))
		{
			m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
			return E_FAIL;/// û�в���Ȩ��
		}
	}

	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;/// �������ݿ�ʧ��
	CString strSqlCmd(_T(""));
	/// ���ж��Ƿ����
	strSqlCmd.Format(_T("SELECT UID FROM [%s] WHERE UID='%s'"),\
		SQLITEDB_TABLE_USBWHITE,(CString)bstrUsbID);
	CString strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlCmd);
	strSqlCmd.Empty();

	COleDateTime curTime = COleDateTime::GetCurrentTime();
	if(strValue.IsEmpty())
	{
		strSqlCmd.Format(_T("INSERT INTO [%s] (UID,UDisk,MName,ATime,Size,UType) \
			VALUES(\'%s\',\'%s\',\'%s\',%f,%ld,%d);"),
			SQLITEDB_TABLE_USBWHITE,(CString )bstrUsbID,(CString )bstrDiskName,\
			(CString )bstrMakerName,curTime.m_dt,nSize,nType);
	}
	else
	{
		strSqlCmd.Format(_T("UPDATE [%s] SET UDisk=\'%s\',MName=\'%s\',Size=%ld,UType=%d WHERE UID='%s'"),\
			SQLITEDB_TABLE_USBWHITE,(CString )bstrDiskName,(CString )bstrMakerName,nSize,nType,(CString )bstrUsbID);
	}
	HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	strSqlCmd.Empty();
	return S_OK;
}

STDMETHODIMP CAccountMgr::DelUsbWhite(BSTR bstrUsbID,SHORT nType)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrUsbID)
		return E_POINTER;
	/// ��¼�����־
	CDbHelper::WriteSysLog(m_eAccountType,m_strCurLID,m_strCurName,_T("ɾ��U�̰�����"));
	if(m_strDataPW.IsEmpty())
		return E_FAIL;/// ��û�е�¼
	if(0 == nType)
	{
		if(USEAUTHTYPE_SAFEDISKMGR != (m_nRight & USEAUTHTYPE_SAFEDISKMGR))
		{
			m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
			return E_FAIL;/// û�в���Ȩ��
		}
	}
	else
	{
		if(USEAUTHTYPE_USBMGR != (m_nRight & USEAUTHTYPE_USBMGR))
		{
			m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
			return E_FAIL;/// û�в���Ȩ��
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;/// �������ݿ�ʧ��
	CString strSqlCmd(_T(""));
	strSqlCmd.Format( _T("DELETE FROM [%s] WHERE UID='%s'"), \
			SQLITEDB_TABLE_USBWHITE,(CString)bstrUsbID);
	HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	strSqlCmd.Empty();
	return hRet;
}

STDMETHODIMP CAccountMgr::get_CanLogin(BSTR bstrLID, VARIANT_BOOL* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrLID || NULL == pVal)
		return E_POINTER;
	(*pVal) = VARIANT_FALSE;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
//			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,TRUE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT Status FROM [%s] WHERE LID='%s'"),\
		SQLITEDB_TABLE_ACCOUNT,(CString )bstrLID);
	/// �ж��Ƿ�Ϊ����
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		LONG nStatus = 0;
		spiSqlite3Connect->GetValueInt(0,&nStatus);
		if(1 != (1 & nStatus))
		{
			/// û�б�����
			(*pVal) = VARIANT_TRUE;
		}
		break;
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return S_OK;
}

STDMETHODIMP CAccountMgr::SaveToCard(BSTR ucDiskName)
{
	// TODO: �ڴ����ʵ�ִ���
	HRESULT hRet(E_FAIL);
#ifndef TDHXKJ_VERSION_NOUSB
	/// ��¼�����־
	CDbHelper::WriteSysLog(m_eAccountType,m_strCurLID,m_strCurName,_T("�������ݵ���ȫU��"));
	m_strLastErr.Empty();
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL == spiSafeService)
		return E_FAIL;
	if(m_strDataPW.IsEmpty())
	{
		GetSafePW(spiSafeService,m_strDataPW);
		if(m_strDataPW.IsEmpty())
		{
			spiSafeService = NULL;
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComBSTR bstrUniID;
	hRet = spiSafeService->GetUniqueID(&bstrUniID);
	CString strDataFile = CBaseFuncLib::GetAppDataDir()+TDHX_SQLITEDB_SYSFILE;
	CString strNewDataFile = bstrUniID.m_str;
	bstrUniID.Empty();
	strNewDataFile += _T("_");
	strNewDataFile += TDHX_SQLITEDB_SYSFILE;/// �����������ð󶨵��ļ�

	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	hRet = spiSafeService->get_CryptoStor(&spiCryptoStor);
	if(NULL == spiCryptoStor)
	{
		spiSafeService = NULL;
		return hRet;
	}
	CComBSTR bstrDisk;
	if(NULL != ucDiskName)
		bstrDisk.Append(ucDiskName);
	if(!bstrDisk.Length())
		spiCryptoStor->GetCurDisk(&bstrDisk);
	if(!bstrDisk.Length())
	{
		CString strDisks(_T(""));
		CComBSTR bstrVal;
		spiCryptoStor->EnumDisk(&bstrVal);
		if(bstrVal.Length())
		{
			strDisks = bstrVal.m_str;
			bstrVal.Empty();
		}
		if(!strDisks.IsEmpty())
			bstrDisk.Append(strDisks.GetAt(0));
	}
	DWORD dwSize = 0;
	if(bstrDisk.Length())
	{
		hRet = spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);
		/// ���浽˽����
		hRet = spiCryptoStor->WriteInFile(CComBSTR(strDataFile),CComBSTR(strNewDataFile),VARIANT_FALSE);
	}
	spiCryptoStor = NULL;
	spiSafeService = NULL;
#endif
	return hRet;
}

STDMETHODIMP CAccountMgr::AddStopService(BSTR bstrServiceName,SHORT nStart,BSTR bstrImagePath)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrServiceName || NULL == bstrImagePath)
		return E_POINTER;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	LONG nServcieID = 0;
	CString strSqlCmd;
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	CString strTableName = SQLITEDB_TABLE_STOPSERVICE;
	HRESULT hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼ϵͳ���÷���� SID��ʶ/SerName��������/ImagePath������·��/ATime���ʱ��/Startԭ������������/Status״̬
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			SID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
			SerName VARCHAR DEFAULT '' NOT NULL,\
			ImagePath VARCHAR DEFAULT '' NOT NULL,\
			ATime DOUBLE DEFAULT '' NOT NULL,\
			Start INTEGER DEFAULT '0' NOT NULL,\
			Status INTEGER DEFAULT '0' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		if(SUCCEEDED(hRet))
		{
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(_T("����ϵͳ���÷����ɹ�"));
#endif
		}
		strSqlCmd.Empty();
	}
	else
	{
		strSqlCmd.Format(_T("SELECT SID FROM [%s] WHERE SerName='%s' AND ImagePath='%s'"),\
			strTableName,(CString )bstrServiceName,(CString )bstrImagePath);
		HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueInt(0,&nServcieID);
			break;
		}

	}
	if(!nServcieID)
	{
		/// ��Ӽ�¼
		COleDateTime curTime(COleDateTime::GetCurrentTime());
		strSqlCmd.Format(_T("INSERT INTO [%s] (SerName,ImagePath,ATime,Start,Status) \
			VALUES(\'%s\',\'%s\',%f,%d,1);"),strTableName,
			(CString )bstrServiceName,(CString )bstrImagePath,curTime.m_dt,nStart);/// 1����
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		if(FAILED(hRet))
		{
			CComBSTR bstrErrInfo;
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
			m_strLastErr = bstrErrInfo.m_str;
#ifdef APP_LOG_ENABLE
			/// д��־
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		strSqlCmd.Empty();
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return hRet;
}

STDMETHODIMP CAccountMgr::DelStopService(BSTR bstrServiceName,BSTR bstrImagePath)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrServiceName || NULL == bstrImagePath)
		return E_POINTER;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	LONG nServcieID = 0;
	CString strSqlCmd;
	strSqlCmd.Format(_T("SELECT SID FROM [%s] WHERE SerName='%s' AND ImagePath='%s'"),\
			SQLITEDB_TABLE_STOPSERVICE,(CString )bstrServiceName,(CString )bstrImagePath);
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		spiSqlite3Connect->GetValueInt(0,&nServcieID);
		break;
	}
	if(nServcieID)
	{
		strSqlCmd.Format( _T("DELETE FROM [%s] WHERE SID=%ld"), \
				SQLITEDB_TABLE_STOPSERVICE,nServcieID);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
		if(FAILED(hRet))
		{
			CComBSTR bstrErrInfo;
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
			m_strLastErr = bstrErrInfo.m_str;
#ifdef APP_LOG_ENABLE
			/// д��־
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return S_OK;
}


//
STDMETHODIMP CAccountMgr::CheckAccountExist(BSTR strAccountName)
{
	if(NULL == strAccountName )
		return E_POINTER;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	CComBSTR nstrVal;
	CString strLID;
	CString strSqlCmd;
	strSqlCmd.Format(_T("SELECT LID FROM [%s] WHERE STempName='%s'"),\
		SQLITEDB_TABLE_TEMP_ACCOUNT,(CString )strAccountName);
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		spiSqlite3Connect->GetValueString(0,&nstrVal);
		strLID = nstrVal.m_str;
		break;
	}

	if (strLID.IsEmpty())
	{
		m_strLastErr = _T("�˺Ų����ڣ�");
		return E_FAIL;
	}

	m_strLastErr = _T("�˺��Ѿ����ڣ�");
	return S_OK;
}
//
STDMETHODIMP CAccountMgr::GetRealAccountInfo(BSTR LID,BSTR* strAccountTypeName, BSTR* strRealPasswd,ULONG* strRight)
{
	if(NULL == LID || NULL == strAccountTypeName || NULL == strRealPasswd || NULL == strRight)
		return E_POINTER;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;

	CComBSTR bstrValName,bstrValCode;
	LONG ulRight = 0;
	CString strSqlCmd;
	strSqlCmd.Format(_T("SELECT SName,CCode,URight FROM [%s] WHERE LID='%s'"),\
		SQLITEDB_TABLE_ACCOUNT,(CString )LID);
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		spiSqlite3Connect->GetValueString(0,&bstrValName);
		
		
		spiSqlite3Connect->GetValueString(1,&bstrValCode);
		
		
		spiSqlite3Connect->GetValueInt(2,&ulRight);
		break;
	}

	if (ulRight == 0)
	{
		m_strLastErr = _T("��ȡ�ļ���Ϣʧ�ܣ�");
		return E_FAIL;
	}

	bstrValName.CopyTo(strAccountTypeName);
	bstrValCode.CopyTo(strRealPasswd);
	
	(*strRight) = ulRight;

	return S_OK;
}
//
STDMETHODIMP CAccountMgr::SaveAccountInfo(BSTR strAccountName, BSTR strPassword,BSTR LID,BSTR strAccountTypeName,BSTR strRealPasswd,ULONG strRight)
{
	if(NULL == strAccountName || NULL == strPassword || NULL == strRealPasswd || NULL == LID || NULL == strAccountTypeName)
		return E_POINTER;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;

	//������ʱ�û���
	CString strSqlCmd;
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	CString strTableName = SQLITEDB_TABLE_TEMP_ACCOUNT;
	HRESULT hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
							LID VARCHAR DEFAULT '' NOT NULL ,\
							STempName VARCHAR DEFAULT '' NOT NULL PRIMARY KEY UNIQUE,\
							SName VARCHAR DEFAULT '' NOT NULL,\
							CTempCode VARCHAR DEFAULT '' NOT NULL,\
							CCode VARCHAR DEFAULT '' NOT NULL,\
							LTime DOUBLE DEFAULT '' NOT NULL,\
							URight INTEGER DEFAULT '0' NOT NULL,\
							Status INTEGER DEFAULT '0' NOT NULL,\
							UType INTEGER DEFAULT '0' NOT NULL,\
							SyncFlag INTEGER DEFAULT '0' NOT NULL);"),\
							strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	strTableName.Empty();
	CString strTempLid(LID); 
	int ntype = (0 == strTempLid.CompareNoCase(TDHX_ACCOUNT_ADMIN))?ACCOUNTTYPE_ADMIN:ACCOUNTTYPE_AUDIT;
	COleDateTime curTime(COleDateTime::GetCurrentTime());
	strSqlCmd.Empty();
	strSqlCmd.Format(_T("INSERT INTO [%s] (LID,STempName,SName,CTempCode,CCode,URight,UType) \
						VALUES(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%ld,%d);"),\
						SQLITEDB_TABLE_TEMP_ACCOUNT,(CString )LID,(CString )strAccountName,
						(CString )strAccountTypeName,(CString )strPassword,(CString )strRealPasswd,
						strRight,ntype);

	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	if(FAILED(hRet))
	{
		CComBSTR bstrErrInfo;
		spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
		m_strLastErr = bstrErrInfo.m_str;
#ifdef APP_LOG_ENABLE
		/// д��־
		WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
		bstrErrInfo.Empty();

		m_strLastErr = _T("�½��˺�ʧ�ܣ�");
		return E_FAIL;
	}
	strSqlCmd.Empty();
	
	/// ��¼�����־
	CDbHelper::WriteSysLog(ntype,LID,strAccountName,_T("����û��ɹ�"));

	return S_OK;
}
//
STDMETHODIMP CAccountMgr::DelelteAccountInfo(BSTR strAccountName,BSTR LID)
{
	if(NULL == strAccountName )
		return E_POINTER;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	CComBSTR nstrVal;
	CString strLID;
	CString strSqlCmd;
	strSqlCmd.Format(_T("SELECT LID FROM [%s] WHERE STempName='%s'"),\
		SQLITEDB_TABLE_TEMP_ACCOUNT,(CString )strAccountName);
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		spiSqlite3Connect->GetValueString(0,&nstrVal);
		strLID = nstrVal.m_str;
		break;
	}

	if (strLID.IsEmpty())
	{
		m_strLastErr = _T("�˺Ų����ڣ�");
		return E_FAIL;
	}
	else if (0 == strLID.CompareNoCase(TDHX_ACCOUNT_SA))
	{
		m_strLastErr = _T("��������Ա�˺Ų�����ɾ����");
		return E_FAIL;
	}

	//ִ��ɾ��
	strSqlCmd.Empty();
	strSqlCmd.Format( _T("DELETE FROM [%s] WHERE STempName='%s'"), \
		SQLITEDB_TABLE_TEMP_ACCOUNT,(CString )strAccountName);
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	
	if(FAILED(hRet))
	{
		CComBSTR bstrErrInfo;
		spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
		m_strLastErr = bstrErrInfo.m_str;
#ifdef APP_LOG_ENABLE
		/// д��־
		WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
		bstrErrInfo.Empty();

		m_strLastErr = _T("�˺�ɾ��ʧ�ܣ�");
		return E_FAIL;
	}

	CString strTempLid(LID); 
	int ntype = (0 == strTempLid.CompareNoCase(TDHX_ACCOUNT_ADMIN))?ACCOUNTTYPE_ADMIN:ACCOUNTTYPE_AUDIT;
	/// ��¼�����־
	CDbHelper::WriteSysLog(ntype,LID,strAccountName,_T("ɾ���û��ɹ�"));

	return S_OK;
}

//
STDMETHODIMP CAccountMgr::CheckIsPassswdRight(BSTR LID,BSTR strAccountName,BSTR bstrMD5,BSTR* strRealPasswd)
{
	if(NULL == strAccountName || NULL == bstrMD5 || NULL == strRealPasswd)
		return E_POINTER;
	m_strLastErr.Empty();
	if(m_strDataPW.IsEmpty())
	{
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return E_FAIL;
		GetSafePW(spiSafeService,m_strDataPW);
		spiSafeService = NULL;
		if(m_strDataPW.IsEmpty())
		{
			if(m_strLastErr.IsEmpty())
			{
#ifdef TDHXKJ_VERSION_NOUSB
				m_strLastErr = _T("�޷���֤Ȩ�ޣ������°�װ����");
#else
				m_strLastErr = _T("�޷���֤Ȩ�ޣ���ȷ���Ƿ�����˺Ϸ��İ�ȫU��");
#endif
			}
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(m_strLastErr);
#endif
			return E_FAIL;
		}
	}
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(m_strDataPW,FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	CComBSTR nstrVal;
	CString strTempName,strTempPass;
	CString strSqlCmd("");
	strSqlCmd.Format(_T("SELECT CCode,STempName,CTempCode FROM [%s] WHERE LID='%s'"),\
		SQLITEDB_TABLE_TEMP_ACCOUNT,(CString )LID);
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		spiSqlite3Connect->GetValueString(2,&nstrVal);
		strTempPass = nstrVal.m_str;
		nstrVal.Empty();
		spiSqlite3Connect->GetValueString(1,&nstrVal);
		strTempName = nstrVal.m_str;
		nstrVal.Empty();
		if ((0 != strTempPass.CompareNoCase(bstrMD5)) || (0 != strTempName.Compare(strAccountName)))
		{
			continue;
		}
		spiSqlite3Connect->GetValueString(0,&nstrVal);
		nstrVal.CopyTo(strRealPasswd);
		break;
	}
	CString strPasswd = nstrVal.m_str;

	if (strPasswd.IsEmpty())
	{
		m_strLastErr = _T("δ���ҵ���Ӧ���˺ţ�");
		return E_FAIL;
	}

	return S_OK;
}
