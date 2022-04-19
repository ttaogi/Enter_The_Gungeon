#pragma once

#include "../StateMachine.h"

#define ENEMY_SHOT_COOL	24

DIRECTION SearchRoute(Room* _room, int _width, int _height, POINT _enemyPos, POINT _playerPos);

class EnemyStateMachine : public StateMachine {
private:
	int shotCool;

	void Update(MonoBehaviour* _mb, SceneManager* _scnMgr) {}
public:
	EnemyStateMachine();
	~EnemyStateMachine();

	void Update(MonoBehaviour* _mb, SceneManager* _scnMgr, Room* _room);

	void SetState(MonoBehaviour* _mb, CHARACTER_STATE _state);
	void SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack);
};