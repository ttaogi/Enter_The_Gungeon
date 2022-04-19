#pragma once

#include "../StateMachine.h"

#define PLAYER_SHOT_COOL	6
#define PLAYER_SKILL_COOL	120

class PlayerStateMachine : public StateMachine {
private:
	int shotCool;
	int skillCool;

	void Update(MonoBehaviour* _mb, SceneManager* _scnMgr, Room* _room) {}
public:
	PlayerStateMachine();
	~PlayerStateMachine();

	void Update(MonoBehaviour* _mb, SceneManager* _scnMgr);

	int GetSkillCool() const { return skillCool; }
	int GetSkillCoolMax() const { return PLAYER_SKILL_COOL; }
	void SetState(MonoBehaviour* _mb, CHARACTER_STATE _state);
	void SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack);
};