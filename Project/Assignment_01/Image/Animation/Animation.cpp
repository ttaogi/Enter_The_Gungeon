#include "Stdafx/stdafx.h"

#include "Animation.h"

HRESULT Animation::Init(
	const wchar_t* _fileName,
	int _width, int _height, int _frameWidth, int _frameHeight,
	int _maxFrameX, int _maxFrameY, int _maxFrameIdx,
	POINT _coordinateRevision,
	CHARACTER_STATE _state, bool _autoLoop,
	bool _isTrans, COLORREF _transColor
) {
	if (
		FAILED(
			stripe.Init(
				_fileName, _width, _height, _frameWidth, _frameHeight,
				_maxFrameX, _maxFrameY, _maxFrameIdx,
				_coordinateRevision, _isTrans, _transColor
			)
		)
		) {
		return E_FAIL;
	}

	state = _state;
	autoLoop = _autoLoop;

	return S_OK;
}

void Animation::AnimationRender(HDC _hdc, POINT _pos, bool _isLeft) {
	stripe.FrameRender(_hdc, _pos, _isLeft);
	if (autoLoop && stripe.IsEnd()) stripe.Reset();
}

void Animation::AnimationRender(
	HDC _hdc, POINT _pos, bool _isLeft,
	POINT _coordinateRevision
) {
	stripe.FrameRender(_hdc, _pos, _isLeft, _coordinateRevision);
	if (autoLoop && stripe.IsEnd()) stripe.Reset();
}

bool Animation::IsEnd() const {
	if (autoLoop) return false;
	return stripe.IsEnd();
}

std::wstring StateToWString(CHARACTER_STATE _state) {
	switch (_state) {
	case CHARACTER_STATE::IDLE_LEFT:
		return L"IDLE_LEFT";
	case CHARACTER_STATE::IDLE_TOP:
		return L"IDLE_TOP";
	case CHARACTER_STATE::IDLE_RIGHT:
		return L"IDLE_RIGHT";
	case CHARACTER_STATE::IDLE_BOTTOM:
		return L"IDLE_BOTTOM";
	case CHARACTER_STATE::MOVE_LEFT:
		return L"MOVE_LEFT";
	case CHARACTER_STATE::MOVE_TOP:
		return L"MOVE_TOP";
	case CHARACTER_STATE::MOVE_RIGHT:
		return L"MOVE_RIGHT";
	case CHARACTER_STATE::MOVE_BOTTOM:
		return L"MOVE_BOTTOM";
	case CHARACTER_STATE::DODGE_LEFT:
		return L"DODGE_LEFT";
	case CHARACTER_STATE::DODGE_TOP:
		return L"DODGE_TOP";
	case CHARACTER_STATE::DODGE_RIGHT:
		return L"DODGE_RIGHT";
	case CHARACTER_STATE::DODGE_BOTTOM:
		return L"DODGE_BOTTOM";
	case CHARACTER_STATE::DODGE_LEFT_TOP:
		return L"DODGE_LEFT_TOP";
	case CHARACTER_STATE::DODGE_LEFT_BOTTOM:
		return L"DODGE_LEFT_BOTTOM";
	case CHARACTER_STATE::DODGE_RIGHT_TOP:
		return L"DODGE_RIGHT_TOP";
	case CHARACTER_STATE::DODGE_RIGHT_BOTTOM:
		return L"DODGE_RIGHT_BOTTOM";
	default:
		return L"STATE_NUM";
	}
}
