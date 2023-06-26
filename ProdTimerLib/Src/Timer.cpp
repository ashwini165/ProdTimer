#include "../Interface/Timer.h"
#include "TimerImpl.h"

Timer::Timer(): m_timerImpl(std::make_unique<TimerImpl>(Timer::TimerStepInMilliSecond))
{}

Timer& Timer::instance() {
    static const std::unique_ptr<Timer> instance{new Timer()};
    return *instance;
}

uint32_t Timer::SetTimer(int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount) {
    return m_timerImpl->SetTimer(delayInMilliSeconds, task, repeatCount);
}

void Timer::CancelTimer(uint32_t timerId) {
    m_timerImpl->CancelTimer(timerId);
}

bool Timer::Start() {
    return m_timerImpl->Start();
}

void Timer::Stop() {
    m_timerImpl->Stop();
}