#pragma once

#include "../SceneManager.h"

class EndSceneManager : public SceneManager {
private:
	Button retryBtn;
	Button quitBtn;
	Image* bgImage;
	
	void SetBackBuffer();
public:
	EndSceneManager(MainGame* mg);
	~EndSceneManager() {}

	void Init(MainGame* mg);
	void Update(HWND hWnd);
	void LateUpdate();
	void Release();
	void Render(HDC hdc);
};