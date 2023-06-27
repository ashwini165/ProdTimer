#pragma once
#include <mutex>
#include <vector>
#include <queue>
#include <thread>
#include <unordered_set>

#include "TimeWheel.h"

class TimerImpl {
public:
	TimerImpl(uint32_t timerStepInMilliSecond);
	~TimerImpl();
	bool Start();
	void Stop();
		
	uint32_t SetTimer(int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount = -1);
	uint32_t SetTimer(int64_t expiryTimeSinceEpoch, const CallBack& task);
	void CancelTimer(uint32_t timerId);

private:
	void Run();
	void OnExpiry();

	TimeWheelPtr GetHighestTimeWheel();
	TimeWheelPtr GetLowestTimeWheel();
	void init();
	void AppendTimeWheel(uint32_t scales, uint32_t scale_unit_ms, const std::string& name = "");
	uint32_t SetTimerImpl(int64_t expiryTimeSinceEpoch, int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount);

private:
	std::mutex m_mutex;
	std::thread m_timerThread;
	std::thread m_workerThread;

	bool m_stop;

	std::mutex m_taskQueueMutex;
	std::queue <CallBack> m_taskQueue;
	std::unordered_set<uint32_t> m_cancelTimerIds;

	uint32_t m_timerStepInMilliSecond;
	std::vector<TimeWheelPtr> m_timeWheels;
};