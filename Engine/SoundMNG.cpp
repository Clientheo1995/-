#include "..\Headers\SoundMNG.h"

IMPLEMENT_SINGLETON(SoundMNG)

SoundMNG::SoundMNG()
{
}
HRESULT SoundMNG::Initialize()
{
	FMOD_System_Create(&System);
	FMOD_System_Init(System, MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);

	LoadSoundFile();
	return NOERROR;
}

_int SoundMNG::UpdateSound()
{
	FMOD_System_Update(System);
	return _int();
}

HRESULT SoundMNG::PlaySFX(const wstring & wstrSoundKey, CHANNEL_ID ID, _bool loop)
{
	auto iter_find = MapSound.find(wstrSoundKey);

	if (MapSound.end() == iter_find)
		return E_FAIL;

	FMOD_System_PlaySound(System, FMOD_CHANNEL_FREE, iter_find->second,
		FALSE, &ChannelArr[ID]);
	if (loop)
	{
	FMOD_Channel_SetMode(ChannelArr[ID], FMOD_LOOP_NORMAL);
	}
	UpdateSound();
	return NOERROR;
}

HRESULT SoundMNG::PlayBGM(const wstring & wstrSoundKey)
{
	auto iter_find = MapSound.find(wstrSoundKey);

	if (MapSound.end() == iter_find)
		return E_FAIL;

	FMOD_System_PlaySound(System, FMOD_CHANNEL_FREE, iter_find->second,
		FALSE, &ChannelArr[BGM]);

	FMOD_Channel_SetMode(ChannelArr[BGM], FMOD_LOOP_NORMAL);

	UpdateSound();
	return NOERROR;
}

HRESULT SoundMNG::StopSound(CHANNEL_ID ID)
{
	FMOD_Channel_Stop(ChannelArr[ID]);
	UpdateSound();
	return NOERROR;
}

HRESULT SoundMNG::StopAll()
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
		FMOD_Channel_Stop(ChannelArr[i]);

	UpdateSound();
	return NOERROR;
}

HRESULT SoundMNG::SetVolume(CHANNEL_ID ID, float Vol)
{
	FMOD_Channel_SetVolume(ChannelArr[ID], Vol);
	UpdateSound();
	return NOERROR;
}

HRESULT SoundMNG::SetMusicSpeed(const wstring& wstrSoundKey, float speed)
{

	auto iter_find = MapSound.find(wstrSoundKey);

	if (MapSound.end() == iter_find)
		return E_FAIL;

	FMOD_Sound_SetMusicSpeed(iter_find->second, speed);

	UpdateSound();
	return NOERROR;
}

void SoundMNG::Free()
{
	for_each(MapSound.begin(), MapSound.end(),
		[](auto& MyPair)
	{
		FMOD_Sound_Release(MyPair.second);
	});

	MapSound.clear();

	FMOD_System_Release(System);
	FMOD_System_Close(System);

	return;
}



HRESULT SoundMNG::LoadSoundFile()
{
	_finddata_t fd;

	int handle = _findfirst("../../Sound/*.*", &fd);

	if (0 == handle)
		return E_FAIL;
	//MSG( L"_findfirst failed!");

	char szFullPath[MAXCHAR] = "";
	char szRelativePath[MAXCHAR] = "../../Sound/";

	int iResult = 0;

	while (-1 != iResult)
	{
		// szFullPath: "../Sound/"
		strcpy_s(szFullPath, szRelativePath);

		// fd.name: "Adam Levine - Lost Stars Lyrics.mp3"
		// szFullPath: "../Sound/Adam Levine - Lost Stars Lyrics.mp3"
		strcat_s(szFullPath, fd.name);

		// fd.name: "Adam Levine - Lost Stars Lyrics.mp3"
		TCHAR* SoundKey = new TCHAR[strlen(fd.name) + 1];

		// 멀티 -> 와이드로 변환.
		MultiByteToWideChar(CP_ACP, 0, fd.name, strlen(fd.name) + 1,
			SoundKey, strlen(fd.name) + 1);

		FMOD_SOUND* Sound = nullptr;

		FMOD_RESULT eResult = FMOD_System_CreateSound(System, szFullPath,
			FMOD_HARDWARE, nullptr, &Sound);

		if (FMOD_OK == eResult)
		{
			auto& iter_find = MapSound.find(SoundKey);

			if (MapSound.end() == iter_find)
				MapSound.insert({ SoundKey, Sound });
			else
			{
				delete[] SoundKey;
				FMOD_Sound_Release(Sound);
			}
		}
		else
			delete[] SoundKey;

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(System);
	return NOERROR;
}
