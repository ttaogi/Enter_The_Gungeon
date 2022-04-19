#include "Stdafx/stdafx.h"

#include "TitleSceneManager.h"

TitleSceneManager::TitleSceneManager(MainGame* mg)
	: SceneManager(SCENE_TYPE::TITLE)
{
	gameStartBtn = Button(
		RECT{
			30, 610, 130, 660
		}
	);
	gameStartBtn.SetCallBack_v_CB_v(std::bind(&MainGame::SetNextScene_BATTLESTAGE, mg));
	quitBtn = Button(
		RECT{
			30, 670, 130, 720
		}
	);
	quitBtn.SetCallBack_v_CB_v(std::bind(&MainGame::QuitGame, mg));
	bgImage = NULL;
}

void TitleSceneManager::SetBackBuffer() {
	SAFE_RELEASE(backBuffer);
	SAFE_DELETE(backBuffer);
	backBuffer = new Image;
	backBuffer->Init(WINSIZE_X, WINSIZE_Y);
}

void TitleSceneManager::Init(MainGame* mg) {
	SetBackBuffer();

	SOUND->Play(KEY_TITLE_THEME_SOUND);

	bgImage = new Image();
	bgImage->Init(BACKGROUND_TITLESCENE, WINSIZE_X, WINSIZE_Y);
	bgImage->InitForAlphaBlend();
}

void TitleSceneManager::Update(HWND hWnd) {
	gameStartBtn.Update(hWnd);
	quitBtn.Update(hWnd);
	SOUND->Update();
}

void TitleSceneManager::LateUpdate() { }

void TitleSceneManager::Release() {
	SAFE_RELEASE(backBuffer);
	SAFE_DELETE(backBuffer);

	SOUND->AllStop();
}

void TitleSceneManager::Render(HDC hdc) {
	HDC memDC = GetBackBuffer()->GetMemDC();

	PatBlt(memDC, 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

	bgImage->Render(memDC, 0, 0);

	GetBackBuffer()->Render(hdc, 0, 0);
}
