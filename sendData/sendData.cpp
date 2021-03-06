// sendData.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <string>
#include <WINSOCK2.H>   
#include <stdio.h>     
     
#pragma comment(lib, "ws2_32.lib")

using namespace std;

string endFlag = "disconnect";
int initFlag = true;
SOCKET sClient;

void init(wchar_t* host,int port) {
	WSADATA wsaData;
	SOCKADDR_IN server;

	WSAStartup(0x0202, &wsaData);
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET指明使用TCP/IP协议族,SOCK_STREAM, IPPROTO_TCP具体指明使用TCP协议       
	memset(&server, 0, sizeof(SOCKADDR_IN)); //先将保存地址的server置为全0      
	server.sin_family = PF_INET;
	server.sin_port = htons(port);
	int i = 0;
	string ht;
	while (host[i] != '\0') {
		ht.push_back(host[i++]);
	}
	ht.push_back('\0');
	server.sin_addr.s_addr = inet_addr(ht.data());
	connect(sClient, (struct sockaddr *) &server, sizeof(SOCKADDR_IN)); //连接后可以用sClient来使用这个连接,server保存了远程服务器的地址信息      
}

void disconnect() {
	closesocket(sClient);
	WSACleanup();
}

__declspec(dllexport) int _stdcall sendData(wchar_t* host, int port,wchar_t* szMessage ,int n)
{
	if (initFlag) {
		init(host,port);
		initFlag = false;
	}

	string l = "";
	for (int i = 0; i<n; i++) {
		l.push_back(szMessage[i]);
	}

	if (l.compare(endFlag) == 0) {
		disconnect();
		return 0;
	}
	send(sClient, l.data(), l.length(), 0);
	return 0;
}

