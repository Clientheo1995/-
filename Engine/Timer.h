#pragma once
#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL Timer final:
	public Base
{
private:
	LARGE_INTEGER frameTime;
	LARGE_INTEGER fixTime;
	LARGE_INTEGER lastTime;
	LARGE_INTEGER cpuTick;

	explicit Timer();
	virtual ~Timer() = default;

public:
	HRESULT Ready();
	_double CalculateTimeDelta();
	static Timer* Create();
	virtual void Free();
};

	END