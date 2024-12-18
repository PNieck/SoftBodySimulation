#include <simulation/model/timedLoop.hpp>

#include <utility>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <iostream>


TimedLoop::TimedLoop(const int periodInMs, std::function<void()> fun):
    fun(std::move(fun)), periodInNs(periodInMs * oneMsInNs), nextClock(0)
{
}


TimedLoop::~TimedLoop()
{
    if (running)
        End();
}


void TimedLoop::ChangePeriod(const int periodInMs)
{
    periodInNs = periodInMs * oneMsInNs;
}


void TimedLoop::Start()
{
    if (running)
        throw std::runtime_error("TimedLoop already started");

    running = true;
    thread = std::thread(&TimedLoop::ThreadFunction, this);
}


void TimedLoop::End() {
    if (!running)
        throw std::runtime_error("TimedLoop not started");

    running = false;
    thread.join();
}


void TimedLoop::ThreadFunction()
{
    int64_t timeToWait = 0;
    nextClock = GetCurrentClockInNs();

    while (running) {
        fun();

        timeToWait = GetTimeToWait();

        if (timeToWait > 0) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(timeToWait));
        } else {
            //std::cout << "Not sleeping\n";
        }
    }
}


int64_t TimedLoop::GetCurrentClockInNs() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
         std::chrono::system_clock::now().time_since_epoch())
           .count();
}


int64_t TimedLoop::GetTimeToWait()
{
    nextClock += periodInNs;
    return nextClock - GetCurrentClockInNs();
}
