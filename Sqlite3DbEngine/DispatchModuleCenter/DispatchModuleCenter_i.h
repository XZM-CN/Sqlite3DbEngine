

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Jul 31 14:05:53 2017
 */
/* Compiler settings for DispatchModuleCenter.idl:
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

#ifndef __DispatchModuleCenter_i_h__
#define __DispatchModuleCenter_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAccountMgr_FWD_DEFINED__
#define __IAccountMgr_FWD_DEFINED__
typedef interface IAccountMgr IAccountMgr;
#endif 	/* __IAccountMgr_FWD_DEFINED__ */


#ifndef __IServiceControl_FWD_DEFINED__
#define __IServiceControl_FWD_DEFINED__
typedef interface IServiceControl IServiceControl;
#endif 	/* __IServiceControl_FWD_DEFINED__ */


#ifndef __AccountMgr_FWD_DEFINED__
#define __AccountMgr_FWD_DEFINED__

#ifdef __cplusplus
typedef class AccountMgr AccountMgr;
#else
typedef struct AccountMgr AccountMgr;
#endif /* __cplusplus */

#endif 	/* __AccountMgr_FWD_DEFINED__ */


#ifndef ___IServiceControlEvents_FWD_DEFINED__
#define ___IServiceControlEvents_FWD_DEFINED__
typedef interface _IServiceControlEvents _IServiceControlEvents;
#endif 	/* ___IServiceControlEvents_FWD_DEFINED__ */


#ifndef __ServiceControl_FWD_DEFINED__
#define __ServiceControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class ServiceControl ServiceControl;
#else
typedef struct ServiceControl ServiceControl;
#endif /* __cplusplus */

#endif 	/* __ServiceControl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DispatchModuleCenter_0000_0000 */
/* [local] */ 

typedef 
enum EHXAccountType
    {	ACCOUNTTYPE_UNKNOWN	= 0,
	ACCOUNTTYPE_SA	= 1,
	ACCOUNTTYPE_ADMIN	= 2,
	ACCOUNTTYPE_AUDIT	= 4,
	ACCOUNTTYPE_USER	= 8
    } 	EAccountType;

typedef 
enum EHXRunMode
    {	RUNMODE_PERSONAL	= 1,
	RUNMODE_NET	= 2,
	RUNMODE_SERVICE	= 256
    } 	ERunMode;

typedef 
enum EHXUseAuthType
    {	USEAUTHTYPE_UNKNOWN	= 0,
	USEAUTHTYPE_UNLOAD	= 1,
	USEAUTHTYPE_SELFSAFE	= 2,
	USEAUTHTYPE_SAFEGUARD	= 4,
	USEAUTHTYPE_CONTROLMODE	= 8,
	USEAUTHTYPE_DATABACKUP	= 16,
	USEAUTHTYPE_ACCOUNTMGR	= 32,
	USEAUTHTYPE_FILEWHITEMGR	= 64,
	USEAUTHTYPE_USBMGR	= 128,
	USEAUTHTYPE_LOGMGR	= 256,
	USEAUTHTYPE_SAFEDISKMGR	= 512,
	USEAUTHTYPE_OSLOGIN	= 4096,
	USEAUTHTYPE_AUTHMGR	= 8192
    } 	EUseAuthType;



extern RPC_IF_HANDLE __MIDL_itf_DispatchModuleCenter_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DispatchModuleCenter_0000_0000_v0_0_s_ifspec;

#ifndef __IAccountMgr_INTERFACE_DEFINED__
#define __IAccountMgr_INTERFACE_DEFINED__

/* interface IAccountMgr */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IAccountMgr;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EC773FBA-BBDB-4275-81F3-3079D1131E4E")
    IAccountMgr : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RunMode( 
            /* [retval][out] */ ERunMode *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RunMode( 
            /* [in] */ ERunMode newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AccountType( 
            /* [retval][out] */ EAccountType *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CurLID( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Right( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowName( 
            BSTR bstrLID,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CanLogin( 
            BSTR bstrLID,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ReadOnly( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AuthValid( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ ULONG nAccount,
            /* [in] */ BSTR bstrPW,
            /* [in] */ VARIANT_BOOL bCheckFile) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetCurrent( 
            /* [in] */ EAccountType eType,
            /* [in] */ BSTR bstrID,
            /* [in] */ BSTR bstrPWHash,
            /* [in] */ EUseAuthType eAuthType,
            /* [in] */ BSTR bstrCurName,
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ EAccountType eType,
            /* [in] */ BSTR bstrID,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPWHash,
            /* [in] */ ULONG nRight) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Del( 
            /* [in] */ BSTR bstrID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangePW( 
            /* [in] */ EAccountType eType,
            /* [in] */ BSTR bstrID,
            /* [in] */ BSTR bstrNewPWHash,
            /* [in] */ BSTR bstrAccountName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeRight( 
            /* [in] */ BSTR bstrID,
            /* [in] */ ULONG nRight) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ChangeName( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [retval][out] */ IDispatch **ppInfo) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ServiceToken( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDataBase( 
            /* [in] */ BSTR bstrToken,
            /* [retval][out] */ IDispatch **ppVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ClearStatus( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnLock( 
            /* [in] */ BSTR bstrID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveConfig( 
            /* [in] */ EUseAuthType nAuthType,
            /* [in] */ BSTR bstrNodeName,
            /* [in] */ BSTR bstrNodeValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddUsbWhite( 
            /* [in] */ BSTR bstrUsbID,
            /* [in] */ BSTR bstrDiskName,
            /* [in] */ BSTR bstrMakerName,
            /* [in] */ ULONG nSize,
            /* [in] */ SHORT nType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DelUsbWhite( 
            /* [in] */ BSTR bstrUsbID,
            /* [in] */ SHORT nType) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RefreshAuth( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveToCard( 
            /* [in] */ BSTR ucDiskName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddStopService( 
            /* [in] */ BSTR bstrServiceName,
            /* [in] */ SHORT nStart,
            /* [in] */ BSTR bstrImagePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DelStopService( 
            /* [in] */ BSTR bstrServiceName,
            /* [in] */ BSTR bstrImagePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAuthCode( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateAuth( 
            /* [in] */ BSTR bstrDeviceID,
            /* [in] */ BSTR bstrFilePath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportAuth( 
            /* [in] */ BSTR bstrFilePath,
            /* [in] */ BYTE *pbKey,
            /* [retval][out] */ IDispatch **ppAuthInfo) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemoveAuth( 
            /* [in] */ BSTR bstrDeviceID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAuthInfo( 
            /* [in] */ BSTR bstrFilePath,
            /* [in] */ BSTR bstrDiskName,
            /* [in] */ BYTE *pbKey,
            /* [retval][out] */ IDispatch **ppAuthInfo) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrorInfo( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckAccountExist( 
            /* [in] */ BSTR strAccountName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRealAccountInfo( 
            /* [in] */ BSTR LID,
            /* [out] */ BSTR *strAccountTypeName,
            /* [out] */ BSTR *strRealPasswd,
            /* [retval][out] */ ULONG *strRight) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveAccountInfo( 
            /* [in] */ BSTR strAccountName,
            /* [in] */ BSTR strPassword,
            /* [in] */ BSTR LID,
            /* [in] */ BSTR strAccountTypeName,
            /* [in] */ BSTR strRealPasswd,
            /* [in] */ ULONG strRight) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DelelteAccountInfo( 
            /* [in] */ BSTR strAccountName,
            /* [in] */ BSTR LID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckIsPassswdRight( 
            /* [in] */ BSTR LID,
            /* [in] */ BSTR strAccountName,
            /* [in] */ BSTR bstrMD5,
            /* [retval][out] */ BSTR *strRealPasswd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get_CurName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAccountMgrVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAccountMgr * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAccountMgr * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAccountMgr * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAccountMgr * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAccountMgr * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAccountMgr * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAccountMgr * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RunMode )( 
            IAccountMgr * This,
            /* [retval][out] */ ERunMode *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RunMode )( 
            IAccountMgr * This,
            /* [in] */ ERunMode newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AccountType )( 
            IAccountMgr * This,
            /* [retval][out] */ EAccountType *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CurLID )( 
            IAccountMgr * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Right )( 
            IAccountMgr * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IAccountMgr * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowName )( 
            IAccountMgr * This,
            BSTR bstrLID,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CanLogin )( 
            IAccountMgr * This,
            BSTR bstrLID,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ReadOnly )( 
            IAccountMgr * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AuthValid )( 
            IAccountMgr * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            IAccountMgr * This,
            /* [in] */ ULONG nAccount,
            /* [in] */ BSTR bstrPW,
            /* [in] */ VARIANT_BOOL bCheckFile);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetCurrent )( 
            IAccountMgr * This,
            /* [in] */ EAccountType eType,
            /* [in] */ BSTR bstrID,
            /* [in] */ BSTR bstrPWHash,
            /* [in] */ EUseAuthType eAuthType,
            /* [in] */ BSTR bstrCurName,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IAccountMgr * This,
            /* [in] */ EAccountType eType,
            /* [in] */ BSTR bstrID,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrPWHash,
            /* [in] */ ULONG nRight);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Del )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangePW )( 
            IAccountMgr * This,
            /* [in] */ EAccountType eType,
            /* [in] */ BSTR bstrID,
            /* [in] */ BSTR bstrNewPWHash,
            /* [in] */ BSTR bstrAccountName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeRight )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrID,
            /* [in] */ ULONG nRight);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ChangeName )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Login )( 
            IAccountMgr * This,
            /* [retval][out] */ IDispatch **ppInfo);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ServiceToken )( 
            IAccountMgr * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataBase )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrToken,
            /* [retval][out] */ IDispatch **ppVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ClearStatus )( 
            IAccountMgr * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnLock )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveConfig )( 
            IAccountMgr * This,
            /* [in] */ EUseAuthType nAuthType,
            /* [in] */ BSTR bstrNodeName,
            /* [in] */ BSTR bstrNodeValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddUsbWhite )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrUsbID,
            /* [in] */ BSTR bstrDiskName,
            /* [in] */ BSTR bstrMakerName,
            /* [in] */ ULONG nSize,
            /* [in] */ SHORT nType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DelUsbWhite )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrUsbID,
            /* [in] */ SHORT nType);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RefreshAuth )( 
            IAccountMgr * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveToCard )( 
            IAccountMgr * This,
            /* [in] */ BSTR ucDiskName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddStopService )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrServiceName,
            /* [in] */ SHORT nStart,
            /* [in] */ BSTR bstrImagePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DelStopService )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrServiceName,
            /* [in] */ BSTR bstrImagePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAuthCode )( 
            IAccountMgr * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateAuth )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrDeviceID,
            /* [in] */ BSTR bstrFilePath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportAuth )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrFilePath,
            /* [in] */ BYTE *pbKey,
            /* [retval][out] */ IDispatch **ppAuthInfo);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemoveAuth )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrDeviceID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAuthInfo )( 
            IAccountMgr * This,
            /* [in] */ BSTR bstrFilePath,
            /* [in] */ BSTR bstrDiskName,
            /* [in] */ BYTE *pbKey,
            /* [retval][out] */ IDispatch **ppAuthInfo);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrorInfo )( 
            IAccountMgr * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CheckAccountExist )( 
            IAccountMgr * This,
            /* [in] */ BSTR strAccountName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRealAccountInfo )( 
            IAccountMgr * This,
            /* [in] */ BSTR LID,
            /* [out] */ BSTR *strAccountTypeName,
            /* [out] */ BSTR *strRealPasswd,
            /* [retval][out] */ ULONG *strRight);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveAccountInfo )( 
            IAccountMgr * This,
            /* [in] */ BSTR strAccountName,
            /* [in] */ BSTR strPassword,
            /* [in] */ BSTR LID,
            /* [in] */ BSTR strAccountTypeName,
            /* [in] */ BSTR strRealPasswd,
            /* [in] */ ULONG strRight);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DelelteAccountInfo )( 
            IAccountMgr * This,
            /* [in] */ BSTR strAccountName,
            /* [in] */ BSTR LID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CheckIsPassswdRight )( 
            IAccountMgr * This,
            /* [in] */ BSTR LID,
            /* [in] */ BSTR strAccountName,
            /* [in] */ BSTR bstrMD5,
            /* [retval][out] */ BSTR *strRealPasswd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *get_CurName )( 
            IAccountMgr * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IAccountMgrVtbl;

    interface IAccountMgr
    {
        CONST_VTBL struct IAccountMgrVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAccountMgr_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAccountMgr_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAccountMgr_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAccountMgr_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAccountMgr_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAccountMgr_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAccountMgr_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IAccountMgr_get_RunMode(This,pVal)	\
    ( (This)->lpVtbl -> get_RunMode(This,pVal) ) 

#define IAccountMgr_put_RunMode(This,newVal)	\
    ( (This)->lpVtbl -> put_RunMode(This,newVal) ) 

#define IAccountMgr_get_AccountType(This,pVal)	\
    ( (This)->lpVtbl -> get_AccountType(This,pVal) ) 

#define IAccountMgr_get_CurLID(This,pVal)	\
    ( (This)->lpVtbl -> get_CurLID(This,pVal) ) 

#define IAccountMgr_get_Right(This,pVal)	\
    ( (This)->lpVtbl -> get_Right(This,pVal) ) 

#define IAccountMgr_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IAccountMgr_get_ShowName(This,bstrLID,pVal)	\
    ( (This)->lpVtbl -> get_ShowName(This,bstrLID,pVal) ) 

#define IAccountMgr_get_CanLogin(This,bstrLID,pVal)	\
    ( (This)->lpVtbl -> get_CanLogin(This,bstrLID,pVal) ) 

#define IAccountMgr_get_ReadOnly(This,pVal)	\
    ( (This)->lpVtbl -> get_ReadOnly(This,pVal) ) 

#define IAccountMgr_get_AuthValid(This,pVal)	\
    ( (This)->lpVtbl -> get_AuthValid(This,pVal) ) 

#define IAccountMgr_Init(This,nAccount,bstrPW,bCheckFile)	\
    ( (This)->lpVtbl -> Init(This,nAccount,bstrPW,bCheckFile) ) 

#define IAccountMgr_SetCurrent(This,eType,bstrID,bstrPWHash,eAuthType,bstrCurName,pVal)	\
    ( (This)->lpVtbl -> SetCurrent(This,eType,bstrID,bstrPWHash,eAuthType,bstrCurName,pVal) ) 

#define IAccountMgr_Add(This,eType,bstrID,bstrName,bstrPWHash,nRight)	\
    ( (This)->lpVtbl -> Add(This,eType,bstrID,bstrName,bstrPWHash,nRight) ) 

#define IAccountMgr_Del(This,bstrID)	\
    ( (This)->lpVtbl -> Del(This,bstrID) ) 

#define IAccountMgr_ChangePW(This,eType,bstrID,bstrNewPWHash,bstrAccountName)	\
    ( (This)->lpVtbl -> ChangePW(This,eType,bstrID,bstrNewPWHash,bstrAccountName) ) 

#define IAccountMgr_ChangeRight(This,bstrID,nRight)	\
    ( (This)->lpVtbl -> ChangeRight(This,bstrID,nRight) ) 

#define IAccountMgr_ChangeName(This,bstrName)	\
    ( (This)->lpVtbl -> ChangeName(This,bstrName) ) 

#define IAccountMgr_Login(This,ppInfo)	\
    ( (This)->lpVtbl -> Login(This,ppInfo) ) 

#define IAccountMgr_ServiceToken(This)	\
    ( (This)->lpVtbl -> ServiceToken(This) ) 

#define IAccountMgr_GetDataBase(This,bstrToken,ppVal)	\
    ( (This)->lpVtbl -> GetDataBase(This,bstrToken,ppVal) ) 

#define IAccountMgr_ClearStatus(This)	\
    ( (This)->lpVtbl -> ClearStatus(This) ) 

#define IAccountMgr_UnLock(This,bstrID)	\
    ( (This)->lpVtbl -> UnLock(This,bstrID) ) 

#define IAccountMgr_SaveConfig(This,nAuthType,bstrNodeName,bstrNodeValue)	\
    ( (This)->lpVtbl -> SaveConfig(This,nAuthType,bstrNodeName,bstrNodeValue) ) 

#define IAccountMgr_AddUsbWhite(This,bstrUsbID,bstrDiskName,bstrMakerName,nSize,nType)	\
    ( (This)->lpVtbl -> AddUsbWhite(This,bstrUsbID,bstrDiskName,bstrMakerName,nSize,nType) ) 

#define IAccountMgr_DelUsbWhite(This,bstrUsbID,nType)	\
    ( (This)->lpVtbl -> DelUsbWhite(This,bstrUsbID,nType) ) 

#define IAccountMgr_RefreshAuth(This,pVal)	\
    ( (This)->lpVtbl -> RefreshAuth(This,pVal) ) 

#define IAccountMgr_SaveToCard(This,ucDiskName)	\
    ( (This)->lpVtbl -> SaveToCard(This,ucDiskName) ) 

#define IAccountMgr_AddStopService(This,bstrServiceName,nStart,bstrImagePath)	\
    ( (This)->lpVtbl -> AddStopService(This,bstrServiceName,nStart,bstrImagePath) ) 

#define IAccountMgr_DelStopService(This,bstrServiceName,bstrImagePath)	\
    ( (This)->lpVtbl -> DelStopService(This,bstrServiceName,bstrImagePath) ) 

#define IAccountMgr_GetAuthCode(This,pVal)	\
    ( (This)->lpVtbl -> GetAuthCode(This,pVal) ) 

#define IAccountMgr_CreateAuth(This,bstrDeviceID,bstrFilePath)	\
    ( (This)->lpVtbl -> CreateAuth(This,bstrDeviceID,bstrFilePath) ) 

#define IAccountMgr_ImportAuth(This,bstrFilePath,pbKey,ppAuthInfo)	\
    ( (This)->lpVtbl -> ImportAuth(This,bstrFilePath,pbKey,ppAuthInfo) ) 

#define IAccountMgr_RemoveAuth(This,bstrDeviceID)	\
    ( (This)->lpVtbl -> RemoveAuth(This,bstrDeviceID) ) 

#define IAccountMgr_GetAuthInfo(This,bstrFilePath,bstrDiskName,pbKey,ppAuthInfo)	\
    ( (This)->lpVtbl -> GetAuthInfo(This,bstrFilePath,bstrDiskName,pbKey,ppAuthInfo) ) 

#define IAccountMgr_get_LastErrorInfo(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrorInfo(This,pVal) ) 

#define IAccountMgr_CheckAccountExist(This,strAccountName)	\
    ( (This)->lpVtbl -> CheckAccountExist(This,strAccountName) ) 

#define IAccountMgr_GetRealAccountInfo(This,LID,strAccountTypeName,strRealPasswd,strRight)	\
    ( (This)->lpVtbl -> GetRealAccountInfo(This,LID,strAccountTypeName,strRealPasswd,strRight) ) 

#define IAccountMgr_SaveAccountInfo(This,strAccountName,strPassword,LID,strAccountTypeName,strRealPasswd,strRight)	\
    ( (This)->lpVtbl -> SaveAccountInfo(This,strAccountName,strPassword,LID,strAccountTypeName,strRealPasswd,strRight) ) 

#define IAccountMgr_DelelteAccountInfo(This,strAccountName,LID)	\
    ( (This)->lpVtbl -> DelelteAccountInfo(This,strAccountName,LID) ) 

#define IAccountMgr_CheckIsPassswdRight(This,LID,strAccountName,bstrMD5,strRealPasswd)	\
    ( (This)->lpVtbl -> CheckIsPassswdRight(This,LID,strAccountName,bstrMD5,strRealPasswd) ) 

#define IAccountMgr_get_CurName(This,pVal)	\
    ( (This)->lpVtbl -> get_CurName(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAccountMgr_INTERFACE_DEFINED__ */


#ifndef __IServiceControl_INTERFACE_DEFINED__
#define __IServiceControl_INTERFACE_DEFINED__

/* interface IServiceControl */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IServiceControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CA8F3B6D-934F-4A2B-8C3C-83103924B4D5")
    IServiceControl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PutServiceName( 
            /* [in] */ VARIANT_BOOL bDriver,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetServiceName( 
            /* [out] */ VARIANT_BOOL *pbDriver,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SafeBoot( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE PutAccountMgr( 
            /* [in] */ IDispatch *piAccountMgr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Del( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetServiceStatus( 
            /* [retval][out] */ SHORT *pnStatus) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsExist( 
            VARIANT_BOOL *pbFlag) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrorInfo( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrorCode( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE OpenDriver( 
            /* [in] */ BSTR bstrDriverName,
            /* [in] */ VARIANT_BOOL bOverlapped) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendControlToDevice( 
            /* [in] */ ULONG unCmd,
            /* [in] */ BYTE *pData,
            USHORT uDataLen) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendUsbCmdToDriver( 
            /* [in] */ USHORT usCmd) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CloseDriver( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StartDriverMonitor( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StopDriverMonitor( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IServiceControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IServiceControl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IServiceControl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IServiceControl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IServiceControl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IServiceControl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IServiceControl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IServiceControl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PutServiceName )( 
            IServiceControl * This,
            /* [in] */ VARIANT_BOOL bDriver,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetServiceName )( 
            IServiceControl * This,
            /* [out] */ VARIANT_BOOL *pbDriver,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SafeBoot )( 
            IServiceControl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *PutAccountMgr )( 
            IServiceControl * This,
            /* [in] */ IDispatch *piAccountMgr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IServiceControl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Del )( 
            IServiceControl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Start )( 
            IServiceControl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IServiceControl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetServiceStatus )( 
            IServiceControl * This,
            /* [retval][out] */ SHORT *pnStatus);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsExist )( 
            IServiceControl * This,
            VARIANT_BOOL *pbFlag);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrorInfo )( 
            IServiceControl * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrorCode )( 
            IServiceControl * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *OpenDriver )( 
            IServiceControl * This,
            /* [in] */ BSTR bstrDriverName,
            /* [in] */ VARIANT_BOOL bOverlapped);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendControlToDevice )( 
            IServiceControl * This,
            /* [in] */ ULONG unCmd,
            /* [in] */ BYTE *pData,
            USHORT uDataLen);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendUsbCmdToDriver )( 
            IServiceControl * This,
            /* [in] */ USHORT usCmd);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CloseDriver )( 
            IServiceControl * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StartDriverMonitor )( 
            IServiceControl * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StopDriverMonitor )( 
            IServiceControl * This);
        
        END_INTERFACE
    } IServiceControlVtbl;

    interface IServiceControl
    {
        CONST_VTBL struct IServiceControlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IServiceControl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IServiceControl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IServiceControl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IServiceControl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IServiceControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IServiceControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IServiceControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IServiceControl_PutServiceName(This,bDriver,newVal)	\
    ( (This)->lpVtbl -> PutServiceName(This,bDriver,newVal) ) 

#define IServiceControl_GetServiceName(This,pbDriver,pVal)	\
    ( (This)->lpVtbl -> GetServiceName(This,pbDriver,pVal) ) 

#define IServiceControl_put_SafeBoot(This,newVal)	\
    ( (This)->lpVtbl -> put_SafeBoot(This,newVal) ) 

#define IServiceControl_PutAccountMgr(This,piAccountMgr)	\
    ( (This)->lpVtbl -> PutAccountMgr(This,piAccountMgr) ) 

#define IServiceControl_Add(This)	\
    ( (This)->lpVtbl -> Add(This) ) 

#define IServiceControl_Del(This)	\
    ( (This)->lpVtbl -> Del(This) ) 

#define IServiceControl_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define IServiceControl_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IServiceControl_GetServiceStatus(This,pnStatus)	\
    ( (This)->lpVtbl -> GetServiceStatus(This,pnStatus) ) 

#define IServiceControl_IsExist(This,pbFlag)	\
    ( (This)->lpVtbl -> IsExist(This,pbFlag) ) 

#define IServiceControl_get_LastErrorInfo(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrorInfo(This,pVal) ) 

#define IServiceControl_get_LastErrorCode(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrorCode(This,pVal) ) 

#define IServiceControl_OpenDriver(This,bstrDriverName,bOverlapped)	\
    ( (This)->lpVtbl -> OpenDriver(This,bstrDriverName,bOverlapped) ) 

#define IServiceControl_SendControlToDevice(This,unCmd,pData,uDataLen)	\
    ( (This)->lpVtbl -> SendControlToDevice(This,unCmd,pData,uDataLen) ) 

#define IServiceControl_SendUsbCmdToDriver(This,usCmd)	\
    ( (This)->lpVtbl -> SendUsbCmdToDriver(This,usCmd) ) 

#define IServiceControl_CloseDriver(This)	\
    ( (This)->lpVtbl -> CloseDriver(This) ) 

#define IServiceControl_StartDriverMonitor(This)	\
    ( (This)->lpVtbl -> StartDriverMonitor(This) ) 

#define IServiceControl_StopDriverMonitor(This)	\
    ( (This)->lpVtbl -> StopDriverMonitor(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IServiceControl_INTERFACE_DEFINED__ */



#ifndef __DispatchModuleCenterLib_LIBRARY_DEFINED__
#define __DispatchModuleCenterLib_LIBRARY_DEFINED__

/* library DispatchModuleCenterLib */
/* [version][uuid] */ 

typedef 
enum EHXConfigType
    {	CONFIGTYPE_UNKNOWN	= 0,
	CONFIGTYPE_FILEWHITE	= 1,
	CONFIGTYPE_EXTFLAG	= 2,
	CONFIGTYPE_RUNCONTROL	= 4,
	CONFIGTYPE_INSTALLORUPDATE	= 8,
	CONFIGTYPE_PROTECTEXE	= 16,
	CONFIGTYPE_DIRDATA	= 32,
	CONFIGTYPE_REGDATA	= 64,
	CONFIGTYPE_WHITEUSB	= 128,
	CONFIGTYPE_USBCONTROL	= 256,
	CONFIGTYPE_SAFEGUARD	= 512,
	CONFIGTYPE_GINALOGIN	= 1024,
	CONFIGTYPE_SCANINGFILE	= 4096,
	CONFIGTYPE_SELFPROTECT	= 8192
    } 	EConfigType;

typedef 
enum EHXFileType
    {	FILETYPE_UNKNOWN	= 0,
	FILETYPE_EXE	= 1,
	FILETYPE_BAT	= 2,
	FILETYPE_JS	= 4,
	FILETYPE_JAR	= 8,
	FILETYPE_COM	= 16,
	FILETYPE_MSI	= 32,
	FILETYPE_DLLOCX	= 64,
	FILETYPE_CABINF	= 128,
	FILETYPE_SYS	= 256,
	FILETYPE_OTHER	= 8192
    } 	EFileType;


EXTERN_C const IID LIBID_DispatchModuleCenterLib;

EXTERN_C const CLSID CLSID_AccountMgr;

#ifdef __cplusplus

class DECLSPEC_UUID("0CD272BF-4093-4E2C-A371-893A692F9BF3")
AccountMgr;
#endif

#ifndef ___IServiceControlEvents_DISPINTERFACE_DEFINED__
#define ___IServiceControlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IServiceControlEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IServiceControlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("38043D37-5799-41A0-88D3-943032A0B7A7")
    _IServiceControlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IServiceControlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IServiceControlEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IServiceControlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IServiceControlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IServiceControlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IServiceControlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IServiceControlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IServiceControlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IServiceControlEventsVtbl;

    interface _IServiceControlEvents
    {
        CONST_VTBL struct _IServiceControlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IServiceControlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IServiceControlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IServiceControlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IServiceControlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IServiceControlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IServiceControlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IServiceControlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IServiceControlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ServiceControl;

#ifdef __cplusplus

class DECLSPEC_UUID("6AE9B1D1-E869-476C-928E-C17AFFE490F9")
ServiceControl;
#endif
#endif /* __DispatchModuleCenterLib_LIBRARY_DEFINED__ */

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


