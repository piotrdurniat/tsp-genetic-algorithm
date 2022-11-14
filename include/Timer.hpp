#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer
{
private:
    std::chrono::_V2::steady_clock::time_point startTime;
    std::chrono::_V2::steady_clock::time_point stopTime;
    unsigned long elapsedTimeNs;

public:
    /**
     * @brief Starts the timer
     */
    void start();
    /**
     * @brief Stops the timer
     */
    void stop();

    /**
     * @brief Prints the elapsed time between calling start() and stop() [ms]
     */
    void print();

    /**
     * @brief Returns the elapsed time between calling start() and this method
     */
    unsigned long getElapsedMin();

    unsigned long getElapsedS();

    unsigned long getElapsedMs();

    unsigned long getElapsedNs();

    std::chrono::duration<int64_t, std::nano> getElapsedTime();
};

#endif
