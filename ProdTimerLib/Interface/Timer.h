#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <unordered_set>

#include "ITimer.h"
class TimerImpl; // Forward declaration of the implementation class

class Timer : public ITimer {
    const uint32_t TimerStepInMilliSecond = 50;

private:
    Timer(); // Disallow instantiation outside of the class.
public:
    static Timer& instance();
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer&&) = delete;

    /*set repeatCount = -1 for forever timer*/
    uint32_t SetTimer(int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount = -1) override;
    uint32_t SetDelayedTask(int64_t expiryTimeSinceEpoch, const CallBack& task) override;

    void CancelTimer(uint32_t timerId) override;
    bool Start();
    void Stop();

private:
	std::unique_ptr<TimerImpl> m_timerImpl;    // PIMPL
};
