#pragma once
#include <chrono>
#include <string>
#include <memory>
#include <vector>
#include <list>

#include "timerinfo.h"

class TimeWheel {
public:
    TimeWheel(uint32_t scales, uint32_t scaleUnitMS, const std::string& name = "");

    uint32_t scaleUnitMS() const {
        return m_scaleUnitMS;
    }

    uint32_t scales() const {
        return m_scales;
    }

    uint32_t currentIndex() const {
        return m_currentIndex;
    }

    void setSmallerLevelTW(TimeWheel* lessLevelTW) {
        m_lessLevelTW = lessLevelTW;
    }

    void setGreaterLevelTW(TimeWheel* greaterLevelTW) {
        m_greaterLevelTW = greaterLevelTW;
    }

    int64_t GetCurrentTime() const;

    void AddTimer(TimerInfoPtr timer);

    void Increase();

    std::list<TimerInfoPtr> GetAndClearCurrentSlot();

private:
    std::string m_name;
    uint32_t m_currentIndex;

    // A time wheel can be devided into multiple scales. A scale has N ms.
    uint32_t m_scales;
    uint32_t m_scaleUnitMS;

    // Every slot corresponds to a scale. Every slot contains the timers.
    std::vector<std::list<TimerInfoPtr>> m_slots;

    TimeWheel* m_lessLevelTW;  // Less scale unit.
    TimeWheel* m_greaterLevelTW;  // Greater scale unit.
};

using TimeWheelPtr = std::shared_ptr<TimeWheel>;