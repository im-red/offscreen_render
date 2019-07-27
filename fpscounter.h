#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include <string>
#include <atomic>

class FpsCounter
{
public:
    enum Type
    {
        Render = 0,
        Display,
        TypeNum
    };

public:
    static FpsCounter *instance();
    void frame(Type t);
    std::string toString();
    static int64_t microSecondsFromEpoch();

private:
    FpsCounter();

private:
    FpsCounter(const FpsCounter &) = delete;
    FpsCounter &operator =(const FpsCounter &) = delete;
    FpsCounter(const FpsCounter &&) = delete;
    FpsCounter &operator =(const FpsCounter &&) = delete;

private:
    std::string m_title[TypeNum];
    std::atomic<int> m_interval[TypeNum];   // in microsecond
    int64_t m_lastTick[TypeNum];            // in microsecond
};

#endif // FPSCOUNTER_H
