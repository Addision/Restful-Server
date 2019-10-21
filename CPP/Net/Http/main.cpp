

#include "Http.h"
#include <iostream>
#include <unistd.h>
#include <time.h>
using namespace std;

int main(int argc, char const *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    Http http("http://127.0.0.1");
    http.Start();  //创建接收线程
    sleep(1);
    std::shared_ptr<Socket> pSocket = http.GetSocket();
    // cout << "======" << pSocket->GetIp() << endl;
    if(!pSocket->Connect(pSocket->GetIp().c_str(), 5555))
    {
        cout << "connnect error" << endl;
        return -1;
    }

    http_map_type book;
    //add book
    book.emplace("id", "666"); //add book id
    book.emplace("name", "book6");
    book.emplace("author", "author6");
    book.emplace("content", "rrrrrrr");

    //put book
    std::string header = http.HttpPost(book);
    pSocket->Send(header.data(), header.length()+1);
    
    //get book
    header.clear();
    header = http.HttpGet(book);
    pSocket->Send(header.data(), header.length()+1);

    while(1){
        sleep(1);
    }
    
    return 0;
}


//g++ -g -o http Http.h main.cpp Socket/Socket.h Socket/ClientSocket.h ../libs/jsoncpp/json.h ../libs/jsoncpp/jsoncpp.cpp 
//../libs/Utils/ThreadBase.h ../libs/Utils/ThreadBase.cpp --std=c++11 -pthread