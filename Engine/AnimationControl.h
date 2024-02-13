#pragma once
#include "Base.h"
BEGIN(Engine)
class AnimationControl final :public Base
{
private:
	LPD3DXANIMATIONCONTROLLER control = nullptr;
	_uint currentTrack = 0;//현재 재생되는 트랙
	_uint newTrack = 0;//바꿀 애니메이션 셋을 가지고 있는 트랙

	_double timeAcc = 0.0;

	_uint oldAnimationIndex = 0;
	_uint returnIndex = 0;

	_double period = 0.0;
	_uint mainTrack = 0;
	_bool play = true;
	_float frameTime;
	_float totalTime;
	_double timeSpeed;

	explicit AnimationControl(LPD3DXANIMATIONCONTROLLER control);
	explicit AnimationControl(const AnimationControl& ani);
	virtual ~AnimationControl() = default;

public:
	void SetTimeSpeed(_double timeSpeed) { this->timeSpeed = timeSpeed; }
	HRESULT Ready();
	HRESULT MainTrackSet(_uint index) { mainTrack = index; return NOERROR; }
	HRESULT AnimationSet(_uint index, _double period);
	HRESULT Play(_double timeDelta);
	HRESULT Stop(_double timeDelta);
	_bool Finished()const;

	static AnimationControl* Create(LPD3DXANIMATIONCONTROLLER control);
	virtual AnimationControl* Clone();
	virtual void Free();
};

END