#pragma once

#include "../MonoBehaviour.h"
#include "../../Map/Room/Room.h"
#include "../../StateMachine/EnemyStateMachine/EnemyStateMachine.h"

#define ENEMY_RECT_WIDTH		64
#define ENEMY_RECT_HEIGHT		80
#define ENEMY_INITIAL_HP		2
#define ENEMY_ATTACK_RANGE		(3 * TILE_SIZE)

class Enemy : public MonoBehaviour {
protected:
	int hp;
	int hpMax;
	Image* shadow;
	Room* room;
	Animator* gunAnimator;

	Enemy() {}
	void Update(HWND hWnd) { }
public:
	Enemy(Room* _room, RECT _area);
	~Enemy();

	void Init();
	void Update(HWND hWnd, SceneManager* _scnMgr);
	void LateUpdate();
	void Render(HDC _hdc);
	void Render(HDC _hdc, POINT _coordinateRevision);

	int GetHp() const { return hp; }
	void SetHp(int _hp) { hp = _hp; if (hp > hpMax) hp = hpMax; if (hp < 0) hp = 0; }
	int GetHpMax() const { return hpMax; }
	void SetHpMax(int _hpMax) { hpMax = _hpMax; }
	CHARACTER_STATE GetState() const { return stateMachine->GetState(); }
	Animator* GetGunAnimator() { return gunAnimator; }

	virtual void GetAttacked();
	void SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack);
};