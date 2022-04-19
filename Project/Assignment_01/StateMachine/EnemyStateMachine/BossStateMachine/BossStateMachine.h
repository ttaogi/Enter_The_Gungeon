#pragma once

#include "../EnemyStateMachine.h"

#define BOSS_SHOT_COOL	48
#define BULLET_COUNT	4
#define PATTERN_COUNT	3

class BossStateMachine : public StateMachine {
private:
	int shotCool;
	int bulletCount;
	int patternCount;

	void Update(MonoBehaviour* _mb, SceneManager* _scnMgr) {}
public:
	BossStateMachine();
	~BossStateMachine();

	void Update(MonoBehaviour* _mb, SceneManager* _scnMgr, Room* _room);

	void SetState(MonoBehaviour* _mb, CHARACTER_STATE _state);
	void SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack);
};