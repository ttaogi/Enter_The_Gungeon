#pragma once

#include "../SceneManager.h"

class TitleSceneManager : public SceneManager {
private:
	Button gameStartBtn;
	Button quitBtn;
	Image* bgImage;
	
	void SetBackBuffer();
public:
	TitleSceneManager(MainGame* mg);
	~TitleSceneManager() {}

	void Init(MainGame *mg);
	void Update(HWND hWnd);
	void LateUpdate();
	void Release();
	void Render(HDC hdc);
};