#include "Timer.hpp"
#include <iostream>

void Timer::start()
{
    startTime = std::chrono::steady_clock::now();
}

void Timer::stop()
{
    stopTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(stopTime - startTime);
    elapsedTimeNs = elapsed.count();
}

void Timer::print()
{
    printf("%lu ns\n", elapsedTimeNs);
}

std::chrono::duration<int64_t, std::nano> Timer::getElapsedTime()
{
    auto now = std::chrono::steady_clock::now();
    return now - startTime;
}

unsigned long Timer::getElapsedMs()
{
    auto elapsedTime = getElapsedTime();
    return std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
}

unsigned long Timer::getElapsedNs()
{
    auto elapsedTime = getElapsedTime();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedTime).count();
}

unsigned long Timer::getElapsedS()
{
    auto elapsedTime = getElapsedTime();
    return std::chrono::duration_cast<std::chrono::seconds>(elapsedTime).count();
}

unsigned long Timer::getElapsedMin()
{
    auto elapsedTime = getElapsedTime();
    return std::chrono::duration_cast<std::chrono::minutes>(elapsedTime).count();
}
