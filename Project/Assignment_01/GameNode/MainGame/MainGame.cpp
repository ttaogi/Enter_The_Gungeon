#pragma warning(disable:28251)

#include "Stdafx/stdafx.h"

#include "MainGame.h"

MainGame::MainGame() {
	scnMgr = nullptr;
	nextScnType = SCENE_TYPE::NONE;
	quit = false;
}

MainGame::~MainGame() {
	Release();
}

HRESULT MainGame::Init() {
	if (scnMgr == nullptr) {
		scnMgr = new TitleSceneManager(this);
		scnMgr->Init(this);
	}

	return GameNode::Init();
}

HRESULT MainGame::Init(bool _managerInit) {
	if (scnMgr == nullptr) {
		scnMgr = new TitleSceneManager(this);
		scnMgr->Init(this);
	}

	return GameNode::Init(_managerInit);
}

void MainGame::Release() {
	if (scnMgr != nullptr) {
		scnMgr->Release();
		delete scnMgr;
		scnMgr = nullptr;
	}

	GameNode::Release();
}

void MainGame::Update(HWND hWnd) {
	if (quit == true) {
		Release();
		PostQuitMessage(0);
		return;
	}

	_mouseClick = false;
	GetCursorPos(&_ptMouse);
	ScreenToClient(hWnd, &_ptMouse);
	if (KEY->IsOnceKeyDown(VK_LBUTTON)) _mouseClick = true;

	switch (nextScnType) {
	case SCENE_TYPE::TITLE:
		nextScnType = SCENE_TYPE::NONE;
		if (scnMgr != nullptr) {
			scnMgr->Release();
			delete scnMgr;
			scnMgr = new TitleSceneManager(this);
			scnMgr->Init(this);
		}
		break;
	case SCENE_TYPE::BATTLE_STAGE:
		nextScnType = SCENE_TYPE::NONE;
		if (scnMgr != nullptr) {
			scnMgr->Release();
			delete scnMgr;
			scnMgr = new BattleStageSceneManager(this);
			scnMgr->Init(this);
		}
		break;
	case SCENE_TYPE::END:
		nextScnType = SCENE_TYPE::NONE;
		if (scnMgr != nullptr) {
			scnMgr->Release();
			delete scnMgr;
			scnMgr = new EndSceneManager(this);
			scnMgr->Init(this);
		}
		break;
	default:
		nextScnType = SCENE_TYPE::NONE;
		break;
	}

	if (scnMgr != nullptr) scnMgr->Update(hWnd);

	GameNode::Update(hWnd);
}

void MainGame::Render(HDC hdc) {
	if (scnMgr != nullptr) scnMgr->Render(hdc);
	GameNode::Render(hdc);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message) {
	case WM_CREATE:
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			Update(hWnd);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Render(hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return GameNode::MainProc(hWnd, message, wParam, lParam);
}

void MainGame::SetNextScene_BATTLESTAGE() {
	nextScnType = SCENE_TYPE::BATTLE_STAGE;
}

void MainGame::SetNextScene_TITLE() {
	nextScnType = SCENE_TYPE::TITLE;
}

void MainGame::SetNextScene_END() {
	nextScnType = SCENE_TYPE::END;
}

void MainGame::QuitGame() {
	quit = true;
}
