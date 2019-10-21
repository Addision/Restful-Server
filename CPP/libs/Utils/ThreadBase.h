#pragma once

#include <thread>
#include <functional>

class ThreadBase{
public:
    virtual bool Start();
    virtual bool Stop();
    virtual bool Init() = 0;
    virtual void Loop() = 0;

    bool Joinable() { return m_thread.joinable(); }
    void Join();
    bool IsActive() { return false; }
    std::thread::id GetThreadId() { return m_thread.get_id(); }
    virtual ~ThreadBase(){}

private:
    std::thread m_thread;
    bool m_bActive{false};
};