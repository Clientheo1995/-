#include "..\Headers\Timer.h"

Timer::Timer()
{
}

HRESULT Timer::Ready()
{
	QueryPerformanceCounter(&frameTime);
	QueryPerformanceCounter(&fixTime);
	QueryPerformanceCounter(&lastTime);
	QueryPerformanceFrequency(&cpuTick);
	return NOERROR;
}

_double Timer::CalculateTimeDelta()
{
	QueryPerformanceCounter(&frameTime);
	if (frameTime.QuadPart - fixTime.QuadPart >= cpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&cpuTick);
		fixTime = frameTime;
	}
	_double timeDelta= _double(frameTime.QuadPart - lastTime.QuadPart)/ cpuTick.QuadPart;
	QueryPerformanceCounter(&lastTime);
	return _double(timeDelta);
}

Timer * Timer::Create()
{
	Timer* instance = new Timer();
	if (FAILED(instance->Ready()))
	{
		MSG("Failed Create Timer");
		SafeRelease(instance);
	}
	return instance;
}

void Timer::Free()
{
}
