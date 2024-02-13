#include "..\Headers\TimerMNG.h"
#include "Timer.h"
IMPLEMENT_SINGLETON(TimerMNG)
TimerMNG::TimerMNG()
{
}

Timer * TimerMNG::FindTimer(const _tchar * timerTag)
{
	auto iter = find_if(timers.begin(), timers.end(), TagFinder(timerTag));
	if (iter == timers.end())
		return nullptr;

	return iter->second;
}

HRESULT TimerMNG::AddTimer(const _tchar * timerTag)
{
	if (FindTimer(timerTag) != nullptr)
		return E_FAIL;
	Timer* timer = Timer::Create();
	if (timer == nullptr)
		return E_FAIL;

	timers.insert(TIMERS::value_type(timerTag, timer));
	return NOERROR;
}

_double TimerMNG::CalculateTimeDelta(const _tchar * timerTag)
{
	Timer* timer = FindTimer(timerTag);
	if (timer == nullptr)
		return 0.0;
	return timer->CalculateTimeDelta();
}

void TimerMNG::Free()
{
	for (auto& pair : timers)
	{
		SafeRelease(pair.second);
	}

	timers.clear();
}
