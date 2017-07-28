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
	//初始化服务器,返回0表示成功
	int Init(const char* address,int port);

	//更新数据
	void Run();



	~CServerNet(void);

};