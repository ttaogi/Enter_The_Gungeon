#include "Stdafx/stdafx.h"

#include "Animator.h"

Animator::Animator() {
	for (int i = 0; i < (int)CHARACTER_STATE::STATE_NUM; ++i) animations[i] = NULL;
	curState = CHARACTER_STATE::IDLE_BOTTOM;
}

void Animator::Release() {
	for (int i = 0; i < (int)CHARACTER_STATE::STATE_NUM; ++i) {
		SAFE_RELEASE(animations[i]);
		SAFE_DELETE(animations[i]);
	}
}

void Animator::AddAnimation(CHARACTER_STATE _state, Animation* _animation) {
	SAFE_RELEASE(animations[(int)_state]);
	SAFE_DELETE(animations[(int)_state]);
	animations[(int)_state] = _animation;
}

bool Animator::ChangeAnimation(CHARACTER_STATE _state) {
	if (animations[(int)_state] != NULL) {
		curState = _state;
		animations[(int)_state]->Reset();
		return true;
	}
	else {
		curState = CHARACTER_STATE::IDLE_BOTTOM;
		if (animations[(int)CHARACTER_STATE::IDLE_BOTTOM] != NULL)
			animations[(int)CHARACTER_STATE::IDLE_BOTTOM]->Reset();
		return false;
	}
}

void Animator::AnimationRender(HDC _hdc, POINT _pos, bool _isLeft) {
	if (animations[(int)curState] != NULL) {
		animations[(int)curState]->AnimationRender(_hdc, _pos, _isLeft);
	}
}

void Animator::AnimationRender(
	HDC _hdc, POINT _pos, bool _isLeft,
	POINT _coordinateRevision
) {
	if (animations[(int)curState] != NULL) {
		animations[(int)curState]->AnimationRender(_hdc, _pos, _isLeft, _coordinateRevision);
	}
}

bool Animator::IsEnd() const {
	if (animations[(int)curState] != NULL) {
		return animations[(int)curState]->IsEnd();
	}
	return true;
}
