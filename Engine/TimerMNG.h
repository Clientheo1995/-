#pragma once
#include "Base.h"
BEGIN(Engine)
class Timer;
class TimerMNG final:
	public Base
{
		DECLARE_SINGLETON(TimerMNG)
private:
		unordered_map<const _tchar*, Timer*> timers;
	typedef unordered_map<const _tchar*, Timer*> TIMERS;

	explicit TimerMNG();
	virtual ~TimerMNG() = default;
	Timer* FindTimer(const _tchar* timerTag);

public:
	HRESULT AddTimer(const _tchar* timerTag);
	_double CalculateTimeDelta(const _tchar* timerTag);
	virtual void Free();
};

	END