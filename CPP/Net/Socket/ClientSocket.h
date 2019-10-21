#pragma once

#include "Socket.h"
#include <iostream>
using namespace std;

class ClientSocket : public Socket{
public:
    bool Connect(const char* addr, int port);
private:
	
};

bool ClientSocket::Connect(const char* addr, int port)
{
	if(addr==nullptr || addr=="" || port < 0)
		return false;
    SetAddr(port, addr);
    if(::connect(m_sockfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
	{
		return false;
	}
	cout << "Connect ok" << endl;
	return true;
}
