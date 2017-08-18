#include "StdAfx.h"
#include "Md5Wrapper.h"


#include <iostream>  
#include "openssl/sha.h"
#include "openssl/md5.h"
#include "openssl/aes.h"


#include "BaseFuncLib.h"


static const char hex_chars[] = "0123456789ABCDEF";

CString convert_hex(unsigned char *md,int nLen)
{
	CString strSha1(_T(""));
	int i = 0;
	unsigned int c = 0;

	for (i = 0; i < nLen; i++)
	{
		c = (md[i] >> 4) & 0x0f;
		strSha1 += hex_chars[c];
		strSha1 += hex_chars[md[i] & 0x0f];
	}
	return strSha1;
}

CMd5Wrapper::CMd5Wrapper(void)
{
}

CMd5Wrapper::~CMd5Wrapper(void)
{
}

CString CMd5Wrapper::GetStringSha1(const CString& strString)
{
	SHA_CTX shactx;  
	unsigned char md[SHA_DIGEST_LENGTH];  

	SHA1_Init(&shactx);
	DWORD m = 0;
	char* szBuf = NULL;
	m = CBaseFuncLib::Us2ToChar(strString,&szBuf);
	if(NULL != szBuf)
	{
		SHA1_Update(&shactx,szBuf,m-1);
		delete []szBuf;
		szBuf = NULL;
	}
	SHA1_Final(md, &shactx);
	CString strSha1 = convert_hex(md,SHA_DIGEST_LENGTH);
	return strSha1;
}

CString CMd5Wrapper::GetStringMD5(const CString& strString)
{
	MD5_CTX c;
	char* szBuf = NULL;
	CString strMD5(_T(""));
	unsigned char md5[MD5_DIGEST_LENGTH]={0};

	MD5_Init(&c); 
	int nLen = CBaseFuncLib::Us2ToChar(strString,&szBuf);
	if(NULL != szBuf)
	{
		MD5_Update(&c, szBuf, nLen-1);
		delete []szBuf;
		szBuf = NULL;
	}
	MD5_Final(md5,&c);
	strMD5 = convert_hex(md5,MD5_DIGEST_LENGTH);
	return strMD5;
}
