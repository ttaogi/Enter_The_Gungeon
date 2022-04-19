#pragma once

#include "../Stripe/Stripe.h"

enum class CHARACTER_STATE {
	IDLE_LEFT = 0, IDLE_TOP,
	IDLE_RIGHT, IDLE_BOTTOM,
	MOVE_LEFT, MOVE_TOP,
	MOVE_RIGHT, MOVE_BOTTOM,
	DODGE_LEFT, DODGE_TOP,
	DODGE_RIGHT, DODGE_BOTTOM,
	DODGE_LEFT_TOP, DODGE_LEFT_BOTTOM,
	DODGE_RIGHT_TOP, DODGE_RIGHT_BOTTOM,
	DEAD,
	STATE_NUM
};

std::wstring StateToWString(CHARACTER_STATE _state);

class Animation {
private:
	Stripe stripe;
	CHARACTER_STATE state;
	bool autoLoop;
public:
	Animation() {}
	~Animation() { Release(); }

	HRESULT Init(
		const wchar_t* _fileName,
		int _width, int _height, int _frameWidth, int _frameHeight,
		int _maxFrameX, int _maxFrameY, int _maxFrameIdx,
		POINT _coordinateRevision,
		CHARACTER_STATE _state, bool _autoLoop,
		bool _isTrans = FALSE, COLORREF _transColor = RGB(0, 0, 0)
	);

	void Reset() { stripe.Reset(); }

	void Release() { stripe.Release(); }

	void AnimationRender(HDC _hdc, POINT _pos, bool _isLeft);
	void AnimationRender(HDC _hdc, POINT _pos, bool _isLeft, POINT _coordinateRevision);

	bool IsEnd() const;
};