#pragma once
#include <stdio.h>
#include <winsock.h>
#include<vector>
#include<iterator>
#include<iostream>
#include <algorithm>
#include<map>
#include<string>
using namespace std;

#pragma comment (lib,"ws2_32.lib")


class CServerNet
{
private:
	SOCKET m_sock;

public:
	CServerNet(void);
	//��ʼ��������,����0��ʾ�ɹ�
	int Init(const char* address,int port);

	//��������
	void Run();



	~CServerNet(void);

};