#pragma once

#include <map>

#include "../../DesignPattern/SingletonBase/SingletonBase.h"
#pragma warning(push)
#pragma warning(disable: 26812)
#include "fmod.hpp"
#pragma warning(pop)

using namespace FMOD;

#define EXTRACHANNELBUFFER	5
#define SOUNDBUFFER			20
#define TOTALSOUNDBUFFER	(SOUNDBUFFER + EXTRACHANNELBUFFER)

class SoundManager : public SingletonBase<SoundManager> {
private:
	std::map<std::wstring, Sound**> m_totalSounds;

	System* m_system;
	Sound** m_sound;
	Channel** m_channel;
public:
	SoundManager();
	~SoundManager();

	HRESULT Init();
	void Release();
	void Update();

	// bgm - true : bgm, false : sfx.
	void AddSound(std::wstring _keyName, std::wstring _fileName, bool _bgm = FALSE, bool _loop = FALSE);
	// volume : 0.0f ~ 1.0f.
	void Play(std::wstring _keyName, float _volume = 1.0f);
	// go to start point.
	void Stop(std::wstring _keyName);
	void AllStop();
	void Pause(std::wstring _keyName);
	void Resume(std::wstring _keyName);

	bool IsPlaySound(std::wstring _keyName);
	bool isPauseSound(std::wstring _keyName);
};
