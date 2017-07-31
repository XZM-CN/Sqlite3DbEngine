//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/06/07  
// ><创建时间 ><  2017年:06月:07日   16时:16分:32秒  
// ><文件     ><  TokenInfo.cpp  
// ><文件路径 ><  C:\迅雷下载\MySecurityTest\MySecurityTest  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin
// ><         ><  2.  https://msdn.microsoft.com/zh-cn/library/f2c0w594(VS.80).aspx
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:06月:07日   16时:16分:32秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "StatisticsInfo.h"
#include "GetTokenInfo.h"

#include "Base.h"

using namespace std;


CStatisticsInfo::CStatisticsInfo(PTOKEN_STATISTICS p)
	:m_pStatistics(p)
{

}

void CStatisticsInfo::Print(std::wostream& os) const
{
	os << L"TokenId: " << m_pStatistics->TokenId << endl;
	os << L"AuthenticationId: " << m_pStatistics->AuthenticationId << endl;
	//os << L"ExpirationTime: " << m_pStatistics->ExpirationTime << endl;
	os << L"TokenType: " << (m_pStatistics->TokenType == 1 ? L"TokenPrimary" : L"TokenImpersonation") << endl;
	os << L"ImpersonationLevel: " << m_pStatistics->ImpersonationLevel << endl;
	os << L"DynamicCharged: " << m_pStatistics->DynamicCharged << endl;
	os << L"DynamicAvailable: " << m_pStatistics->DynamicAvailable << endl;
	os << L"GroupCount: " << m_pStatistics->GroupCount << endl;
	os << L"PrivilegeCount: " << m_pStatistics->PrivilegeCount << endl;
	os << L"ModifiedId: " << m_pStatistics->ModifiedId << endl;
}