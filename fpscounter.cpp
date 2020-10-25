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
    m_frameCount[t]++;
}

std::string FpsCounter::toString()
{
    int64_t ms = microSecondsFromEpoch();

    std::string result;
    for (int i = 0; i < TypeCount; i++)
    {
        result += m_title[i] + ": ";
        if (m_frameCount[i] == 0)
        {
            result += "0";
        }
        else
        {
            float fps = m_frameCount[i] * 1.0f / ((ms - m_lastTick[i]) / 1000000.0f);
            result += std::to_string(fps);
        }
        result += " ";

        m_frameCount[i] = 0;
        m_lastTick[i] = ms;
    }
    return result;
}

FpsCounter::FpsCounter()
{
    m_title[Render] = "Render";
    m_title[Display] = "Display";

    for (int i = 0; i < TypeCount; i++)
    {
        m_frameCount[i] = 0;
        m_lastTick[i] = 0;
    }
}

int64_t FpsCounter::microSecondsFromEpoch()
{
    auto t = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t.time_since_epoch()).count();
}
