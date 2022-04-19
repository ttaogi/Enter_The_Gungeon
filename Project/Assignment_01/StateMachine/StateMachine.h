#pragma once

#include <functional>

#include "../Image/Animator/Animator.h"

class MonoBehaviour;
class Room;
class SceneManager;

class StateMachine {
protected:
	std::function<void(MonoBehaviour*)> callBack;
	CHARACTER_STATE curState;
	bool isLeft; // default = left(true)
public:
	StateMachine() { curState = CHARACTER_STATE::IDLE_BOTTOM; isLeft = true; callBack = NULL; }

	virtual void Update(MonoBehaviour* mb, SceneManager* sMgr) = 0;
	virtual void Update(MonoBehaviour* _mb, SceneManager* _scnMgr, Room* _room) = 0;

	virtual void SetState(MonoBehaviour* _mb, CHARACTER_STATE _state) = 0;
	CHARACTER_STATE GetState() const { return curState; }
	bool GetIsLeft() const { return isLeft; }
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
	virtual void SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack) = 0;
};