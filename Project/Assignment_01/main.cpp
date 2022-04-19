#pragma warning(disable: 28251)

#include "Stdafx/stdafx.h"

HINSTANCE		_hInstance;
HWND			_hWnd;
POINT			_ptMouse;
MainGame*		_mg;
bool			_mouseClick;

void SetWindowSize(int x, int y, int width, int height);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadResources();

int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow
) {
	_mg = new MainGame();
	_mouseClick = false;

	_hInstance = hInstance;

	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndClass);

	LoadResources();

	_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WINSTYLE,
		WINSTART_X,
		WINSTART_Y,
		WINSIZE_X,
		WINSIZE_Y,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL
	);

	SetWindowSize(WINSTART_X, WINSTART_Y, WINSIZE_X, WINSIZE_Y);
	ShowWindow(_hWnd, nCmdShow);

	if (FAILED(_mg->Init()))	return 0;

	MSG message;

	while (GetMessage(&message, 0, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return (int)message.wParam;
}

void SetWindowSize(int x, int y, int width, int height) {
	RECT rc{ 0, 0, width, height };
	AdjustWindowRect(&rc, WINSTYLE, false);
	SetWindowPos(_hWnd, NULL, x, y,
		(rc.right - rc.left),
		(rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE
	);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return _mg->MainProc(hWnd, message, wParam, lParam);
}

void LoadResources() {
	// stripe.
	IMG->init();

	IMG->AddImage(KEY_BACKGROUND_BATTLESTAGESCENE, BACKGROUND_BATTLESTAGESCENE,WINSIZE_X, WINSIZE_Y);
	IMG->FindImage(KEY_BACKGROUND_BATTLESTAGESCENE)->InitForAlphaBlend();

	IMG->AddImage(KEY_TILE_GROUND, TILE_GROUND, TILE_SIZE, TILE_SIZE);
	IMG->AddImage(KEY_TILE_PILLAR, TILE_PILLAR, TILE_SIZE, TILE_SIZE);
	IMG->AddImage(KEY_TILE_PILLAR_TOP, TILE_PILLAR_TOP, TILE_SIZE, TILE_SIZE);
	IMG->AddImage(KEY_TILE_WALL, TILE_WALL, TILE_SIZE, TILE_SIZE);

	IMG->AddImage(KEY_UI_BLACK_BACKGROUND, UI_BLACK_BACKGROUND, WINSIZE_X, WINSIZE_Y);
	IMG->FindImage(KEY_UI_BLACK_BACKGROUND)->InitForAlphaBlend();
	IMG->AddImage(KEY_UI_MINIMAP, UI_MINIMAP, WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_PLAYER_MARKER, UI_PLAYER_MARKER, 10, 10, true, RGB(255, 0, 255));

	IMG->AddImage(KEY_UI_GUN_BACKGROUND, UI_GUN_BACKGROUND, 160, 100, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_GUN_GUN, UI_GUN_GUN, 140, 80, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_GUN_BULLET_BASE, UI_GUN_BULLET_BASE, 20, 20, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_GUN_BULLET, UI_GUN_BULLET, 20, 10, true, RGB(255, 0, 255));

	IMG->AddImage(KEY_UI_HP_FULL, UI_HP_FULL, 30, 30, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_HP_HALF, UI_HP_HALF, 30, 30, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_HP_EMPTY, UI_HP_EMPTY, 30, 30, true, RGB(255, 0, 255));

	IMG->AddImage(KEY_UI_SKILL_BACKGROUND, UI_SKILL_BACKGROUND, 160, 100, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_SKILL_SKILL, UI_SKILL_SKILL, 160, 100, true, RGB(255, 0, 255));
	IMG->FindImage(KEY_UI_SKILL_SKILL)->InitForAlphaBlend();
	IMG->AddImage(KEY_UI_SKILL_COOL_BACKGROUND, UI_SKILL_COOL_BACKGROUND, 10, 100, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_SKILL_COOL_BAR, UI_SKILL_COOL_BAR, 10, 100, true, RGB(255, 0, 255));

	IMG->AddImage(KEY_UI_BUTTON_QUIT, UI_BUTTON_QUIT, 200, 200, false, RGB(255, 0, 255));

	IMG->AddImage(KEY_CUT_SCENE_BOSS, CUT_SCENE_BOSS, WINSIZE_X, WINSIZE_Y, true, RGB(255, 0, 255));

	IMG->AddImage(KEY_UI_BOSS_HP_BACKGROUND, UI_BOSS_HP_BACKGROUND, 534, 39, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_UI_BOSS_HP_BAR, UI_BOSS_HP_BAR, 440, 21, true, RGB(255, 0, 255));

	IMG->AddImage(KEY_BULLET_STRIPE, BULLET_STRIPE, 24, 24, true, RGB(255, 0, 255));

	IMG->AddImage(KEY_PLAYER_SHADOW, PLAYER_SHADOW, 64, 84, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_ENEMY_SHADOW, ENEMY_SHADOW, 64, 84, true, RGB(255, 0, 255));
	IMG->AddImage(KEY_BOSS_SHADOW, BOSS_SHADOW, 64, 84, true, RGB(255, 0, 255));

	// sound
	SOUND->Init();

	SOUND->AddSound(KEY_TITLE_THEME_SOUND, TITLE_THEME_SOUND, true, true);
	SOUND->AddSound(KEY_BATTLE_STAGE_THEME_SOUND, BATTLE_STAGE_THEME_SOUND, true, true);
	SOUND->AddSound(KEY_BOSS_THEME_SOUND, BOSS_THEME_SOUND, true, true);

	SOUND->AddSound(KEY_DEFAULT_GUN_SOUND, DEFAULT_GUN_SOUND, false, false);
	SOUND->AddSound(KEY_PLAYER_SKILL_SOUND, PLAYER_SKILL_SOUND, false, false);
	SOUND->AddSound(KEY_PLAYER_RELOAD_SOUND, PLAYER_RELOAD_SOUND, false, false);
}