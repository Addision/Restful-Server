#pragma once

#include "Socket/ClientSocket.h"
#include "../../libs/jsoncpp/json.h"
#include "../../libs/Utils/ThreadBase.h"

#include <string>
#include <iostream>
using namespace std;

typedef std::map<std::string, std::string> http_map_type;
class Http : public ThreadBase{
public:
    Http(std::string url);
    Http();
    virtual ~Http(){}
    virtual bool Init() { return true; }
    virtual void Loop();
    std::string HttpGet(http_map_type& book);
    std::string HttpPost(http_map_type& book);
    std::string CreateHttpHead(const std::string& method, std::map<std::string, std::string>& book);
    std::shared_ptr<Socket>& GetSocket() { return m_psocket; }
    void SetUrl(std::string& url)
    {
        m_url = url;
    }
private:
    std::string m_url{""};
    std::string m_host{""};
    std::shared_ptr<Socket> m_psocket;
};

Http::Http()
{
    m_psocket = std::shared_ptr<Socket>(new ClientSocket());
}

Http::Http(std::string url):m_url(url)
{
    m_psocket = std::shared_ptr<Socket>(new ClientSocket());
    size_t pos = url.find("//");
    if(pos != string::npos)
    {
        cout << pos << endl;
    }
    url = url.substr(pos+2);
    pos = url.find_first_of("/");
    url = url.substr(0, pos);
    m_psocket->GetHostByName(url.c_str());
}

std::string Http::HttpGet(http_map_type& book)
{
    return CreateHttpHead("GET", book);
}

std::string Http::HttpPost(http_map_type& book)
{
    return CreateHttpHead("POST", book);
}

std::string Http::CreateHttpHead(const std::string& method, http_map_type& book)
{
    std::string Header = "";
    Header.append(method+ " ");
    std::string book_id = book["id"];
    cout << book_id << endl;
    Header.append("/api/v1/books/"+book_id+" ");
    Header.append("HTTP/1.1\r\n");
    Header.append("Host: ");
    Header.append("192.168.199.111:5555\r\n");
    Header.append("Accept:*/*\r\n");
    Header.append("Content-Type: application/json;charset:UTF-8\r\n");
    Header.append("Cache-Control: no-cache\r\n");
    Header.append("Connection: close\r\n");  
    if(method == "POST")
    {
        Json::Value root;
        Json::FastWriter writer;
        root["id"] = book_id;
        root["name"] = book["name"];
        root["author"] = book["author"];
        root["content"] = book["content"];
        std::string str = writer.write(root);
        Header.append("Content-Length: ");
        Header.append(std::to_string(str.length())+"\r\n\r\n");
        Header.append(str);
    }
    Header.append("\r\n\r\n");
    cout << Header << endl;
    return Header;
}

void Http::Loop()  //读取数据from web server
{
    char buf[2048];
    m_psocket->Recv(buf, sizeof(buf));
    cout << buf << endl;    
    while(1)
    {
        usleep(1000000);
    }
}