#pragma once

#include <functional>
#include <cstdint>
#include <atomic>
#include <thread>


class TimedLoop {
public:
    TimedLoop(int periodInMs, std::function<void(void)> fun);
    ~TimedLoop();

    void ChangePeriod(int periodInMs);

    void Start();
    void End();

    [[nodiscard]]
    bool IsRunning() const
        { return running; }

private:

    void ThreadFunction();

    static int64_t GetCurrentClockInNs();
    int64_t GetTimeToWait();

    std::function<void(void)> fun;
    int64_t periodInNs;
    int64_t nextClock;

    std::atomic_bool running;
    std::thread thread;

    static constexpr uint64_t oneMsInNs = 1000000L;
};
