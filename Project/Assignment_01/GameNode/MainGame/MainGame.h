#pragma once

#include "../GameNode.h"
#include "../../Manager/SceneManager/BattleStageSceneManager/BattleStageSceneManager.h"
#include "../../Manager/SceneManager/EndSceneManager/EndSceneManager.h"
#include "../../Manager/SceneManager/TitleSceneManager/TitleSceneManager.h"

class MainGame : public GameNode {
	SceneManager* scnMgr;
	SCENE_TYPE nextScnType;
	bool quit;
public:
	MainGame();
	~MainGame();

	HRESULT Init();
	HRESULT Init(bool _managerInit);
	void Release();

	void Update(HWND hWnd);
	void Render(HDC hdc);

	LRESULT MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetNextScene_BATTLESTAGE();
	void SetNextScene_TITLE();
	void SetNextScene_END();

	void QuitGame();
};