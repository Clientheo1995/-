#include "AnimationControl.h"

AnimationControl::AnimationControl(LPD3DXANIMATIONCONTROLLER control) :control(control)
{
	AddRefCount(this->control);
	timeSpeed = 1.0;
}

AnimationControl::AnimationControl(const AnimationControl & ani) :control(nullptr)
{
	ani.control->CloneAnimationController(ani.control->GetMaxNumAnimationOutputs(), ani.control->GetMaxNumAnimationSets(), ani.control->GetMaxNumTracks(), ani.control->GetMaxNumEvents(), &control);
	timeSpeed = 1.0;
}

HRESULT AnimationControl::Ready()
{
	return NOERROR;
}

HRESULT AnimationControl::AnimationSet(_uint index, _double peri)
{
	if (control == nullptr)
		return E_FAIL;

	if (oldAnimationIndex == index)
		return E_FAIL;
	newTrack = currentTrack == 0 ? 1 : 0;

	LPD3DXANIMATIONSET aniSet = nullptr;

	control->GetAnimationSet(index, &aniSet);
	control->SetTrackAnimationSet(newTrack, aniSet);

	period = aniSet->GetPeriod() - peri;//애니메이션마다 모션 깔끔하게 끝나는게 다름

	SafeRelease(aniSet);

	control->UnkeyAllTrackEvents(currentTrack);
	control->UnkeyAllTrackEvents(newTrack);

	control->KeyTrackEnable(currentTrack, FALSE, timeAcc + 0.2);
	control->KeyTrackSpeed(currentTrack, 1.f, timeAcc, 0.2, D3DXTRANSITION_LINEAR);//선형보간
	control->KeyTrackWeight(currentTrack, 0.1f, timeAcc, 0.2, D3DXTRANSITION_LINEAR);

	control->SetTrackEnable(newTrack, TRUE);
	control->KeyTrackSpeed(newTrack, 1.f, timeAcc, 0.2, D3DXTRANSITION_LINEAR);//선형보간
	control->KeyTrackWeight(newTrack, 0.9f, timeAcc, 0.2, D3DXTRANSITION_LINEAR);

	currentTrack = newTrack;
	timeAcc = 0.0;
	control->ResetTime();
	control->SetTrackPosition(currentTrack, 0.0);

	oldAnimationIndex = index;
	return NOERROR;
}

HRESULT AnimationControl::Play(_double timeDelta)
{
	if (control == nullptr)
		return E_FAIL;
	control->AdvanceTime(timeDelta*timeSpeed, nullptr);
	timeAcc += (timeDelta*timeSpeed);
	return NOERROR;
}

HRESULT AnimationControl::Stop(_double timeDelta)
{
	D3DXTRACK_DESC trackDesc;
	control->GetTrackDesc(currentTrack, &trackDesc);
	return NOERROR;
}

//기존 인덱스를 저장
//필요하다면 피리오드 입력
//입력되지 않았다면 기존의 피리오드를 사용
_bool AnimationControl::Finished()const
{
	if (control == nullptr)
		return false;
	D3DXTRACK_DESC trackDesc;
	control->GetTrackDesc(currentTrack, &trackDesc);
	if (period != 0.0f)
	{
		if (period <= trackDesc.Position)
			return true;
	}

	return _bool(false);
}

AnimationControl * AnimationControl::Create(LPD3DXANIMATIONCONTROLLER control)
{
	AnimationControl* instance = new AnimationControl(control);
	if (FAILED(instance->Ready()))
	{
		MSG("Failed Create AnimationControl");
		SafeRelease(instance);
	}
	return instance;
}

AnimationControl * AnimationControl::Clone()
{
	return new AnimationControl(*this);
}

void AnimationControl::Free()
{
	SafeRelease(control);
}
