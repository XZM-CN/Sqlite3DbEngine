#pragma once

///////////////////////////// �汾���� ///////////////////////////////////
/// ����Ϊ������
#define	TDHXKJ_VRSION_SINGLE
/// ����Ϊ�����
//#define TDHXKJ_VRSION_NET

//�����ൺ���ذ汾
//#define	QDAK_VRSION_SINGLE

/// �����Ƿ���USB��ȫU������
#define TDHXKJ_VERSION_NOUSB
//#undef  TDHXKJ_VERSION_NOUSB
/// ����������XP��2000ϵͳ�汾
#define TDHXKJ_VERSION_XP2K 

/// �����Ƿ񼯳�BugTrap���Կ�
#define TDHXKJ_VERSION_BUGTRAP

/// �������汾��
#ifdef TDHXKJ_VRSION_SINGLE

#define	APP_VERSION_MAIN			2
/// ����ΰ汾��
#define	APP_VERSION_MINOR			1
/// �������汾��
#define	APP_VERSION_BUILD			0

#else
#define	APP_VERSION_MAIN			1
/// ����ΰ汾��
#define	APP_VERSION_MINOR			1
/// �������汾��
#define	APP_VERSION_BUILD			1
#endif

#define	APP_BUILD_VERSION			_T("2017.3.3")

/// ����汾����
//#define	APP_COPYRIGHT_CODE
