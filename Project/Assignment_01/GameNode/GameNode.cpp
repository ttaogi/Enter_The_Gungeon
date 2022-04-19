#include "Stdafx/stdafx.h"

#include <locale.h>

#include "GameNode.h"

GameNode::GameNode() { }

GameNode::~GameNode() { Release(); }

HRESULT GameNode::Init() {
	backBuffer = IMG->AddImage(KEY_BACKGROUND_BACKBUFFER, BACKGROUND_BACKBUFFER, WINSIZE_X, WINSIZE_Y);
	SetTimer(_hWnd, 1, 42, NULL);
	return S_OK;
}

HRESULT GameNode::Init(bool _managerInit) {
	nodeHdc = GetDC(_hWnd);
	managerInit = _managerInit;

	if (_managerInit) {
		IMG->init();
		_wsetlocale(LC_ALL, L"Korean");
	}

	return Init();
}

void GameNode::Release() {
	KillTimer(_hWnd, 1);
	KEY->ReleaseSingleton();
	RND->ReleaseSingleton();
	if (managerInit) {
		IMG->Release();
		IMG->ReleaseSingleton();
	}
	ReleaseDC(_hWnd, nodeHdc);
}

void GameNode::Update(HWND hWnd) {
	InvalidateRect(hWnd, NULL, true);
}

void GameNode::Render(HDC _hdc) { }

LRESULT GameNode::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, message, wParam, lParam);
}
