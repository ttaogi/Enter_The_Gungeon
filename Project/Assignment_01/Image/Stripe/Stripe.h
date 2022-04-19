#pragma once

#include "../Image.h"

class Stripe : private Image {
private:
	int frameIdx;
	int maxFrameIdx;
	POINT coordinateRevision;
public:
	Stripe();
	~Stripe();

	HRESULT Init(
		const wchar_t* _fileName,
		int _width, int _height, int _frameWidth, int _frameHeight,
		int _maxFrameX, int _maxFrameY, int _maxFrameIdx,
		POINT _coordinateRevision,
		bool _isTrans = FALSE, COLORREF _transColor = RGB(0, 0, 0)
	);
	void Reset() { frameIdx = 0; }
	void Release();
	void FrameRender(HDC _hdc, POINT _pos, bool _isLeft);
	void FrameRender(HDC _hdc, POINT _pos, bool _isLeft, POINT _coordinateRevision);

	bool IsEnd() const { return (frameIdx == maxFrameIdx); }
};
