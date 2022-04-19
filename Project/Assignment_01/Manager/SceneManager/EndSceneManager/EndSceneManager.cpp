#include "Stdafx/stdafx.h"

#include "EndSceneManager.h"

EndSceneManager::EndSceneManager(MainGame* mg)
	: SceneManager(SCENE_TYPE::TITLE)
{
	retryBtn = Button(
		RECT{
			WINSIZE_X / 2 - 210, WINSIZE_Y * 3 / 4 - 100,
			WINSIZE_X / 2 - 10, WINSIZE_Y * 3 / 4 + 100
		}, L"RETRY"
	);
	retryBtn.SetCallBack_v_CB_v(std::bind(&MainGame::SetNextScene_BATTLESTAGE, mg));
	quitBtn = Button(
		RECT{
			WINSIZE_X / 2 + 10, WINSIZE_Y * 3 / 4 - 100,
			WINSIZE_X / 2 + 210, WINSIZE_Y * 3 / 4 + 100
		}, L"QUIT"
	);
	quitBtn.SetCallBack_v_CB_v(std::bind(&MainGame::QuitGame, mg));
	bgImage = NULL;
}

void EndSceneManager::SetBackBuffer() {
	SAFE_RELEASE(backBuffer);
	SAFE_DELETE(backBuffer);
	backBuffer = new Image;
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);
}

void EndSceneManager::Init(MainGame* mg) {
	SetBackBuffer();

	bgImage = new Image();
	bgImage->Init(BACKGROUND_ENDSCENE, WINSIZE_X, WINSIZE_Y);
	bgImage->InitForAlphaBlend();
}

void EndSceneManager::Update(HWND hWnd) {
	retryBtn.Update(hWnd);
	quitBtn.Update(hWnd);
}

void EndSceneManager::LateUpdate() { }

void EndSceneManager::Release() { }

void EndSceneManager::Render(HDC hdc) {
	HDC memDC = GetBackBuffer()->GetMemDC();

	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

	bgImage->Render(memDC, 0, 0);

	retryBtn.Render(memDC);
	quitBtn.Render(memDC);

	GetBackBuffer()->Render(hdc, 0, 0);
}
