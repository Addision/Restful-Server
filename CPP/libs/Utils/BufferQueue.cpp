#pragma once

// #include <string>
#include <list>
#include "Mutex.h"

template<typename T>
class BufferList{
public:
    BufferList(){}
    virtual ~BufferList()
    {
        m_buflist.clear();
    }

    virtual bool AddBuffer(const T& t)
    {
        AutoLock lock(m_lock);
        m_buflist.emplace_back(t);
    }

    virtual T RemoveBuffer()
    {
        AutoLock lock(m_lock);
        if(m_buflist.size() > 0 )
        {
            T t = m_buflist.front();
            m_buflist.pop_front();
            return t;
        }
    }
private:
    std::list<T> m_buflist;
    MyLock m_lock;
};
// #include <iostream>
// int main(int argc, char const *argv[])
// {
//     BufferList<std::string> buflist;
//     buflist.AddBuffer(std::string("AAAAA"));
//     std::string str = buflist.RemoveBuffer();
    
//     std::cout << str << std::endl;
//     return 0;
// }
