

#include "ServerSocket.h"
#include <iostream>
#include <signal.h>
using namespace std;

int main()
{
    signal(SIGPIPE, SIG_IGN);
    std::shared_ptr<Socket> pServSocket(new ServerSocket());
    if(!pServSocket->Listen(5555))
    {
        cout << "listen error" << endl;
        return -1;
    }
    while(1)
    {
        int sockfd = pServSocket->Accept();
        if(sockfd < 0)
            continue;
        std::shared_ptr<Socket> pClient = pServSocket->GetSocket(sockfd);
        if(pClient == nullptr)
        {
            cout << "server error " << endl;
            continue;
        }
        //todo:创建线程处理每一个socket 连接处理
        char buf[1024];
        pClient->Recv(buf, sizeof(buf));
        cout << buf << endl;
    }
    return 0;
}


//g++ Socket.h Server.cpp ServerSocket.h  --std=c++11 -o Server