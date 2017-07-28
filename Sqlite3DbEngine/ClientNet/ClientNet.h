#pragma once
#include<stdio.h>
#include<Windows.h>
#pragma comment(lib, "Ws2_32.lib")
class CClientNet
{
private:
	SOCKET m_sock;
public:
	CClientNet(void);
	//连接上指定服务器
	int Connect(int port,const char* address);
	//发送信息
	int SendMsg(const char* msg,int len);
	//关闭
	void Close();
	~CClientNet(void);
};