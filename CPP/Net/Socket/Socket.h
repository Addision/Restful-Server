#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#include <string>
#include <iostream>
#include <memory>

class Socket{
public:
    Socket();
    ~Socket();
    virtual bool Send(const char* buf, int len);
    virtual bool Recv(char *buf, int len);
	virtual bool SetAddr(int port, const char* addr = nullptr);
    std::string GetIp() { return m_ip; }
    int GetSock() { return m_sockfd; }
    void Close();
    bool SetNonBlock();
    bool GetHostByName(const char* ptr);

    //server socket
    virtual bool Listen(int port) { return false; }
    virtual int Accept() { return -1; }
    void SetUp(int sockfd, struct sockaddr_in* addr);
    virtual std::shared_ptr<Socket> GetSocket(int sockfd) { 
        return nullptr; 
    }

    //client
    virtual bool Connect(const char *addr, int port) { return false; }

    std::string& GetRecvBuf() { return m_recvBuf; }
    std::string& GetSendBuf() { return m_sendBuf; }
protected:
    int m_sockfd;
    char m_ip[32];
    struct sockaddr_in sockAddr;

    std::string m_recvBuf;   //信息缓存
    std::string m_sendBuf;   //信息缓存
};

Socket::Socket()
{
	m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(m_sockfd <= 0)
	{
        std::cout << "create socket error" << std::endl;
    }
	memset(m_ip, 0, sizeof(m_ip));    
}

Socket::~Socket()
{
    Close();
}

void Socket::Close()
{
    ::close(m_sockfd);
    m_sockfd = -1;
}

bool Socket::SetNonBlock()
{
    bool res = false;
    if(m_sockfd)
	{
		int oldfd = ::fcntl(m_sockfd, F_GETFL);
		res = ::fcntl(m_sockfd, F_SETFL, oldfd | O_NONBLOCK) ==0 ? true : false;
    }
    return res;
}

bool Socket::SetAddr(int port, const char* addr)
{
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);
	if(addr==nullptr || strlen(addr)==0)
	{
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		return true;
	}
	else
	{
		unsigned long inaddr;
		struct hostent* hp;
		inaddr = inet_addr(addr);
		if(inaddr != INADDR_NONE)
		{
			memcpy(&sockAddr.sin_addr, &inaddr, sizeof(inaddr));
		}
		else
		{
			hp = gethostbyname(addr);
			if(hp == nullptr)
			{
				return false;
			}
			memcpy(&sockAddr.sin_addr, hp->h_addr, hp->h_length);
		}
	}
	return true;
}

void Socket::SetUp(int sockfd, struct sockaddr_in* addr)
{
    m_sockfd = sockfd;
    memcpy(&sockAddr, addr, sizeof(sockAddr));  
}

bool Socket::GetHostByName(const char* ptr)
{
    char    **pptr;
    struct  hostent *hptr;
    char    ipstr[32];
    if(ptr == nullptr)
        return false;

    if((hptr = gethostbyname(ptr)) == nullptr)
    {
        return false;
    }
    std::cout << hptr->h_name << std::endl;
    for(pptr = hptr->h_aliases; *pptr != nullptr; pptr++)
        std::cout << *pptr << std::endl;

    switch(hptr->h_addrtype)
    {
        case AF_INET:
        case AF_INET6:
            pptr=hptr->h_addr_list;
            for(; *pptr!=nullptr; pptr++)
            {
                inet_ntop(hptr->h_addrtype, *pptr, ipstr, sizeof(ipstr));
                if(ipstr != nullptr && strlen(ipstr) > 0)
                {
                    memcpy(m_ip, ipstr, sizeof(ipstr));
                    return true;
                }
            }  
        break;
        default:
            std::cout << "unknown address type" << std::endl;
        break;
    }
    return false;
}

bool Socket::Send(const char* buf, int len)
{
	int ret = 0;
    int left_len = len;
    const char *ptr = buf;
    int send_len = 0;
    do
	{
        ret = ::send(m_sockfd, ptr+send_len, left_len, 0);
        if(ret > 0)
        {
            left_len -= ret;
            send_len += ret;
        }
	} while (ret < 0 && (errno == EAGAIN || errno == EINTR));
    return send_len == len;     
}

bool Socket::Recv(char *buf, int len)
{
	int ret = 0;
    char *ptr = buf;
    int recv_len = 0;
    int left_len = len;
    do
	{
		ret = ::recv(m_sockfd, ptr+recv_len, left_len, 0);
        if(ret > 0)
        {
            recv_len += ret;
            left_len -= recv_len;
        }
        // std::cout << "recv buf" << buf <<std::endl;
	} while (ret < 0 && (errno == EAGAIN || errno == EINTR));
    return recv_len > 0; 
}

