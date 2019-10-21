

#include "ClientSocket.h"
#include <iostream>
using namespace std;

int main()
{
    std::shared_ptr<Socket> pCliSocket;
    pCliSocket.reset(new ClientSocket());
    bool ret = pCliSocket->Connect("127.0.0.1", 5555);
    if(!ret)
        return -1;
    char buf[1024];
    strcpy(buf, "hello world");
    pCliSocket->Send(buf, sizeof(buf));
    cout << "send ok!!!" << endl;

    return 0;
}