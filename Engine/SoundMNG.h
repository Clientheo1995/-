#pragma once
#include "Base.h"
// FMOD: 유니티, 언리얼과 같은 상용 엔진에도 두루 쓰이는 사운드 관련 엔진.
// 사운드에 관련된 인터페이스를 제공.
BEGIN(Engine)
class SoundMNG:public Base
{
	DECLARE_SINGLETON(SoundMNG)
public:
	enum CHANNEL_ID { BGM, PLAYER,RUN, NPC,  MONSTER, BOSS, EFFECT, MAX_CHANNEL };

private:
	map<wstring, FMOD_SOUND*>	MapSound;//사운드 파일에 대한 정보를 갖는 구조체.
	FMOD_CHANNEL*	ChannelArr[MAX_CHANNEL];//사운드를 재생하고 관리하는 역할. ex) 볼륨 조절 등.
	FMOD_SYSTEM*	System;//FMOD_SOUND과 FMOD_CHANNEL을 총괄 관리하는 객체.

	explicit SoundMNG();
	virtual ~SoundMNG() = default;

	HRESULT LoadSoundFile();
	_int UpdateSound();

public:
	HRESULT Initialize();
	HRESULT PlaySFX(const wstring& wstrSoundKey, CHANNEL_ID ID, _bool loop=false);
	HRESULT PlayBGM(const wstring& wstrSoundKey);
	HRESULT StopSound(CHANNEL_ID ID);
	HRESULT StopAll();
	HRESULT SetVolume(CHANNEL_ID ID, float Vol);
	HRESULT SetMusicSpeed(const wstring& wstrSoundKey, float speed);
	virtual void Free();

};
END