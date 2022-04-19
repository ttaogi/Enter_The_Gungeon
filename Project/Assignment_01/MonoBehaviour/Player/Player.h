#pragma once

#include "../MonoBehaviour.h"
#include "../../StateMachine/PlayerStateMachine/PlayerStateMachine.h"

#define PLAYER_RECT_WIDTH		64
#define PLAYER_RECT_HEIGHT		80
#define PLAYER_INITIAL_HP		10
#define PLAYER_MAX_BULLET		8
#define DAMAGE_COOL				24

class Player : public MonoBehaviour {
private:
	int hp;
	int hpMax;
	int bulletCount;
	int damageCool;
	Image* shadow;
	Animator* gunAnimator;
	Animator* reloadAnimator;

	void Update(HWND hWnd) { }
public:
	Player();
	~Player();

	void Init();
	void Update(HWND hWnd, SceneManager* _scnMgr);
	void LateUpdate();
	void Render(HDC _hdc);
	void Render(HDC _hdc, POINT _coordinateRevision);
	void Render(HDC _hdc, RECT _destArea);

	int GetHp() const { return hp; }
	void SetHp(int _hp) { hp = _hp; if (hp > hpMax) hp = hpMax; if (hp < 0) hp = 0; }
	int GetHpMax() const { return hpMax; }
	void SetHpMax(int _hpMax) { hpMax = _hpMax; }
	int GetBulletCount() const { return bulletCount; }
	void SetBulletCount(int _bulletCount) { bulletCount = _bulletCount; }
	int GetSkillCool() const { return ((PlayerStateMachine*)stateMachine)->GetSkillCool(); }
	int GetSkillCoolMax() const { return ((PlayerStateMachine*)stateMachine)->GetSkillCoolMax(); }
	CHARACTER_STATE GetState() const { return stateMachine->GetState(); }
	Animator* GetGunAnimator() { return gunAnimator; }
	Animator* GetReloadAnimator() { return reloadAnimator; }

	void GetAttacked();
	void SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack);
};