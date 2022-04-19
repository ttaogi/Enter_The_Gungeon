#pragma once

#include "../Player.h"

#define LIFE_COUNT	48

class PlayerSkill : public MonoBehaviour {
private:
	int lifeCount;
	Player* player;

	void Update(HWND hWnd) {}
public:
	PlayerSkill(Player* _player);
	~PlayerSkill();

	void Init();
	void Update(HWND hWnd, SceneManager* _scnMgr);
	void LateUpdate();
	void Render(HDC _hdc) {}
	void Render(HDC _hdc, POINT _coordinateRevision) {}
};