#include "TimeWheel.h"
#include "Helpers.h"

TimeWheel::TimeWheel(uint32_t scales, uint32_t scaleUnitMS, const std::string& name)
    : m_name(name)
    , m_currentIndex(0)
    , m_scales(scales)
    , m_scaleUnitMS(scaleUnitMS)
    , m_slots(scales)
    , m_greaterLevelTW(nullptr)
    , m_lessLevelTW(nullptr) {
}

int64_t TimeWheel::GetCurrentTime() const {
    int64_t time = m_currentIndex * m_scaleUnitMS;
    if (m_lessLevelTW != nullptr) {
        time += m_lessLevelTW->GetCurrentTime();
    }

    return time;
}

void TimeWheel::AddTimer(TimerInfoPtr timer) {
    int64_t lessTWTime = 0;
    if (m_lessLevelTW != nullptr) {
        lessTWTime = m_lessLevelTW->GetCurrentTime();
    }
    int64_t diff = timer->ExpiryTimeSinceEpoch() + lessTWTime - GetNowTimestamp();

    // If the difference is greater than scale unit, the timer can be added into the current time wheel.
    if (diff >= m_scaleUnitMS) {
        size_t n = (m_currentIndex + diff / m_scaleUnitMS) % m_scales;
        m_slots[n].push_back(timer);
        return;
    }

    // If the difference is less than scale uint, the timer should be added into less level time wheel.
    if (m_lessLevelTW != nullptr) {
        m_lessLevelTW->AddTimer(timer);
        return;
    }

    // If the current time wheel is the least level, the timer can be added into the current time wheel.
    m_slots[m_currentIndex].push_back(timer);
}

void TimeWheel::Increase() {
    // Increase the time wheel.
    ++m_currentIndex;
    if (m_currentIndex < m_scales) {
        return;
    }

    // If the time wheel is full, the greater level time wheel should be increased.
    // The timers in the current slot of the greater level time wheel should be moved into
    // the less level time wheel.
    m_currentIndex = m_currentIndex % m_scales;
    if (m_greaterLevelTW != nullptr) {
        m_greaterLevelTW->Increase();
        std::list<TimerInfoPtr> slot = std::move(m_greaterLevelTW->GetAndClearCurrentSlot());
        for (TimerInfoPtr timer : slot) {
            AddTimer(timer);
        }
    }
}

std::list<TimerInfoPtr> TimeWheel::GetAndClearCurrentSlot() {
    std::list<TimerInfoPtr> slot;
    slot = std::move(m_slots[m_currentIndex]);
    return slot;
}
