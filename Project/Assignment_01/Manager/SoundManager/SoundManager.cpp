#include "Stdafx/stdafx.h"

#include "SoundManager.h"

SoundManager::SoundManager() : m_system(NULL), m_channel(NULL), m_sound(NULL) { }

SoundManager::~SoundManager() { }

HRESULT SoundManager::Init() {
	System_Create(&m_system);

	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	m_sound = new Sound*[TOTALSOUNDBUFFER];
	m_channel = new Channel*[TOTALSOUNDBUFFER];

	for (int i = 0; i < TOTALSOUNDBUFFER; i++) {
		m_sound[i] = NULL;
		m_channel[i] = NULL;
	}

	return S_OK;
}

void SoundManager::Release() {
	if (m_channel != NULL || m_sound != NULL) {
		for (int i = 0; i < TOTALSOUNDBUFFER; i++) {
			if (m_channel != NULL && m_channel[i] != NULL) m_channel[i]->stop();
			if (m_sound != NULL && m_sound[i] != NULL) m_sound[i]->release();
		}
	}
	SAFE_DELETE_ARRAY(m_sound);
	SAFE_DELETE_ARRAY(m_channel);

	if (m_system != NULL) {
		m_system->release();
		m_system->close();
	}
}

void SoundManager::Update() {
	m_system->update();
}

void SoundManager::AddSound(wstring _keyName, wstring _wfileName, bool _bgm, bool _loop) {
	string fileName = "";
	WStringToString(fileName, _wfileName);
	if (_loop) {
		if (_bgm) {
			m_system->createStream(fileName.c_str(),
				FMOD_LOOP_NORMAL, NULL,
				&m_sound[m_totalSounds.size()]);
		}
		else {
			m_system->createSound(fileName.c_str(),
				FMOD_LOOP_NORMAL, NULL,
				&m_sound[m_totalSounds.size()]);
		}
	}
	else {
		m_system->createSound(fileName.c_str(),
			FMOD_DEFAULT, NULL,
			&m_sound[m_totalSounds.size()]);
	}
	m_totalSounds[_keyName] = &m_sound[m_totalSounds.size()];
}

void SoundManager::Play(wstring _keyName, float _volume) {
	int count = 0;
	for (auto iter = m_totalSounds.begin(); iter != m_totalSounds.end(); ++iter, count++) {
		if (_keyName == iter->first) {
			m_system->playSound(*(iter->second), NULL, false, &m_channel[count]);
			m_channel[count]->setVolume(_volume);
			break;
		}
	}
}

void SoundManager::Stop(wstring _keyName) {
	int count = 0;
	map<string, Sound**>::iterator iter;
	for (auto iter = m_totalSounds.begin(); iter != m_totalSounds.end(); ++iter, count++) {
		if (_keyName == iter->first) {
			m_channel[count]->stop();
			break;
		}
	}
}

void SoundManager::AllStop() {
	int count = 0;
	for (auto iter = m_totalSounds.begin(); iter != m_totalSounds.end(); ++iter, ++count) {
		m_channel[count]->stop();
	}
}

void SoundManager::Pause(wstring _keyName) {
	int count = 0;
	for (auto iter = m_totalSounds.begin(); iter != m_totalSounds.end(); ++iter, count++) {
		if (_keyName == iter->first) {
			m_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(wstring _keyName) {
	int count = 0;
	for (auto iter = m_totalSounds.begin(); iter != m_totalSounds.end(); ++iter, count++) {
		if (_keyName == iter->first) {
			m_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPlaySound(wstring _keyName) {
	bool isPlay = false;
	int count = 0;
	for (auto iter = m_totalSounds.begin(); iter != m_totalSounds.end(); ++iter, count++) {
		if (_keyName == iter->first) {
			m_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool SoundManager::isPauseSound(wstring _keyName) {
	bool isPaused = false;
	int count = 0;
	for (auto iter = m_totalSounds.begin(); iter != m_totalSounds.end(); ++iter, count++) {
		if (_keyName == iter->first) {
			m_channel[count]->getPaused(&isPaused);
			break;
		}
	}

	return isPaused;
}