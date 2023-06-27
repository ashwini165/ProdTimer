#pragma once
#include <cstdint>
#include <functional>

typedef std::function<void()> CallBack;

class ITimer {
public:
	/*set repeatCount = -1 for forever timer*/
	virtual uint32_t SetTimer(int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount = -1) = 0;
	virtual uint32_t SetDelayedTask(int64_t expiryTimeSinceEpoch, const CallBack& task) = 0;

	virtual void CancelTimer(uint32_t timerId) = 0;
};