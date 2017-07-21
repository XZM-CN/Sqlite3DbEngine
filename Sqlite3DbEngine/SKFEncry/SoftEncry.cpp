// SoftEncry.cpp : CSoftEncry 的实现

#include "stdafx.h"
#include "SoftEncry.h"
#include "openssl/ssl.h"
#include "openssl/aes.h"
#include "BaseFuncLib.h"


#ifndef WIN64
#ifdef _DEBUG
#pragma comment(lib, "..\\bin\\Release\\libeay32.lib") // Debug
#else
#pragma comment(lib, "..\\bin\\Release\\libeay32.lib") // Release
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "\\Debug\\X64\\SdCryptoStor.lib") // Debug
#else
#pragma comment(lib, "\\Release\\X64\\SdCryptoStor.lib") // Release
#endif
#endif



BYTE* EncodeAesCbc(BYTE* szIn,ULONG nInLen,BYTE* szKey,int nKeyLen,ULONG& nOutLen,BYTE *szIV )
{
	AES_KEY aes_key;
	if(AES_set_encrypt_key(szKey, nKeyLen * 8, &aes_key) < 0)
	{
		ATLASSERT(false);
		return NULL;
	}
	int data_length = nInLen;
	int padding = 0,nLeft = data_length % AES_BLOCK_SIZE;
	if (nLeft > 0)
	{
		padding =  AES_BLOCK_SIZE - nLeft;
	}
	/// 补齐长度
	data_length += padding;

	BYTE* szInData = new BYTE[data_length],*szOutData = new BYTE[data_length];
	if(NULL == szInData || NULL == szOutData)
		return NULL;
	::memset(szInData,'\0',data_length);
	::memset(szOutData,0,data_length);
	::memcpy(szInData,szIn,data_length-padding);

	BYTE* pIn = szInData,*pOut = szOutData;
	BYTE Temp[AES_BLOCK_SIZE];
	int nCount = data_length/AES_BLOCK_SIZE;
	for(int i = 0; i < nCount; i++)
	{
		::memset(Temp, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt(pIn,Temp,AES_BLOCK_SIZE,&aes_key,szIV,AES_ENCRYPT);
		::memcpy(pOut,Temp,AES_BLOCK_SIZE);
		pIn += AES_BLOCK_SIZE;
		pOut += AES_BLOCK_SIZE;
		nOutLen += AES_BLOCK_SIZE;
	}
	delete []szInData;
	szInData = NULL;
	return szOutData;
}

BYTE* DecodeAesCbc(BYTE* szIn,ULONG nInLen,BYTE* szKey,int nKeyLen,ULONG& nOutLen,BYTE *szIV )
{
	AES_KEY aes_key;
	if(AES_set_decrypt_key(szKey, nKeyLen * 8, &aes_key) < 0)
	{
		ATLASSERT(false);
		return NULL;
	}
	int data_length = nInLen;
	BYTE* szInData = new BYTE[data_length],*szOutData = new BYTE[data_length];
	if(NULL == szInData || NULL == szOutData)
		return NULL;
	::memset(szInData,0,data_length);
	::memset(szOutData,0,data_length);
	::memcpy(szInData,szIn,data_length);

	BYTE Temp[AES_BLOCK_SIZE];
	BYTE* pIn = szInData,*pOut = szOutData;
	int nCount = data_length/AES_BLOCK_SIZE;
	for(int i = 0; i < nCount; i++)
	{
		::memset(Temp, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt(pIn,Temp,AES_BLOCK_SIZE,&aes_key,\
			szIV,AES_DECRYPT);
		::memcpy(pOut,Temp,AES_BLOCK_SIZE);
		pIn += AES_BLOCK_SIZE;
		pOut += AES_BLOCK_SIZE;
		nOutLen += AES_BLOCK_SIZE;
	}
	delete []szInData;
	szInData = NULL;
	return szOutData;
}

BYTE* EncodeAesEcb(BYTE* szIn,ULONG nInLen,BYTE* szKey,ULONG& nOutLen,int nKeyLen )
{
	AES_KEY aes_key;
	if(AES_set_encrypt_key(szKey, nKeyLen * 8, &aes_key) < 0)
	{
		ATLASSERT(false);
		return NULL;
	}
	int data_length = nInLen;
	int padding = 0,nLeft = data_length % AES_BLOCK_SIZE;
	if (nLeft > 0)
	{
		padding =  AES_BLOCK_SIZE - nLeft;
	}
	/// 补齐长度
	data_length += padding;

	BYTE* szInData = new BYTE[data_length],*szOutData = new BYTE[data_length];
	if(NULL == szInData || NULL == szOutData)
		return NULL;
	::memset(szInData,'\0',data_length);
	::memset(szOutData,0,data_length);
	::memcpy(szInData,szIn,data_length-padding);

	BYTE* pIn = szInData,*pOut = szOutData;
	BYTE Temp[AES_BLOCK_SIZE];
	int nCount = data_length/AES_BLOCK_SIZE;

	for(int i = 0; i < data_length/AES_BLOCK_SIZE; i++)
	{
		::memset(Temp, 0, AES_BLOCK_SIZE);
		AES_encrypt(pIn, Temp, &aes_key);
		::memcpy(pOut,Temp,AES_BLOCK_SIZE);
		pIn += AES_BLOCK_SIZE;
		pOut += AES_BLOCK_SIZE;
		nOutLen += AES_BLOCK_SIZE;
	}
	delete []szInData;
	szInData = NULL;
	return szOutData;
}

BYTE* DecodeAesEcb(BYTE* szIn,ULONG nInLen,BYTE* szKey,ULONG& nOutLen,int nKeyLen )
{
	AES_KEY aes_key;
	if(AES_set_decrypt_key(szKey, nKeyLen * 8, &aes_key) < 0)
	{
		ATLASSERT(false);
		return NULL;
	}
	int data_length = nInLen;
	BYTE* szInData = new BYTE[data_length],*szOutData = new BYTE[data_length];
	if(NULL == szInData || NULL == szOutData)
		return NULL;
	::memset(szInData,0,data_length);
	::memset(szOutData,0,data_length);
	::memcpy(szInData,szIn,data_length);

	BYTE Temp[AES_BLOCK_SIZE];
	BYTE* pIn = szInData,*pOut = szOutData;
	int nCount = data_length/AES_BLOCK_SIZE;
	for(int i = 0; i < data_length/AES_BLOCK_SIZE; i++)
	{
		::memset(Temp, 0, AES_BLOCK_SIZE);
		AES_decrypt(pIn, Temp, &aes_key);
		::memcpy(pOut,Temp,AES_BLOCK_SIZE);
		pIn += AES_BLOCK_SIZE;
		pOut += AES_BLOCK_SIZE;
		nOutLen += AES_BLOCK_SIZE;
	}
	delete []szInData;
	szInData = NULL;
	return szOutData;
}

// CSoftEncry
STDMETHODIMP CSoftEncry::AesEcbEnc(BYTE* szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** pVal)
{
	// TODO: 在此添加实现代码
	if(nInLen < 1 || NULL == szIn || NULL == szKey || NULL == pnOutLen || NULL == pVal)
		return E_POINTER;
	int nKeyLen = (int)::strlen((char *)szKey);
	if(16 != nKeyLen && 24 != nKeyLen && 32 != nKeyLen)
		return E_FAIL;
	(*pVal) = EncodeAesEcb(szIn,nInLen,szKey,*pnOutLen,nKeyLen);
	if(NULL == (*pVal))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSoftEncry::AesEcbDes(BYTE* szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** pVal)
{
	// TODO: 在此添加实现代码
	if(nInLen < 1 || NULL == szIn || NULL == szKey || NULL == pnOutLen || NULL == pVal)
		return E_POINTER;
	int nKeyLen = (int)::strlen((char* )szKey);
	if(16 != nKeyLen && 24 != nKeyLen && 32 != nKeyLen)
		return E_FAIL;
	(*pVal) = DecodeAesEcb(szIn,nInLen,szKey,*pnOutLen,nKeyLen);
	if(NULL == (*pVal))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSoftEncry::AesCbcEnc(BYTE* szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** pVal)
{
	// TODO: 在此添加实现代码
	if(nInLen < 1 || NULL == szIn || NULL == szKey || NULL == pnOutLen || NULL == pVal)
		return E_POINTER;
	int nKeyLen = (int)::strlen((char*)szKey);
	if(16 != nKeyLen && 24 != nKeyLen && 32 != nKeyLen)
		return E_FAIL;
	(*pVal) = EncodeAesCbc(szIn,nInLen,szKey,nKeyLen,*pnOutLen,szIV);
	if(NULL == (*pVal))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSoftEncry::ReleaseBuf(BYTE* pBuf)
{
	if(NULL == pBuf)
		return E_POINTER;
	delete []pBuf;
	pBuf = NULL;
	return S_OK;
}

STDMETHODIMP CSoftEncry::AesCbcDes(BYTE* szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** pVal)
{
	// TODO: 在此添加实现代码
	if(nInLen < 1 || NULL == szIn || NULL == szKey || NULL == pnOutLen || NULL == pVal)
		return E_POINTER;
	int nKeyLen = (int)::strlen((char *)szKey);
	if(16 != nKeyLen && 24 != nKeyLen && 32 != nKeyLen)
		return E_FAIL;
	(*pVal) = DecodeAesCbc(szIn,nInLen,szKey,nKeyLen,*pnOutLen,szIV);
	if(NULL == (*pVal))
		return E_FAIL;
	return S_OK;
}