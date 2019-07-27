#include "fpscounter.h"

#include <chrono>

FpsCounter *FpsCounter::instance()
{
    static FpsCounter s_instance;
    return &s_instance;
}

// one type is only used in one thread
void FpsCounter::frame(FpsCounter::Type t)
{
    auto us = microSecondsFromEpoch();
    auto last = m_lastTick[t];
    if (last != 0)
    {
        m_interval[t] = us - m_lastTick[t];
    }
    m_lastTick[t] = us;
}

std::string FpsCounter::toString()
{
    std::string result;
    for (int i = 0; i < TypeNum; i++)
    {
        result += m_title[i] + ": ";
        if (m_interval[i] == 0)
        {
            result += "?";
        }
        else
        {
            float fps = 1.0f / (m_interval[i] / 1000000.0f);
            result += std::to_string(fps);
        }
        result += " ";
    }
    return result;
}

FpsCounter::FpsCounter()
{
    m_title[Render] = "Render";
    m_title[Display] = "Display";

    for (int i = 0; i < TypeNum; i++)
    {
        m_interval[i] = 0;
        m_lastTick[i] = 0;
    }
}

int64_t FpsCounter::microSecondsFromEpoch()
{
    auto t = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(t.time_since_epoch()).count();
}
