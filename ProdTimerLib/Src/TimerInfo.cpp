#include "timerinfo.h"

TimerInfo::TimerInfo(uint32_t id, int64_t expiryTimeSinceEpoch, uint32_t delayInMilliSeconds, int32_t repeatCount, const CallBack& cb)
    : m_id(id), m_expiryTimeSinceEpoch(expiryTimeSinceEpoch), m_delayInMilliSeconds(delayInMilliSeconds), m_repeatCount(repeatCount), m_cb(cb)
{}

void TimerInfo::Run() {
    if (m_repeatCount > 0) {
        m_repeatCount--;
    }
    if (m_cb) {
        m_cb();
    }
}