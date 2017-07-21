

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jul 21 14:25:32 2017
 */
/* Compiler settings for SKFEncry.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __SKFEncry_i_h__
#define __SKFEncry_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISafeService_FWD_DEFINED__
#define __ISafeService_FWD_DEFINED__
typedef interface ISafeService ISafeService;
#endif 	/* __ISafeService_FWD_DEFINED__ */


#ifndef __ISafeCard_FWD_DEFINED__
#define __ISafeCard_FWD_DEFINED__
typedef interface ISafeCard ISafeCard;
#endif 	/* __ISafeCard_FWD_DEFINED__ */


#ifndef __ICryptoStor_FWD_DEFINED__
#define __ICryptoStor_FWD_DEFINED__
typedef interface ICryptoStor ICryptoStor;
#endif 	/* __ICryptoStor_FWD_DEFINED__ */


#ifndef __IWMIDevice_FWD_DEFINED__
#define __IWMIDevice_FWD_DEFINED__
typedef interface IWMIDevice IWMIDevice;
#endif 	/* __IWMIDevice_FWD_DEFINED__ */


#ifndef __IUSBDevice_FWD_DEFINED__
#define __IUSBDevice_FWD_DEFINED__
typedef interface IUSBDevice IUSBDevice;
#endif 	/* __IUSBDevice_FWD_DEFINED__ */


#ifndef __ISoftEncry_FWD_DEFINED__
#define __ISoftEncry_FWD_DEFINED__
typedef interface ISoftEncry ISoftEncry;
#endif 	/* __ISoftEncry_FWD_DEFINED__ */


#ifndef ___ISafeServiceEvents_FWD_DEFINED__
#define ___ISafeServiceEvents_FWD_DEFINED__
typedef interface _ISafeServiceEvents _ISafeServiceEvents;
#endif 	/* ___ISafeServiceEvents_FWD_DEFINED__ */


#ifndef __SafeService_FWD_DEFINED__
#define __SafeService_FWD_DEFINED__

#ifdef __cplusplus
typedef class SafeService SafeService;
#else
typedef struct SafeService SafeService;
#endif /* __cplusplus */

#endif 	/* __SafeService_FWD_DEFINED__ */


#ifndef ___ISafeCardEvents_FWD_DEFINED__
#define ___ISafeCardEvents_FWD_DEFINED__
typedef interface _ISafeCardEvents _ISafeCardEvents;
#endif 	/* ___ISafeCardEvents_FWD_DEFINED__ */


#ifndef __SafeCard_FWD_DEFINED__
#define __SafeCard_FWD_DEFINED__

#ifdef __cplusplus
typedef class SafeCard SafeCard;
#else
typedef struct SafeCard SafeCard;
#endif /* __cplusplus */

#endif 	/* __SafeCard_FWD_DEFINED__ */


#ifndef ___ICryptoStorEvents_FWD_DEFINED__
#define ___ICryptoStorEvents_FWD_DEFINED__
typedef interface _ICryptoStorEvents _ICryptoStorEvents;
#endif 	/* ___ICryptoStorEvents_FWD_DEFINED__ */


#ifndef __CryptoStor_FWD_DEFINED__
#define __CryptoStor_FWD_DEFINED__

#ifdef __cplusplus
typedef class CryptoStor CryptoStor;
#else
typedef struct CryptoStor CryptoStor;
#endif /* __cplusplus */

#endif 	/* __CryptoStor_FWD_DEFINED__ */


#ifndef __WMIDevice_FWD_DEFINED__
#define __WMIDevice_FWD_DEFINED__

#ifdef __cplusplus
typedef class WMIDevice WMIDevice;
#else
typedef struct WMIDevice WMIDevice;
#endif /* __cplusplus */

#endif 	/* __WMIDevice_FWD_DEFINED__ */


#ifndef __USBDevice_FWD_DEFINED__
#define __USBDevice_FWD_DEFINED__

#ifdef __cplusplus
typedef class USBDevice USBDevice;
#else
typedef struct USBDevice USBDevice;
#endif /* __cplusplus */

#endif 	/* __USBDevice_FWD_DEFINED__ */


#ifndef __SoftEncry_FWD_DEFINED__
#define __SoftEncry_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoftEncry SoftEncry;
#else
typedef struct SoftEncry SoftEncry;
#endif /* __cplusplus */

#endif 	/* __SoftEncry_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_SKFEncry_0000_0000 */
/* [local] */ 

typedef 
enum ESKFAappFileSecureType
    {	SKFAPP_FILESECURETYPE_NEVER	= 0,
	SKFAPP_FILESECURETYPE_ADM	= 0x1,
	SKFAPP_FILESECURETYPE_USER	= 0x10,
	SKFAPP_FILESECURETYPE_ANYONE	= 0xff
    } 	EAappFileSecureType;

typedef 
enum EHXWMIType
    {	WMITYPE_NETMACADDR	= 0,
	WMITYPE_HARDID	= 1,
	WMITYPE_BOARDID	= 2,
	WMITYPE_PROCID	= 3,
	WMITYPE_BIOSID	= 4,
	WMITYPE_USBID	= 5,
	WMITYPE_LOGICDISK	= 6
    } 	EWMIType;






extern RPC_IF_HANDLE __MIDL_itf_SKFEncry_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SKFEncry_0000_0000_v0_0_s_ifspec;

#ifndef __ISafeService_INTERFACE_DEFINED__
#define __ISafeService_INTERFACE_DEFINED__

/* interface ISafeService */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISafeService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("AC0495FC-7C4B-4BB0-AC20-24AF6C6CDEAC")
    ISafeService : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SafeCard( 
            /* [retval][out] */ ISafeCard **ppVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CryptoStor( 
            /* [retval][out] */ ICryptoStor **ppVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurDiskName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SoftEncry( 
            /* [retval][out] */ ISoftEncry **ppVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CustomerName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CustomerID( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DefaultPW( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AuthNode( 
            /* [in] */ USHORT newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ValidDay( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AuthPacket( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DBPW( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CID( 
            /* [in] */ VARIANT_BOOL bInstallFirst,
            /* [out][in] */ BSTR *pbstrDiskName,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_InstallDisk( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_InstallDisk( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FileSha1( 
            /* [in] */ BSTR bstrFilePath,
            /* [retval][out] */ BSTR *pbstrSha1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StringSha1( 
            /* [in] */ BSTR bstrString,
            /* [retval][out] */ BSTR *pbstrSha1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FileMD5( 
            /* [in] */ BSTR bstrFilePath,
            /* [out][in] */ unsigned char *pbMD5) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StringMD5( 
            /* [in] */ BSTR bstrString,
            /* [retval][out] */ BSTR *pbstrMD5) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FileMD5String( 
            /* [in] */ BSTR bstrFilePath,
            /* [retval][out] */ BSTR *pbstrMD5) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TransToString( 
            /* [in] */ BYTE *pbMD5,
            /* [in] */ SHORT nLen,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetInstallDisk( 
            /* [in] */ VARIANT_BOOL bCID,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseSafeCard( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Auth( 
            /* [in] */ BSTR bstrDeviceID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetLeftCount( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUniqueID( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDiskNum( 
            /* [in] */ BSTR bstrDiskNum,
            /* [in] */ VARIANT_BOOL bFlag) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISafeServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISafeService * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISafeService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISafeService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISafeService * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISafeService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISafeService * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISafeService * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SafeCard )( 
            ISafeService * This,
            /* [retval][out] */ ISafeCard **ppVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CryptoStor )( 
            ISafeService * This,
            /* [retval][out] */ ICryptoStor **ppVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurDiskName )( 
            ISafeService * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SoftEncry )( 
            ISafeService * This,
            /* [retval][out] */ ISoftEncry **ppVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CustomerName )( 
            ISafeService * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CustomerID )( 
            ISafeService * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DefaultPW )( 
            ISafeService * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AuthNode )( 
            ISafeService * This,
            /* [in] */ USHORT newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ValidDay )( 
            ISafeService * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AuthPacket )( 
            ISafeService * This,
            /* [in] */ ULONG newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DBPW )( 
            ISafeService * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CID )( 
            ISafeService * This,
            /* [in] */ VARIANT_BOOL bInstallFirst,
            /* [out][in] */ BSTR *pbstrDiskName,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_InstallDisk )( 
            ISafeService * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InstallDisk )( 
            ISafeService * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FileSha1 )( 
            ISafeService * This,
            /* [in] */ BSTR bstrFilePath,
            /* [retval][out] */ BSTR *pbstrSha1);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StringSha1 )( 
            ISafeService * This,
            /* [in] */ BSTR bstrString,
            /* [retval][out] */ BSTR *pbstrSha1);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FileMD5 )( 
            ISafeService * This,
            /* [in] */ BSTR bstrFilePath,
            /* [out][in] */ unsigned char *pbMD5);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StringMD5 )( 
            ISafeService * This,
            /* [in] */ BSTR bstrString,
            /* [retval][out] */ BSTR *pbstrMD5);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FileMD5String )( 
            ISafeService * This,
            /* [in] */ BSTR bstrFilePath,
            /* [retval][out] */ BSTR *pbstrMD5);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TransToString )( 
            ISafeService * This,
            /* [in] */ BYTE *pbMD5,
            /* [in] */ SHORT nLen,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetInstallDisk )( 
            ISafeService * This,
            /* [in] */ VARIANT_BOOL bCID,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseSafeCard )( 
            ISafeService * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Auth )( 
            ISafeService * This,
            /* [in] */ BSTR bstrDeviceID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetLeftCount )( 
            ISafeService * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetUniqueID )( 
            ISafeService * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDiskNum )( 
            ISafeService * This,
            /* [in] */ BSTR bstrDiskNum,
            /* [in] */ VARIANT_BOOL bFlag);
        
        END_INTERFACE
    } ISafeServiceVtbl;

    interface ISafeService
    {
        CONST_VTBL struct ISafeServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISafeService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISafeService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISafeService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISafeService_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISafeService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISafeService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISafeService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISafeService_get_SafeCard(This,ppVal)	\
    ( (This)->lpVtbl -> get_SafeCard(This,ppVal) ) 

#define ISafeService_get_CryptoStor(This,ppVal)	\
    ( (This)->lpVtbl -> get_CryptoStor(This,ppVal) ) 

#define ISafeService_get_CurDiskName(This,pVal)	\
    ( (This)->lpVtbl -> get_CurDiskName(This,pVal) ) 

#define ISafeService_get_SoftEncry(This,ppVal)	\
    ( (This)->lpVtbl -> get_SoftEncry(This,ppVal) ) 

#define ISafeService_put_CustomerName(This,newVal)	\
    ( (This)->lpVtbl -> put_CustomerName(This,newVal) ) 

#define ISafeService_put_CustomerID(This,newVal)	\
    ( (This)->lpVtbl -> put_CustomerID(This,newVal) ) 

#define ISafeService_put_DefaultPW(This,newVal)	\
    ( (This)->lpVtbl -> put_DefaultPW(This,newVal) ) 

#define ISafeService_put_AuthNode(This,newVal)	\
    ( (This)->lpVtbl -> put_AuthNode(This,newVal) ) 

#define ISafeService_put_ValidDay(This,newVal)	\
    ( (This)->lpVtbl -> put_ValidDay(This,newVal) ) 

#define ISafeService_put_AuthPacket(This,newVal)	\
    ( (This)->lpVtbl -> put_AuthPacket(This,newVal) ) 

#define ISafeService_put_DBPW(This,newVal)	\
    ( (This)->lpVtbl -> put_DBPW(This,newVal) ) 

#define ISafeService_get_CID(This,bInstallFirst,pbstrDiskName,pVal)	\
    ( (This)->lpVtbl -> get_CID(This,bInstallFirst,pbstrDiskName,pVal) ) 

#define ISafeService_put_InstallDisk(This,newVal)	\
    ( (This)->lpVtbl -> put_InstallDisk(This,newVal) ) 

#define ISafeService_get_InstallDisk(This,pVal)	\
    ( (This)->lpVtbl -> get_InstallDisk(This,pVal) ) 

#define ISafeService_FileSha1(This,bstrFilePath,pbstrSha1)	\
    ( (This)->lpVtbl -> FileSha1(This,bstrFilePath,pbstrSha1) ) 

#define ISafeService_StringSha1(This,bstrString,pbstrSha1)	\
    ( (This)->lpVtbl -> StringSha1(This,bstrString,pbstrSha1) ) 

#define ISafeService_FileMD5(This,bstrFilePath,pbMD5)	\
    ( (This)->lpVtbl -> FileMD5(This,bstrFilePath,pbMD5) ) 

#define ISafeService_StringMD5(This,bstrString,pbstrMD5)	\
    ( (This)->lpVtbl -> StringMD5(This,bstrString,pbstrMD5) ) 

#define ISafeService_FileMD5String(This,bstrFilePath,pbstrMD5)	\
    ( (This)->lpVtbl -> FileMD5String(This,bstrFilePath,pbstrMD5) ) 

#define ISafeService_TransToString(This,pbMD5,nLen,pVal)	\
    ( (This)->lpVtbl -> TransToString(This,pbMD5,nLen,pVal) ) 

#define ISafeService_GetInstallDisk(This,bCID,pVal)	\
    ( (This)->lpVtbl -> GetInstallDisk(This,bCID,pVal) ) 

#define ISafeService_CloseSafeCard(This)	\
    ( (This)->lpVtbl -> CloseSafeCard(This) ) 

#define ISafeService_Auth(This,bstrDeviceID)	\
    ( (This)->lpVtbl -> Auth(This,bstrDeviceID) ) 

#define ISafeService_GetLeftCount(This,pVal)	\
    ( (This)->lpVtbl -> GetLeftCount(This,pVal) ) 

#define ISafeService_GetUniqueID(This,pVal)	\
    ( (This)->lpVtbl -> GetUniqueID(This,pVal) ) 

#define ISafeService_SetDiskNum(This,bstrDiskNum,bFlag)	\
    ( (This)->lpVtbl -> SetDiskNum(This,bstrDiskNum,bFlag) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISafeService_INTERFACE_DEFINED__ */


#ifndef __ISafeCard_INTERFACE_DEFINED__
#define __ISafeCard_INTERFACE_DEFINED__

/* interface ISafeCard */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISafeCard;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4A047C80-FE61-44E6-B524-DD0F29E5F233")
    ISafeCard : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumCard( 
            /* [retval][out] */ BSTR *pbstrDisks) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCID( 
            /* [in] */ BSTR cDisk,
            /* [retval][out] */ BSTR *pbstrCID) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurDisk( 
            /* [retval][out] */ BSTR *pDisk) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CurDisk( 
            /* [in] */ BSTR cDisk) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LockDev( 
            /* [in] */ VARIANT_BOOL bLockFlag) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WaitForDevEvent( 
            /* [out] */ SHORT *pnEvent,
            /* [retval][out] */ BSTR *pbstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CancelWaitForDevEvent( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [retval][out] */ SHORT *psStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDeviceInfo( 
            /* [retval][out] */ IDispatch **ppDeviceInfo) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeDevAuth( 
            /* [in] */ unsigned char *szOldPW,
            /* [in] */ unsigned char *szNewPW) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DevAuth( 
            /* [in] */ unsigned char *szKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateApplication( 
            /* [in] */ BSTR bstrAppName,
            /* [in] */ BSTR bstrAdminPin,
            /* [in] */ SHORT nAdminPinRetryCount,
            /* [in] */ BSTR bstrUserPin,
            /* [in] */ SHORT nUserPinRetryCount,
            /* [in] */ EAappFileSecureType eCreateFileRight,
            /* [retval][out] */ ULONG *phApp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumApplication( 
            /* [retval][out] */ BSTR *pbstrApps) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteApplication( 
            /* [in] */ BSTR bstrAppName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenApplication( 
            /* [in] */ BSTR bstrAppName,
            /* [retval][out] */ ULONG *phApp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseApplication( 
            /* [in] */ ULONG hApp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangePIN( 
            /* [in] */ ULONG hApp,
            /* [in] */ VARIANT_BOOL bAdminFlag,
            /* [in] */ BSTR bstrOldPin,
            /* [in] */ BSTR bstrNewPin,
            /* [retval][out] */ SHORT *pnRetryCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPINInfo( 
            /* [in] */ ULONG hApp,
            /* [in] */ VARIANT_BOOL bAdminFlag,
            /* [out] */ SHORT *pnMaxRetryCount,
            /* [out] */ SHORT *pnRemainRetryCount,
            /* [retval][out] */ VARIANT_BOOL *pbDefaultPin) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VerifyPIN( 
            /* [in] */ ULONG hApp,
            /* [in] */ VARIANT_BOOL bAdminFlag,
            /* [in] */ BSTR bstrPin,
            /* [retval][out] */ SHORT *pnRetryCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnblockPIN( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrAdminPin,
            /* [in] */ BSTR bstrUserPin,
            /* [retval][out] */ SHORT *pnRetryCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearSecureState( 
            /* [in] */ ULONG hApp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateAppFile( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG nFileSize,
            /* [in] */ ULONG nReadRight,
            /* [in] */ ULONG nWriteRight) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteAppFile( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumAppFile( 
            /* [in] */ ULONG hApp,
            /* [retval][out] */ BSTR *bstrFiles) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAppFileInfo( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [retval][out] */ IDispatch **pPara) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadAppFile( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG nOffset,
            /* [out][in] */ SHORT *pnReadLen,
            /* [retval][out] */ BYTE *pbData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteAppFile( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG nOffset,
            /* [in] */ BYTE *pbData,
            /* [in] */ ULONG nSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateContainer( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrContainerName,
            /* [retval][out] */ ULONG *phContainer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteContainer( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrContainerName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenContainer( 
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrContainerName,
            /* [retval][out] */ ULONG *phContainer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseContainer( 
            /* [in] */ ULONG hContainer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumContainer( 
            /* [in] */ ULONG hApp,
            /* [retval][out] */ BSTR *pbstrNameList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetContainerType( 
            /* [in] */ ULONG hContainer,
            /* [retval][out] */ SHORT *psType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportCertificate( 
            /* [in] */ ULONG hContainer,
            /* [in] */ VARIANT_BOOL bSign,
            /* [in] */ BYTE *pbCert,
            /* [in] */ SHORT nCertLen) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExportCertificate( 
            /* [in] */ ULONG hContainer,
            /* [in] */ VARIANT_BOOL bSign,
            /* [out][in] */ SHORT *pnCertLen,
            /* [retval][out] */ BYTE *pbCert) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenRandom( 
            /* [in] */ SHORT nRandomLen,
            /* [retval][out] */ BSTR *pbstrRandom) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenExtRSAKey( 
            /* [in] */ ULONG nBitsLen,
            /* [out] */ ULONG *pRsaPrivateKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenRSAKeyPair( 
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nBitsLen,
            /* [out] */ ULONG *pRsaPubKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportRSAKeyPair( 
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nSymAlgId,
            /* [in] */ BYTE *pbWrappedKey,
            /* [in] */ ULONG nWrappedKeyLen,
            /* [in] */ BYTE *pbEncryptedData,
            /* [in] */ ULONG nEncryptedDataLen) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RSASignData( 
            /* [in] */ ULONG hContainer,
            /* [in] */ SHORT nDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ SHORT *pnSignature,
            /* [retval][out] */ BYTE *pbSignature) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RSAVerify( 
            /* [in] */ ULONG *pRsaPubKey,
            /* [in] */ SHORT nDataLen,
            /* [in] */ BYTE *pbData,
            /* [in] */ SHORT nSignatureLen,
            /* [in] */ BYTE *pbSignature) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RSAExportSessionKey( 
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nAlgId,
            /* [in] */ ULONG *pRsaPubKey,
            /* [out][in] */ SHORT *pnDataLen,
            /* [out] */ BYTE *pbData,
            /* [retval][out] */ ULONG *phSessionKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtRSAPubKeyOperation( 
            /* [in] */ ULONG *pRsaPubKey,
            /* [in] */ SHORT nInputLen,
            /* [out] */ BYTE *pbInputData,
            /* [out][in] */ SHORT *pnOutput,
            /* [retval][out] */ BYTE *pbOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExtRSAPriKeyOperation( 
            /* [in] */ ULONG *pRsaPubKey,
            /* [in] */ SHORT nInputLen,
            /* [out] */ BYTE *pbInputData,
            /* [out][in] */ SHORT *pnOutput,
            /* [retval][out] */ BYTE *pbOutput) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExportPublicKey( 
            /* [in] */ ULONG hContainer,
            /* [in] */ VARIANT_BOOL bSignFlag,
            /* [out][in] */ SHORT *pnBlob,
            /* [out] */ ULONG *pbBlob) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportSessionKey( 
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nAlgId,
            /* [in] */ ULONG nWrapedLen,
            /* [in] */ BYTE *pbWrapedData,
            /* [retval][out] */ ULONG *phSessionKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSymmKey( 
            /* [in] */ ULONG nAlgId,
            /* [in] */ BYTE *pbKey,
            /* [retval][out] */ ULONG *phSessionKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncryptInit( 
            /* [in] */ ULONG hKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Encrypt( 
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ ULONG *pnEncryptedDataLen,
            /* [retval][out] */ BYTE *pbEncryptedData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncryptUpdate( 
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ ULONG *pnEncryptedDataLen,
            /* [retval][out] */ BYTE *pbEncryptedData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EncryptFinal( 
            /* [in] */ ULONG hKey,
            /* [out][in] */ ULONG *pnEncryptedDataLen,
            /* [retval][out] */ BYTE *pbEncryptedData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecryptInit( 
            /* [in] */ ULONG hKey) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Decrypt( 
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nEncryptedDataLen,
            /* [in] */ BYTE *pbEncryptedData,
            /* [out][in] */ ULONG *pnDataLen,
            /* [retval][out] */ BYTE *pbData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecryptUpdate( 
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nEncryptedDataLen,
            /* [in] */ BYTE *pbEncryptedData,
            /* [out][in] */ ULONG *pnDataLen,
            /* [retval][out] */ BYTE *pbData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DecryptFinal( 
            /* [in] */ ULONG hKey,
            /* [out][in] */ ULONG *pnDecryptedLen,
            /* [retval][out] */ BYTE *pbDecryptedData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DigestInit( 
            /* [in] */ ULONG ulAlgID,
            /* [in] */ ULONG *pPubKey,
            /* [in] */ SHORT nIDLen,
            /* [in] */ unsigned char *pucID,
            /* [retval][out] */ ULONG *phHash) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Digest( 
            /* [in] */ ULONG hHash,
            /* [in] */ SHORT ulDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ SHORT *pnHashLen,
            /* [retval][out] */ BYTE *pbHashData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DigestUpdate( 
            /* [in] */ ULONG hHash,
            /* [in] */ SHORT ulDataLen,
            /* [in] */ BYTE *pbData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DigestFinal( 
            /* [in] */ ULONG hHash,
            /* [out][in] */ SHORT *pnHashLen,
            /* [retval][out] */ BYTE *pbHashData) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseHandle( 
            /* [in] */ ULONG hHandle) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SKFType( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrCode( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrInfo( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseDev( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISafeCardVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISafeCard * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISafeCard * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISafeCard * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISafeCard * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISafeCard * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISafeCard * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISafeCard * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumCard )( 
            ISafeCard * This,
            /* [retval][out] */ BSTR *pbstrDisks);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCID )( 
            ISafeCard * This,
            /* [in] */ BSTR cDisk,
            /* [retval][out] */ BSTR *pbstrCID);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurDisk )( 
            ISafeCard * This,
            /* [retval][out] */ BSTR *pDisk);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CurDisk )( 
            ISafeCard * This,
            /* [in] */ BSTR cDisk);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LockDev )( 
            ISafeCard * This,
            /* [in] */ VARIANT_BOOL bLockFlag);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WaitForDevEvent )( 
            ISafeCard * This,
            /* [out] */ SHORT *pnEvent,
            /* [retval][out] */ BSTR *pbstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CancelWaitForDevEvent )( 
            ISafeCard * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            ISafeCard * This,
            /* [retval][out] */ SHORT *psStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDeviceInfo )( 
            ISafeCard * This,
            /* [retval][out] */ IDispatch **ppDeviceInfo);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeDevAuth )( 
            ISafeCard * This,
            /* [in] */ unsigned char *szOldPW,
            /* [in] */ unsigned char *szNewPW);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DevAuth )( 
            ISafeCard * This,
            /* [in] */ unsigned char *szKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateApplication )( 
            ISafeCard * This,
            /* [in] */ BSTR bstrAppName,
            /* [in] */ BSTR bstrAdminPin,
            /* [in] */ SHORT nAdminPinRetryCount,
            /* [in] */ BSTR bstrUserPin,
            /* [in] */ SHORT nUserPinRetryCount,
            /* [in] */ EAappFileSecureType eCreateFileRight,
            /* [retval][out] */ ULONG *phApp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumApplication )( 
            ISafeCard * This,
            /* [retval][out] */ BSTR *pbstrApps);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteApplication )( 
            ISafeCard * This,
            /* [in] */ BSTR bstrAppName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenApplication )( 
            ISafeCard * This,
            /* [in] */ BSTR bstrAppName,
            /* [retval][out] */ ULONG *phApp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseApplication )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangePIN )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ VARIANT_BOOL bAdminFlag,
            /* [in] */ BSTR bstrOldPin,
            /* [in] */ BSTR bstrNewPin,
            /* [retval][out] */ SHORT *pnRetryCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPINInfo )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ VARIANT_BOOL bAdminFlag,
            /* [out] */ SHORT *pnMaxRetryCount,
            /* [out] */ SHORT *pnRemainRetryCount,
            /* [retval][out] */ VARIANT_BOOL *pbDefaultPin);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VerifyPIN )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ VARIANT_BOOL bAdminFlag,
            /* [in] */ BSTR bstrPin,
            /* [retval][out] */ SHORT *pnRetryCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnblockPIN )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrAdminPin,
            /* [in] */ BSTR bstrUserPin,
            /* [retval][out] */ SHORT *pnRetryCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearSecureState )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateAppFile )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG nFileSize,
            /* [in] */ ULONG nReadRight,
            /* [in] */ ULONG nWriteRight);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteAppFile )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumAppFile )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [retval][out] */ BSTR *bstrFiles);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAppFileInfo )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [retval][out] */ IDispatch **pPara);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadAppFile )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG nOffset,
            /* [out][in] */ SHORT *pnReadLen,
            /* [retval][out] */ BYTE *pbData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteAppFile )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ ULONG nOffset,
            /* [in] */ BYTE *pbData,
            /* [in] */ ULONG nSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateContainer )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrContainerName,
            /* [retval][out] */ ULONG *phContainer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteContainer )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrContainerName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenContainer )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [in] */ BSTR bstrContainerName,
            /* [retval][out] */ ULONG *phContainer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseContainer )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumContainer )( 
            ISafeCard * This,
            /* [in] */ ULONG hApp,
            /* [retval][out] */ BSTR *pbstrNameList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetContainerType )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [retval][out] */ SHORT *psType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportCertificate )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ VARIANT_BOOL bSign,
            /* [in] */ BYTE *pbCert,
            /* [in] */ SHORT nCertLen);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExportCertificate )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ VARIANT_BOOL bSign,
            /* [out][in] */ SHORT *pnCertLen,
            /* [retval][out] */ BYTE *pbCert);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenRandom )( 
            ISafeCard * This,
            /* [in] */ SHORT nRandomLen,
            /* [retval][out] */ BSTR *pbstrRandom);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenExtRSAKey )( 
            ISafeCard * This,
            /* [in] */ ULONG nBitsLen,
            /* [out] */ ULONG *pRsaPrivateKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenRSAKeyPair )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nBitsLen,
            /* [out] */ ULONG *pRsaPubKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportRSAKeyPair )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nSymAlgId,
            /* [in] */ BYTE *pbWrappedKey,
            /* [in] */ ULONG nWrappedKeyLen,
            /* [in] */ BYTE *pbEncryptedData,
            /* [in] */ ULONG nEncryptedDataLen);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RSASignData )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ SHORT nDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ SHORT *pnSignature,
            /* [retval][out] */ BYTE *pbSignature);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RSAVerify )( 
            ISafeCard * This,
            /* [in] */ ULONG *pRsaPubKey,
            /* [in] */ SHORT nDataLen,
            /* [in] */ BYTE *pbData,
            /* [in] */ SHORT nSignatureLen,
            /* [in] */ BYTE *pbSignature);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RSAExportSessionKey )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nAlgId,
            /* [in] */ ULONG *pRsaPubKey,
            /* [out][in] */ SHORT *pnDataLen,
            /* [out] */ BYTE *pbData,
            /* [retval][out] */ ULONG *phSessionKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExtRSAPubKeyOperation )( 
            ISafeCard * This,
            /* [in] */ ULONG *pRsaPubKey,
            /* [in] */ SHORT nInputLen,
            /* [out] */ BYTE *pbInputData,
            /* [out][in] */ SHORT *pnOutput,
            /* [retval][out] */ BYTE *pbOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExtRSAPriKeyOperation )( 
            ISafeCard * This,
            /* [in] */ ULONG *pRsaPubKey,
            /* [in] */ SHORT nInputLen,
            /* [out] */ BYTE *pbInputData,
            /* [out][in] */ SHORT *pnOutput,
            /* [retval][out] */ BYTE *pbOutput);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExportPublicKey )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ VARIANT_BOOL bSignFlag,
            /* [out][in] */ SHORT *pnBlob,
            /* [out] */ ULONG *pbBlob);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportSessionKey )( 
            ISafeCard * This,
            /* [in] */ ULONG hContainer,
            /* [in] */ ULONG nAlgId,
            /* [in] */ ULONG nWrapedLen,
            /* [in] */ BYTE *pbWrapedData,
            /* [retval][out] */ ULONG *phSessionKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSymmKey )( 
            ISafeCard * This,
            /* [in] */ ULONG nAlgId,
            /* [in] */ BYTE *pbKey,
            /* [retval][out] */ ULONG *phSessionKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncryptInit )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Encrypt )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ ULONG *pnEncryptedDataLen,
            /* [retval][out] */ BYTE *pbEncryptedData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncryptUpdate )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ ULONG *pnEncryptedDataLen,
            /* [retval][out] */ BYTE *pbEncryptedData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EncryptFinal )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey,
            /* [out][in] */ ULONG *pnEncryptedDataLen,
            /* [retval][out] */ BYTE *pbEncryptedData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecryptInit )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Decrypt )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nEncryptedDataLen,
            /* [in] */ BYTE *pbEncryptedData,
            /* [out][in] */ ULONG *pnDataLen,
            /* [retval][out] */ BYTE *pbData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecryptUpdate )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey,
            /* [in] */ ULONG nEncryptedDataLen,
            /* [in] */ BYTE *pbEncryptedData,
            /* [out][in] */ ULONG *pnDataLen,
            /* [retval][out] */ BYTE *pbData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DecryptFinal )( 
            ISafeCard * This,
            /* [in] */ ULONG hKey,
            /* [out][in] */ ULONG *pnDecryptedLen,
            /* [retval][out] */ BYTE *pbDecryptedData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DigestInit )( 
            ISafeCard * This,
            /* [in] */ ULONG ulAlgID,
            /* [in] */ ULONG *pPubKey,
            /* [in] */ SHORT nIDLen,
            /* [in] */ unsigned char *pucID,
            /* [retval][out] */ ULONG *phHash);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Digest )( 
            ISafeCard * This,
            /* [in] */ ULONG hHash,
            /* [in] */ SHORT ulDataLen,
            /* [in] */ BYTE *pbData,
            /* [out][in] */ SHORT *pnHashLen,
            /* [retval][out] */ BYTE *pbHashData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DigestUpdate )( 
            ISafeCard * This,
            /* [in] */ ULONG hHash,
            /* [in] */ SHORT ulDataLen,
            /* [in] */ BYTE *pbData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DigestFinal )( 
            ISafeCard * This,
            /* [in] */ ULONG hHash,
            /* [out][in] */ SHORT *pnHashLen,
            /* [retval][out] */ BYTE *pbHashData);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseHandle )( 
            ISafeCard * This,
            /* [in] */ ULONG hHandle);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SKFType )( 
            ISafeCard * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrCode )( 
            ISafeCard * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrInfo )( 
            ISafeCard * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseDev )( 
            ISafeCard * This);
        
        END_INTERFACE
    } ISafeCardVtbl;

    interface ISafeCard
    {
        CONST_VTBL struct ISafeCardVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISafeCard_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISafeCard_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISafeCard_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISafeCard_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISafeCard_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISafeCard_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISafeCard_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISafeCard_EnumCard(This,pbstrDisks)	\
    ( (This)->lpVtbl -> EnumCard(This,pbstrDisks) ) 

#define ISafeCard_GetCID(This,cDisk,pbstrCID)	\
    ( (This)->lpVtbl -> GetCID(This,cDisk,pbstrCID) ) 

#define ISafeCard_get_CurDisk(This,pDisk)	\
    ( (This)->lpVtbl -> get_CurDisk(This,pDisk) ) 

#define ISafeCard_put_CurDisk(This,cDisk)	\
    ( (This)->lpVtbl -> put_CurDisk(This,cDisk) ) 

#define ISafeCard_LockDev(This,bLockFlag)	\
    ( (This)->lpVtbl -> LockDev(This,bLockFlag) ) 

#define ISafeCard_WaitForDevEvent(This,pnEvent,pbstrName)	\
    ( (This)->lpVtbl -> WaitForDevEvent(This,pnEvent,pbstrName) ) 

#define ISafeCard_CancelWaitForDevEvent(This)	\
    ( (This)->lpVtbl -> CancelWaitForDevEvent(This) ) 

#define ISafeCard_GetStatus(This,psStatus)	\
    ( (This)->lpVtbl -> GetStatus(This,psStatus) ) 

#define ISafeCard_GetDeviceInfo(This,ppDeviceInfo)	\
    ( (This)->lpVtbl -> GetDeviceInfo(This,ppDeviceInfo) ) 

#define ISafeCard_ChangeDevAuth(This,szOldPW,szNewPW)	\
    ( (This)->lpVtbl -> ChangeDevAuth(This,szOldPW,szNewPW) ) 

#define ISafeCard_DevAuth(This,szKey)	\
    ( (This)->lpVtbl -> DevAuth(This,szKey) ) 

#define ISafeCard_CreateApplication(This,bstrAppName,bstrAdminPin,nAdminPinRetryCount,bstrUserPin,nUserPinRetryCount,eCreateFileRight,phApp)	\
    ( (This)->lpVtbl -> CreateApplication(This,bstrAppName,bstrAdminPin,nAdminPinRetryCount,bstrUserPin,nUserPinRetryCount,eCreateFileRight,phApp) ) 

#define ISafeCard_EnumApplication(This,pbstrApps)	\
    ( (This)->lpVtbl -> EnumApplication(This,pbstrApps) ) 

#define ISafeCard_DeleteApplication(This,bstrAppName)	\
    ( (This)->lpVtbl -> DeleteApplication(This,bstrAppName) ) 

#define ISafeCard_OpenApplication(This,bstrAppName,phApp)	\
    ( (This)->lpVtbl -> OpenApplication(This,bstrAppName,phApp) ) 

#define ISafeCard_CloseApplication(This,hApp)	\
    ( (This)->lpVtbl -> CloseApplication(This,hApp) ) 

#define ISafeCard_ChangePIN(This,hApp,bAdminFlag,bstrOldPin,bstrNewPin,pnRetryCount)	\
    ( (This)->lpVtbl -> ChangePIN(This,hApp,bAdminFlag,bstrOldPin,bstrNewPin,pnRetryCount) ) 

#define ISafeCard_GetPINInfo(This,hApp,bAdminFlag,pnMaxRetryCount,pnRemainRetryCount,pbDefaultPin)	\
    ( (This)->lpVtbl -> GetPINInfo(This,hApp,bAdminFlag,pnMaxRetryCount,pnRemainRetryCount,pbDefaultPin) ) 

#define ISafeCard_VerifyPIN(This,hApp,bAdminFlag,bstrPin,pnRetryCount)	\
    ( (This)->lpVtbl -> VerifyPIN(This,hApp,bAdminFlag,bstrPin,pnRetryCount) ) 

#define ISafeCard_UnblockPIN(This,hApp,bstrAdminPin,bstrUserPin,pnRetryCount)	\
    ( (This)->lpVtbl -> UnblockPIN(This,hApp,bstrAdminPin,bstrUserPin,pnRetryCount) ) 

#define ISafeCard_ClearSecureState(This,hApp)	\
    ( (This)->lpVtbl -> ClearSecureState(This,hApp) ) 

#define ISafeCard_CreateAppFile(This,hApp,bstrFileName,nFileSize,nReadRight,nWriteRight)	\
    ( (This)->lpVtbl -> CreateAppFile(This,hApp,bstrFileName,nFileSize,nReadRight,nWriteRight) ) 

#define ISafeCard_DeleteAppFile(This,hApp,bstrFileName)	\
    ( (This)->lpVtbl -> DeleteAppFile(This,hApp,bstrFileName) ) 

#define ISafeCard_EnumAppFile(This,hApp,bstrFiles)	\
    ( (This)->lpVtbl -> EnumAppFile(This,hApp,bstrFiles) ) 

#define ISafeCard_GetAppFileInfo(This,hApp,bstrFileName,pPara)	\
    ( (This)->lpVtbl -> GetAppFileInfo(This,hApp,bstrFileName,pPara) ) 

#define ISafeCard_ReadAppFile(This,hApp,bstrFileName,nOffset,pnReadLen,pbData)	\
    ( (This)->lpVtbl -> ReadAppFile(This,hApp,bstrFileName,nOffset,pnReadLen,pbData) ) 

#define ISafeCard_WriteAppFile(This,hApp,bstrFileName,nOffset,pbData,nSize)	\
    ( (This)->lpVtbl -> WriteAppFile(This,hApp,bstrFileName,nOffset,pbData,nSize) ) 

#define ISafeCard_CreateContainer(This,hApp,bstrContainerName,phContainer)	\
    ( (This)->lpVtbl -> CreateContainer(This,hApp,bstrContainerName,phContainer) ) 

#define ISafeCard_DeleteContainer(This,hApp,bstrContainerName)	\
    ( (This)->lpVtbl -> DeleteContainer(This,hApp,bstrContainerName) ) 

#define ISafeCard_OpenContainer(This,hApp,bstrContainerName,phContainer)	\
    ( (This)->lpVtbl -> OpenContainer(This,hApp,bstrContainerName,phContainer) ) 

#define ISafeCard_CloseContainer(This,hContainer)	\
    ( (This)->lpVtbl -> CloseContainer(This,hContainer) ) 

#define ISafeCard_EnumContainer(This,hApp,pbstrNameList)	\
    ( (This)->lpVtbl -> EnumContainer(This,hApp,pbstrNameList) ) 

#define ISafeCard_GetContainerType(This,hContainer,psType)	\
    ( (This)->lpVtbl -> GetContainerType(This,hContainer,psType) ) 

#define ISafeCard_ImportCertificate(This,hContainer,bSign,pbCert,nCertLen)	\
    ( (This)->lpVtbl -> ImportCertificate(This,hContainer,bSign,pbCert,nCertLen) ) 

#define ISafeCard_ExportCertificate(This,hContainer,bSign,pnCertLen,pbCert)	\
    ( (This)->lpVtbl -> ExportCertificate(This,hContainer,bSign,pnCertLen,pbCert) ) 

#define ISafeCard_GenRandom(This,nRandomLen,pbstrRandom)	\
    ( (This)->lpVtbl -> GenRandom(This,nRandomLen,pbstrRandom) ) 

#define ISafeCard_GenExtRSAKey(This,nBitsLen,pRsaPrivateKey)	\
    ( (This)->lpVtbl -> GenExtRSAKey(This,nBitsLen,pRsaPrivateKey) ) 

#define ISafeCard_GenRSAKeyPair(This,hContainer,nBitsLen,pRsaPubKey)	\
    ( (This)->lpVtbl -> GenRSAKeyPair(This,hContainer,nBitsLen,pRsaPubKey) ) 

#define ISafeCard_ImportRSAKeyPair(This,hContainer,nSymAlgId,pbWrappedKey,nWrappedKeyLen,pbEncryptedData,nEncryptedDataLen)	\
    ( (This)->lpVtbl -> ImportRSAKeyPair(This,hContainer,nSymAlgId,pbWrappedKey,nWrappedKeyLen,pbEncryptedData,nEncryptedDataLen) ) 

#define ISafeCard_RSASignData(This,hContainer,nDataLen,pbData,pnSignature,pbSignature)	\
    ( (This)->lpVtbl -> RSASignData(This,hContainer,nDataLen,pbData,pnSignature,pbSignature) ) 

#define ISafeCard_RSAVerify(This,pRsaPubKey,nDataLen,pbData,nSignatureLen,pbSignature)	\
    ( (This)->lpVtbl -> RSAVerify(This,pRsaPubKey,nDataLen,pbData,nSignatureLen,pbSignature) ) 

#define ISafeCard_RSAExportSessionKey(This,hContainer,nAlgId,pRsaPubKey,pnDataLen,pbData,phSessionKey)	\
    ( (This)->lpVtbl -> RSAExportSessionKey(This,hContainer,nAlgId,pRsaPubKey,pnDataLen,pbData,phSessionKey) ) 

#define ISafeCard_ExtRSAPubKeyOperation(This,pRsaPubKey,nInputLen,pbInputData,pnOutput,pbOutput)	\
    ( (This)->lpVtbl -> ExtRSAPubKeyOperation(This,pRsaPubKey,nInputLen,pbInputData,pnOutput,pbOutput) ) 

#define ISafeCard_ExtRSAPriKeyOperation(This,pRsaPubKey,nInputLen,pbInputData,pnOutput,pbOutput)	\
    ( (This)->lpVtbl -> ExtRSAPriKeyOperation(This,pRsaPubKey,nInputLen,pbInputData,pnOutput,pbOutput) ) 

#define ISafeCard_ExportPublicKey(This,hContainer,bSignFlag,pnBlob,pbBlob)	\
    ( (This)->lpVtbl -> ExportPublicKey(This,hContainer,bSignFlag,pnBlob,pbBlob) ) 

#define ISafeCard_ImportSessionKey(This,hContainer,nAlgId,nWrapedLen,pbWrapedData,phSessionKey)	\
    ( (This)->lpVtbl -> ImportSessionKey(This,hContainer,nAlgId,nWrapedLen,pbWrapedData,phSessionKey) ) 

#define ISafeCard_SetSymmKey(This,nAlgId,pbKey,phSessionKey)	\
    ( (This)->lpVtbl -> SetSymmKey(This,nAlgId,pbKey,phSessionKey) ) 

#define ISafeCard_EncryptInit(This,hKey)	\
    ( (This)->lpVtbl -> EncryptInit(This,hKey) ) 

#define ISafeCard_Encrypt(This,hKey,nDataLen,pbData,pnEncryptedDataLen,pbEncryptedData)	\
    ( (This)->lpVtbl -> Encrypt(This,hKey,nDataLen,pbData,pnEncryptedDataLen,pbEncryptedData) ) 

#define ISafeCard_EncryptUpdate(This,hKey,nDataLen,pbData,pnEncryptedDataLen,pbEncryptedData)	\
    ( (This)->lpVtbl -> EncryptUpdate(This,hKey,nDataLen,pbData,pnEncryptedDataLen,pbEncryptedData) ) 

#define ISafeCard_EncryptFinal(This,hKey,pnEncryptedDataLen,pbEncryptedData)	\
    ( (This)->lpVtbl -> EncryptFinal(This,hKey,pnEncryptedDataLen,pbEncryptedData) ) 

#define ISafeCard_DecryptInit(This,hKey)	\
    ( (This)->lpVtbl -> DecryptInit(This,hKey) ) 

#define ISafeCard_Decrypt(This,hKey,nEncryptedDataLen,pbEncryptedData,pnDataLen,pbData)	\
    ( (This)->lpVtbl -> Decrypt(This,hKey,nEncryptedDataLen,pbEncryptedData,pnDataLen,pbData) ) 

#define ISafeCard_DecryptUpdate(This,hKey,nEncryptedDataLen,pbEncryptedData,pnDataLen,pbData)	\
    ( (This)->lpVtbl -> DecryptUpdate(This,hKey,nEncryptedDataLen,pbEncryptedData,pnDataLen,pbData) ) 

#define ISafeCard_DecryptFinal(This,hKey,pnDecryptedLen,pbDecryptedData)	\
    ( (This)->lpVtbl -> DecryptFinal(This,hKey,pnDecryptedLen,pbDecryptedData) ) 

#define ISafeCard_DigestInit(This,ulAlgID,pPubKey,nIDLen,pucID,phHash)	\
    ( (This)->lpVtbl -> DigestInit(This,ulAlgID,pPubKey,nIDLen,pucID,phHash) ) 

#define ISafeCard_Digest(This,hHash,ulDataLen,pbData,pnHashLen,pbHashData)	\
    ( (This)->lpVtbl -> Digest(This,hHash,ulDataLen,pbData,pnHashLen,pbHashData) ) 

#define ISafeCard_DigestUpdate(This,hHash,ulDataLen,pbData)	\
    ( (This)->lpVtbl -> DigestUpdate(This,hHash,ulDataLen,pbData) ) 

#define ISafeCard_DigestFinal(This,hHash,pnHashLen,pbHashData)	\
    ( (This)->lpVtbl -> DigestFinal(This,hHash,pnHashLen,pbHashData) ) 

#define ISafeCard_CloseHandle(This,hHandle)	\
    ( (This)->lpVtbl -> CloseHandle(This,hHandle) ) 

#define ISafeCard_get_SKFType(This,pVal)	\
    ( (This)->lpVtbl -> get_SKFType(This,pVal) ) 

#define ISafeCard_get_LastErrCode(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrCode(This,pVal) ) 

#define ISafeCard_get_LastErrInfo(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrInfo(This,pVal) ) 

#define ISafeCard_CloseDev(This)	\
    ( (This)->lpVtbl -> CloseDev(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISafeCard_INTERFACE_DEFINED__ */


#ifndef __ICryptoStor_INTERFACE_DEFINED__
#define __ICryptoStor_INTERFACE_DEFINED__

/* interface ICryptoStor */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ICryptoStor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("84FB8D9B-252D-4971-BF84-5960B1288384")
    ICryptoStor : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumDisk( 
            /* [retval][out] */ BSTR *pbstrDisks) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PutCurDisk( 
            /* [in] */ BSTR cDisk,
            /* [retval][out] */ DWORD *dwCapcity) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileCount( 
            /* [retval][out] */ DWORD *dwFileCount) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFileInfo( 
            /* [in] */ LONG dwFileIndex,
            /* [out] */ BSTR *pbstrFileName,
            /* [out] */ DWORD *dwBegin,
            /* [retval][out] */ DWORD *dwFileSize) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadOutFile( 
            /* [in] */ DWORD dwBegin,
            /* [in] */ DWORD dwFileLength,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ BSTR bstrPathFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadOnlyFile( 
            /* [in] */ BSTR bstrFileName,
            /* [in] */ BSTR bstrPathFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCurDisk( 
            /* [retval][out] */ BSTR *pDisk) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteInFile( 
            /* [in] */ BSTR bstrPathFile,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ VARIANT_BOOL bAddFlag) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddWriteFile( 
            /* [in] */ BSTR bstrPathFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteAllFile( 
            /* [in] */ VARIANT_BOOL bBeginFirst) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDiskStatus( 
            /* [retval][out] */ SHORT *psStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DelFile( 
            /* [in] */ BSTR bstrFileName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearAllFile( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DiskClean( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseDisk( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrCode( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrInfo( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICryptoStorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICryptoStor * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICryptoStor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICryptoStor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICryptoStor * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICryptoStor * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICryptoStor * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICryptoStor * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumDisk )( 
            ICryptoStor * This,
            /* [retval][out] */ BSTR *pbstrDisks);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PutCurDisk )( 
            ICryptoStor * This,
            /* [in] */ BSTR cDisk,
            /* [retval][out] */ DWORD *dwCapcity);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileCount )( 
            ICryptoStor * This,
            /* [retval][out] */ DWORD *dwFileCount);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFileInfo )( 
            ICryptoStor * This,
            /* [in] */ LONG dwFileIndex,
            /* [out] */ BSTR *pbstrFileName,
            /* [out] */ DWORD *dwBegin,
            /* [retval][out] */ DWORD *dwFileSize);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadOutFile )( 
            ICryptoStor * This,
            /* [in] */ DWORD dwBegin,
            /* [in] */ DWORD dwFileLength,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ BSTR bstrPathFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadOnlyFile )( 
            ICryptoStor * This,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ BSTR bstrPathFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCurDisk )( 
            ICryptoStor * This,
            /* [retval][out] */ BSTR *pDisk);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteInFile )( 
            ICryptoStor * This,
            /* [in] */ BSTR bstrPathFile,
            /* [in] */ BSTR bstrFileName,
            /* [in] */ VARIANT_BOOL bAddFlag);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddWriteFile )( 
            ICryptoStor * This,
            /* [in] */ BSTR bstrPathFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteAllFile )( 
            ICryptoStor * This,
            /* [in] */ VARIANT_BOOL bBeginFirst);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDiskStatus )( 
            ICryptoStor * This,
            /* [retval][out] */ SHORT *psStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DelFile )( 
            ICryptoStor * This,
            /* [in] */ BSTR bstrFileName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearAllFile )( 
            ICryptoStor * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DiskClean )( 
            ICryptoStor * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseDisk )( 
            ICryptoStor * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrCode )( 
            ICryptoStor * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrInfo )( 
            ICryptoStor * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } ICryptoStorVtbl;

    interface ICryptoStor
    {
        CONST_VTBL struct ICryptoStorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICryptoStor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICryptoStor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICryptoStor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICryptoStor_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICryptoStor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICryptoStor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICryptoStor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ICryptoStor_EnumDisk(This,pbstrDisks)	\
    ( (This)->lpVtbl -> EnumDisk(This,pbstrDisks) ) 

#define ICryptoStor_PutCurDisk(This,cDisk,dwCapcity)	\
    ( (This)->lpVtbl -> PutCurDisk(This,cDisk,dwCapcity) ) 

#define ICryptoStor_GetFileCount(This,dwFileCount)	\
    ( (This)->lpVtbl -> GetFileCount(This,dwFileCount) ) 

#define ICryptoStor_GetFileInfo(This,dwFileIndex,pbstrFileName,dwBegin,dwFileSize)	\
    ( (This)->lpVtbl -> GetFileInfo(This,dwFileIndex,pbstrFileName,dwBegin,dwFileSize) ) 

#define ICryptoStor_ReadOutFile(This,dwBegin,dwFileLength,bstrFileName,bstrPathFile)	\
    ( (This)->lpVtbl -> ReadOutFile(This,dwBegin,dwFileLength,bstrFileName,bstrPathFile) ) 

#define ICryptoStor_ReadOnlyFile(This,bstrFileName,bstrPathFile)	\
    ( (This)->lpVtbl -> ReadOnlyFile(This,bstrFileName,bstrPathFile) ) 

#define ICryptoStor_GetCurDisk(This,pDisk)	\
    ( (This)->lpVtbl -> GetCurDisk(This,pDisk) ) 

#define ICryptoStor_WriteInFile(This,bstrPathFile,bstrFileName,bAddFlag)	\
    ( (This)->lpVtbl -> WriteInFile(This,bstrPathFile,bstrFileName,bAddFlag) ) 

#define ICryptoStor_AddWriteFile(This,bstrPathFile)	\
    ( (This)->lpVtbl -> AddWriteFile(This,bstrPathFile) ) 

#define ICryptoStor_WriteAllFile(This,bBeginFirst)	\
    ( (This)->lpVtbl -> WriteAllFile(This,bBeginFirst) ) 

#define ICryptoStor_GetDiskStatus(This,psStatus)	\
    ( (This)->lpVtbl -> GetDiskStatus(This,psStatus) ) 

#define ICryptoStor_DelFile(This,bstrFileName)	\
    ( (This)->lpVtbl -> DelFile(This,bstrFileName) ) 

#define ICryptoStor_ClearAllFile(This)	\
    ( (This)->lpVtbl -> ClearAllFile(This) ) 

#define ICryptoStor_DiskClean(This)	\
    ( (This)->lpVtbl -> DiskClean(This) ) 

#define ICryptoStor_CloseDisk(This)	\
    ( (This)->lpVtbl -> CloseDisk(This) ) 

#define ICryptoStor_get_LastErrCode(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrCode(This,pVal) ) 

#define ICryptoStor_get_LastErrInfo(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrInfo(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICryptoStor_INTERFACE_DEFINED__ */


#ifndef __IWMIDevice_INTERFACE_DEFINED__
#define __IWMIDevice_INTERFACE_DEFINED__

/* interface IWMIDevice */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWMIDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2CCC295C-FA9D-4C02-BC4D-AA24E49E0E98")
    IWMIDevice : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Connect( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Query( 
            /* [in] */ EWMIType eWMIType,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetValue( 
            /* [in] */ SHORT nIndex,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetName( 
            /* [in] */ SHORT nIndex,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetKeyValue( 
            /* [in] */ SHORT nIndex,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWMIDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWMIDevice * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWMIDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWMIDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWMIDevice * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWMIDevice * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWMIDevice * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWMIDevice * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Connect )( 
            IWMIDevice * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Query )( 
            IWMIDevice * This,
            /* [in] */ EWMIType eWMIType,
            /* [in] */ BSTR bstrKey,
            /* [retval][out] */ SHORT *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetValue )( 
            IWMIDevice * This,
            /* [in] */ SHORT nIndex,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IWMIDevice * This,
            /* [in] */ SHORT nIndex,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetKeyValue )( 
            IWMIDevice * This,
            /* [in] */ SHORT nIndex,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IWMIDevice * This);
        
        END_INTERFACE
    } IWMIDeviceVtbl;

    interface IWMIDevice
    {
        CONST_VTBL struct IWMIDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWMIDevice_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWMIDevice_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWMIDevice_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWMIDevice_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWMIDevice_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWMIDevice_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWMIDevice_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWMIDevice_Connect(This)	\
    ( (This)->lpVtbl -> Connect(This) ) 

#define IWMIDevice_Query(This,eWMIType,bstrKey,pVal)	\
    ( (This)->lpVtbl -> Query(This,eWMIType,bstrKey,pVal) ) 

#define IWMIDevice_GetValue(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> GetValue(This,nIndex,pVal) ) 

#define IWMIDevice_GetName(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> GetName(This,nIndex,pVal) ) 

#define IWMIDevice_GetKeyValue(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> GetKeyValue(This,nIndex,pVal) ) 

#define IWMIDevice_Close(This)	\
    ( (This)->lpVtbl -> Close(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWMIDevice_INTERFACE_DEFINED__ */


#ifndef __IUSBDevice_INTERFACE_DEFINED__
#define __IUSBDevice_INTERFACE_DEFINED__

/* interface IUSBDevice */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IUSBDevice;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9A5071D8-A072-464D-8158-E4229AA0C260")
    IUSBDevice : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IUSBDeviceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IUSBDevice * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IUSBDevice * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IUSBDevice * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IUSBDevice * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IUSBDevice * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IUSBDevice * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IUSBDevice * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IUSBDeviceVtbl;

    interface IUSBDevice
    {
        CONST_VTBL struct IUSBDeviceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUSBDevice_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IUSBDevice_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IUSBDevice_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IUSBDevice_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IUSBDevice_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IUSBDevice_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IUSBDevice_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUSBDevice_INTERFACE_DEFINED__ */


#ifndef __ISoftEncry_INTERFACE_DEFINED__
#define __ISoftEncry_INTERFACE_DEFINED__

/* interface ISoftEncry */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISoftEncry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("FE350B42-93B8-4C33-B533-4CE8AC283F18")
    ISoftEncry : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesEcbEnc( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesEcbDes( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesCbcEnc( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesCbcDes( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReleaseBuf( 
            /* [out][in] */ BYTE *pBuf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoftEncryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoftEncry * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoftEncry * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoftEncry * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoftEncry * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoftEncry * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoftEncry * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoftEncry * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesEcbEnc )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesEcbDes )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesCbcEnc )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesCbcDes )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReleaseBuf )( 
            ISoftEncry * This,
            /* [out][in] */ BYTE *pBuf);
        
        END_INTERFACE
    } ISoftEncryVtbl;

    interface ISoftEncry
    {
        CONST_VTBL struct ISoftEncryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoftEncry_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISoftEncry_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISoftEncry_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISoftEncry_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISoftEncry_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISoftEncry_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISoftEncry_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISoftEncry_AesEcbEnc(This,szIn,nInLen,szKey,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesEcbEnc(This,szIn,nInLen,szKey,pnOutLen,ppBuf) ) 

#define ISoftEncry_AesEcbDes(This,szIn,nInLen,szKey,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesEcbDes(This,szIn,nInLen,szKey,pnOutLen,ppBuf) ) 

#define ISoftEncry_AesCbcEnc(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesCbcEnc(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf) ) 

#define ISoftEncry_AesCbcDes(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesCbcDes(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf) ) 

#define ISoftEncry_ReleaseBuf(This,pBuf)	\
    ( (This)->lpVtbl -> ReleaseBuf(This,pBuf) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISoftEncry_INTERFACE_DEFINED__ */



#ifndef __SKFEncryLib_LIBRARY_DEFINED__
#define __SKFEncryLib_LIBRARY_DEFINED__

/* library SKFEncryLib */
/* [version][uuid] */ 

typedef 
enum EHXPacketType
    {	HXPACKETTYPE_BASE	= 1,
	HXPACKETTYPE_FILEWHITE	= 2,
	HXPACKETTYPE_USBMGR	= 4,
	HXPACKETTYPE_LOGAUDIT	= 8,
	HXPACKETTYPE_GINALOGIN	= 16,
	HXPACKETTYPE_ACCOUNTMGR	= 32,
	HXPACKETTYPE_HOSTSAFE	= 64,
	HXPACKETTYPE_SCAN	= 128,
	HXPACKETTYPE_SERVER	= 8192
    } 	EPacketType;


EXTERN_C const IID LIBID_SKFEncryLib;

#ifndef ___ISafeServiceEvents_DISPINTERFACE_DEFINED__
#define ___ISafeServiceEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISafeServiceEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ISafeServiceEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0C4E0846-C3E8-4B20-A949-6D7D7054D52F")
    _ISafeServiceEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISafeServiceEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ISafeServiceEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ISafeServiceEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ISafeServiceEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ISafeServiceEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ISafeServiceEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ISafeServiceEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ISafeServiceEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ISafeServiceEventsVtbl;

    interface _ISafeServiceEvents
    {
        CONST_VTBL struct _ISafeServiceEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISafeServiceEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ISafeServiceEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ISafeServiceEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ISafeServiceEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ISafeServiceEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ISafeServiceEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ISafeServiceEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISafeServiceEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SafeService;

#ifdef __cplusplus

class DECLSPEC_UUID("E411DCC5-4A95-4899-8578-DE15D85D2E18")
SafeService;
#endif

#ifndef ___ISafeCardEvents_DISPINTERFACE_DEFINED__
#define ___ISafeCardEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISafeCardEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ISafeCardEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2FC5E4A0-B3F1-4233-87C6-FB0A8DECEDE5")
    _ISafeCardEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISafeCardEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ISafeCardEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ISafeCardEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ISafeCardEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ISafeCardEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ISafeCardEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ISafeCardEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ISafeCardEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ISafeCardEventsVtbl;

    interface _ISafeCardEvents
    {
        CONST_VTBL struct _ISafeCardEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISafeCardEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ISafeCardEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ISafeCardEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ISafeCardEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ISafeCardEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ISafeCardEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ISafeCardEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISafeCardEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SafeCard;

#ifdef __cplusplus

class DECLSPEC_UUID("D47BAC83-6C6E-48A5-B576-E962A8446853")
SafeCard;
#endif

#ifndef ___ICryptoStorEvents_DISPINTERFACE_DEFINED__
#define ___ICryptoStorEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ICryptoStorEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ICryptoStorEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B8FD9D2C-929E-429D-BD2C-9FD3D0B36171")
    _ICryptoStorEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ICryptoStorEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ICryptoStorEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ICryptoStorEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ICryptoStorEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ICryptoStorEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ICryptoStorEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ICryptoStorEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ICryptoStorEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ICryptoStorEventsVtbl;

    interface _ICryptoStorEvents
    {
        CONST_VTBL struct _ICryptoStorEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ICryptoStorEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ICryptoStorEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ICryptoStorEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ICryptoStorEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ICryptoStorEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ICryptoStorEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ICryptoStorEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ICryptoStorEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CryptoStor;

#ifdef __cplusplus

class DECLSPEC_UUID("A12E4FF8-99B5-40A5-922C-787BA6D5F3DC")
CryptoStor;
#endif

EXTERN_C const CLSID CLSID_WMIDevice;

#ifdef __cplusplus

class DECLSPEC_UUID("CAEC6A18-537E-4B9E-9F7C-42130E4B8034")
WMIDevice;
#endif

EXTERN_C const CLSID CLSID_USBDevice;

#ifdef __cplusplus

class DECLSPEC_UUID("C4FE3A0B-6B17-4703-8911-94241A404810")
USBDevice;
#endif

EXTERN_C const CLSID CLSID_SoftEncry;

#ifdef __cplusplus

class DECLSPEC_UUID("1F3F9D6F-5A5A-4643-A319-220C6BC036F1")
SoftEncry;
#endif
#endif /* __SKFEncryLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


