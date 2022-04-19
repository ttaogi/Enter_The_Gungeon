#pragma once

#include <Windows.h>
#include <functional>

#include "../../MonoBehaviour/Button/Button.h"
#include "../../Image/Image.h"

class MainGame;

enum class SCENE_TYPE {
	NONE, TITLE, BATTLE_STAGE, END
};

class SceneManager {
protected:
	SCENE_TYPE scnT;
	Image* backBuffer;
	std::vector<MonoBehaviour*> effectVec;

	virtual void SetBackBuffer() = 0;
public:
	SceneManager(SCENE_TYPE _scnT) { scnT = _scnT; backBuffer = NULL; }
	~SceneManager() {}

	virtual void Init(MainGame* mg) = 0;
	virtual void Update(HWND hWnd) = 0;
	virtual void LateUpdate() = 0;
	virtual void Release() = 0;
	virtual void Render(HDC hdc) = 0;

	virtual SCENE_TYPE GetSceneType() { return scnT; }

	Image* GetBackBuffer() { return backBuffer; }
	std::vector<MonoBehaviour*>* GetEffectVec() { return &effectVec; }
	void AddEffect(MonoBehaviour* _effect) { effectVec.push_back(_effect); }
};