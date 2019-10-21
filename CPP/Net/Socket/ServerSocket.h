#pragma once

#include "Socket.h"
#include <map>
#include <iostream>
using namespace std;

class ServerSocket: public Socket{
public:
    bool Listen(int port);
    int Accept();
    std::shared_ptr<Socket> GetSocket(int sockfd);
private:
    std::map<int, std::shared_ptr<Socket>> m_mClients;
};

bool ServerSocket::Listen(int port)
{
	if(port < 0)
	{
		return false;
	}
	if(m_sockfd < 0)
	{
		return false;
	}
	SetAddr(port);
	int value = 1;
	setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&value, sizeof(value));
	if(::bind(m_sockfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
	{
		return false;
	}
	if(::listen(m_sockfd, 0) < 0)
	{
		return false;		
	}
	cout << "Listen Port:" << port << endl;
	return true;
}

int ServerSocket::Accept()
{
    std::shared_ptr<Socket> pConnSock;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int connfd = -1;
	if((connfd = ::accept(m_sockfd, (struct sockaddr*)&addr, &len)) <0)
	{
		return -1;
	}
    pConnSock.reset(new Socket());
    pConnSock->SetUp(connfd, &addr);
    m_mClients.emplace(connfd, pConnSock);
    return connfd;
}


std::shared_ptr<Socket> ServerSocket::GetSocket(int sockfd)
{
    auto it = m_mClients.find(sockfd);
    if(it != m_mClients.end())
    {
		return it->second;
    }
    return nullptr;
}