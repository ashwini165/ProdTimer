#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include "../Interface/ITimer.h"


class TimerInfo {
public:
    TimerInfo(uint32_t id, int64_t expiryTimeSinceEpoch, uint32_t delayInMilliSeconds, int32_t repeatCount, const CallBack& cb);

    void Run();

    uint32_t Id() const {
        return m_id;
    }

    int64_t ExpiryTimeSinceEpoch() const {
        return m_expiryTimeSinceEpoch;
    }

    void SetNextExpiry() {
        m_expiryTimeSinceEpoch += m_delayInMilliSeconds;
    }

    uint32_t DelayInMilliSeconds() const {
        return m_delayInMilliSeconds;
    }

    int32_t GetRepeatCount() const {
        return m_repeatCount;
    }

private:
    uint32_t m_id;
    CallBack m_cb;
    int64_t m_expiryTimeSinceEpoch;
    uint32_t m_delayInMilliSeconds;
    int32_t m_repeatCount;
};

using TimerInfoPtr = std::shared_ptr<TimerInfo>;
