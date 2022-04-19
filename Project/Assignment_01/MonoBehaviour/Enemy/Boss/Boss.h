#pragma once

#include "../Enemy.h"
#include "../../../StateMachine/EnemyStateMachine/BossStateMachine/BossStateMachine.h"

#define BOSS_RECT_WIDTH		64
#define BOSS_RECT_HEIGHT	80
#define BOSS_INITIAL_HP		40
#define BOSS_ATTACK_RANGE	(9 * TILE_SIZE)

class Boss : public Enemy {
private:
public:
	Boss(Room* _room, RECT _area);
	~Boss();

	void Init();
	void Update(HWND hWnd, SceneManager* _scnMgr);
	void LateUpdate();
	void Render(HDC _hdc);
	void Render(HDC _hdc, POINT _coordinateRevision);

	void GetAttacked();
};