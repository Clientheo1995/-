#pragma once
#include "Base.h"
// FMOD: ����Ƽ, �𸮾�� ���� ��� �������� �η� ���̴� ���� ���� ����.
// ���忡 ���õ� �������̽��� ����.
BEGIN(Engine)
class SoundMNG:public Base
{
	DECLARE_SINGLETON(SoundMNG)
public:
	enum CHANNEL_ID { BGM, PLAYER,RUN, NPC,  MONSTER, BOSS, EFFECT, MAX_CHANNEL };

private:
	map<wstring, FMOD_SOUND*>	MapSound;//���� ���Ͽ� ���� ������ ���� ����ü.
	FMOD_CHANNEL*	ChannelArr[MAX_CHANNEL];//���带 ����ϰ� �����ϴ� ����. ex) ���� ���� ��.
	FMOD_SYSTEM*	System;//FMOD_SOUND�� FMOD_CHANNEL�� �Ѱ� �����ϴ� ��ü.

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