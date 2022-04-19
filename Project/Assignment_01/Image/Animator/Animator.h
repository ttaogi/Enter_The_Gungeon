#pragma once

#include "../Animation/Animation.h"

class Animator {
private:
	Animation* animations[(unsigned int)CHARACTER_STATE::STATE_NUM];
	CHARACTER_STATE curState;
public:
	Animator();
	~Animator() { Release(); }

	void Init() {}
	void Release();

	CHARACTER_STATE GetCurState() const { return curState; }

	void AddAnimation(CHARACTER_STATE _state, Animation* _animation);
	bool ChangeAnimation(CHARACTER_STATE _state);
	void AnimationRender(HDC _hdc, POINT _pos, bool _isLeft);
	void AnimationRender(HDC _hdc, POINT _pos, bool _isLeft, POINT _coordinateRevision);

	bool IsEnd() const;
};