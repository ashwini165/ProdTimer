#include "TimerImpl.h"
#include "Helpers.h"

static uint32_t s_nextId = 1;

TimerImpl::TimerImpl(uint32_t timerStepInMilliSecond): m_timerStepInMilliSecond(timerStepInMilliSecond), m_stop(false){
    init();
}

bool TimerImpl::Start() {
    if (m_timerStepInMilliSecond < 50) {
        return false;
    }

    if (m_timeWheels.empty()) {
        return false;
    }

    m_timerThread = std::thread(std::bind(&TimerImpl::Run, this));
    m_workerThread = std::thread(std::bind(&TimerImpl::OnExpiry, this));

    return true;
}

void TimerImpl::Run() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_timerStepInMilliSecond));
        std::list<CallBack> expiredTimers;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_stop) {
                break;
            }

            TimeWheelPtr lowestTimeWheel = GetLowestTimeWheel();
            lowestTimeWheel->Increase();
            std::list<TimerInfoPtr> slot = std::move(lowestTimeWheel->GetAndClearCurrentSlot());
            for (const TimerInfoPtr& timer : slot) {
                auto it = m_cancelTimerIds.find(timer->Id());
                if (it != m_cancelTimerIds.end()) {
                    m_cancelTimerIds.erase(it);
                    continue;
                }
                expiredTimers.push_back(timer->GetCallBack());
                timer->DecreaseRepeatCount();
                uint32_t repeatCount = timer->GetRepeatCount();
                if (repeatCount == -1 || repeatCount > 0) {
                    timer->SetNextExpiry();
                    GetHighestTimeWheel()->AddTimer(timer);
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_taskQueueMutex);
            for (auto& cb : expiredTimers) {
                m_taskQueue.push(cb);
            }
        }
    }
}

void TimerImpl::OnExpiry() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(m_timerStepInMilliSecond));
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_stop) {
                break;
            }
        }
        {
            std::lock_guard<std::mutex> lock(m_taskQueueMutex);
            while (!m_taskQueue.empty()) {
                auto cb = m_taskQueue.front();
                m_taskQueue.pop();
                cb();
            }
        }
    }
}
TimerImpl::~TimerImpl() {
}

void TimerImpl::Stop() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
    }

    m_timerThread.join();
    m_workerThread.join();
}

TimeWheelPtr TimerImpl::GetHighestTimeWheel() {
    if (m_timeWheels.empty()) {
        return TimeWheelPtr();
    }

    return m_timeWheels.front();
}

TimeWheelPtr TimerImpl::GetLowestTimeWheel() {
    if (m_timeWheels.empty()) {
        return TimeWheelPtr();
    }

    return m_timeWheels.back();
}

uint32_t TimerImpl::SetTimerImpl(int64_t expiryTimeSinceEpoch, int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount){
    if (m_timeWheels.empty()) {
        return 0;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    ++s_nextId;
    GetHighestTimeWheel()->AddTimer(std::make_shared<TimerInfo>(s_nextId, expiryTimeSinceEpoch, delayInMilliSeconds, repeatCount, task));

    return s_nextId;
}

uint32_t TimerImpl::SetTimer(int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount) {
    int64_t expiryTimeSinceEpoch = GetNowTimestamp() + delayInMilliSeconds;
    return SetTimerImpl(expiryTimeSinceEpoch, delayInMilliSeconds, task, repeatCount);
}

uint32_t TimerImpl::SetTimer(int64_t expiryTimeSinceEpoch, const CallBack& task) {
    return SetTimerImpl(expiryTimeSinceEpoch, 0, task, 1);
}

void TimerImpl::CancelTimer(uint32_t timerID) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cancelTimerIds.insert(timerID);
}

void TimerImpl::init() {
    // Hour time wheel. 24 scales, 1 scale = 60 * 60 * 1000 ms.
    AppendTimeWheel(24, 60 * 60 * 1000, "HourTimeWheel");
    // Minute time wheel. 60 scales, 1 scale = 60 * 1000 ms.
    AppendTimeWheel(60, 60 * 1000, "MinuteTimeWheel");
    // Second time wheel. 60 scales, 1 scale = 1000 ms.
    AppendTimeWheel(60, 1000, "SecondTimeWheel");
    // Millisecond time wheel. 1000/m_timerStepInMilliSecond scales, 1 scale = m_timerStepInMilliSecond ms.
    AppendTimeWheel(1000 / m_timerStepInMilliSecond, m_timerStepInMilliSecond, "MillisecondTimeWheel");
}

void TimerImpl::AppendTimeWheel(uint32_t scales, uint32_t scaleUnitInMS, const std::string& name) {
    TimeWheelPtr timeWheel = std::make_shared<TimeWheel>(scales, scaleUnitInMS, name);
    if (m_timeWheels.empty()) {
        m_timeWheels.push_back(timeWheel);
        return;
    }

    TimeWheelPtr greaterTW = m_timeWheels.back();
    greaterTW->setSmallerLevelTW(timeWheel.get());
    timeWheel->setGreaterLevelTW(greaterTW.get());
    m_timeWheels.push_back(timeWheel);
}