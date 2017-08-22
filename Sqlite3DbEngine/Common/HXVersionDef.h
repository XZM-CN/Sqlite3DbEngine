#pragma once

///////////////////////////// 版本定义 ///////////////////////////////////
/// 定义为单机版
#define	TDHXKJ_VRSION_SINGLE
/// 定义为网络版
//#define TDHXKJ_VRSION_NET

//定义青岛安控版本
//#define	QDAK_VRSION_SINGLE

/// 定义是否用USB安全U卡方案
#define TDHXKJ_VERSION_NOUSB
//#undef  TDHXKJ_VERSION_NOUSB
/// 定义适用于XP和2000系统版本
#define TDHXKJ_VERSION_XP2K 

/// 定义是否集成BugTrap调试库
#define TDHXKJ_VERSION_BUGTRAP

/// 定义主版本号
#ifdef TDHXKJ_VRSION_SINGLE

#define	APP_VERSION_MAIN			2
/// 定义次版本号
#define	APP_VERSION_MINOR			1
/// 定义编译版本号
#define	APP_VERSION_BUILD			0

#else
#define	APP_VERSION_MAIN			1
/// 定义次版本号
#define	APP_VERSION_MINOR			1
/// 定义编译版本号
#define	APP_VERSION_BUILD			1
#endif

#define	APP_BUILD_VERSION			_T("2017.3.3")

/// 定义版本保护
//#define	APP_COPYRIGHT_CODE
