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
	//������ָ��������
	int Connect(int port,const char* address);
	//������Ϣ
	int SendMsg(const char* msg,int len);
	//�ر�
	void Close();
	~CClientNet(void);
};