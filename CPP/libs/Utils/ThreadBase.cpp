
#include "ThreadBase.h"

bool ThreadBase::Start()
{
    m_bActive = true;
    Init();   //call server thread init
    m_thread = std::thread(std::bind(&ThreadBase::Loop, this));
    return true;
}

bool ThreadBase::Stop()
{
    m_bActive = false;
    Join();
    return true;
}

void ThreadBase::Join()
{
    if(m_thread.joinable())
    {
        m_thread.join();
    }
}



