#pragma once
#include <cstdint>
#include <functional>

typedef std::function<void()> CallBack;

class ITimer {
public:
	virtual uint32_t SetTimer(int32_t delayInMilliSeconds, const CallBack& task, uint32_t repeatCount = -1) = 0;

	virtual void CancelTimer(uint32_t timerId) = 0;
};