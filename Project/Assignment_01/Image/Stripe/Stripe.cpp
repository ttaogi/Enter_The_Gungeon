#include "Stdafx/stdafx.h"

#include "Stripe.h"

Stripe::Stripe() : Image() { }

Stripe::~Stripe() {
	Release();
}

HRESULT Stripe::Init(
	const wchar_t* _fileName,
	int _width, int _height, int _frameWidth, int _frameHeight,
	int _maxFrameX, int _maxFrameY, int _maxFrameIdx,
	POINT _coordinateRevision,
	bool _isTrans, COLORREF _transColor
) {
	if (imageInfo != NULL) Release();

	HDC hdc = GetDC(_hWnd);

	imageInfo = new IMAGE_INFO();
	imageInfo->loadType = LOAD_FILE;
	imageInfo->resID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, _fileName, IMAGE_BITMAP,
		_width, _height, LR_LOADFROMFILE);
	imageInfo->hOBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);
	imageInfo->width = _width;
	imageInfo->height = _height;

	imageInfo->x = 0;
	imageInfo->y = 0;
	imageInfo->currentFrameX = 0;
	imageInfo->currentFrameY = 0;
	imageInfo->maxFrameX = _maxFrameX;
	imageInfo->maxFrameY = _maxFrameY;
	imageInfo->frameWidth = _frameWidth;
	imageInfo->frameHeight = _frameHeight;

	fileName = new wchar_t[wcslen(_fileName) + 1];
	wcscpy_s(fileName, wcslen(_fileName) + 1, _fileName);

	isTrans = _isTrans;
	transColor = _transColor;

	frameIdx = 0;
	maxFrameIdx = _maxFrameIdx;
	coordinateRevision = _coordinateRevision;

	if (imageInfo->hBit == 0) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

void Stripe::Release() { Image::Release(); }

void Stripe::FrameRender(HDC _hdc, POINT _pos, bool _isLeft) {
	if (frameIdx >= maxFrameIdx) frameIdx = maxFrameIdx - 1;
	int destX;
	int destY;
	if (_isLeft) {
		destX = _pos.x + coordinateRevision.x;
		destY = _pos.y + coordinateRevision.y;
	}
	else {
		destX = _pos.x - coordinateRevision.x - imageInfo->frameWidth;
		destY = _pos.y + coordinateRevision.y;
	}
	int srcX;
	int srcY;
	if (_isLeft) {
		srcX = (frameIdx % imageInfo->maxFrameX) * imageInfo->frameWidth;
		srcY = (frameIdx / imageInfo->maxFrameX) * imageInfo->frameHeight;
	}
	else {
		srcX = (frameIdx % imageInfo->maxFrameX) * imageInfo->frameWidth;
		srcY = (frameIdx / imageInfo->maxFrameX + imageInfo->maxFrameY)
			* imageInfo->frameHeight;
	}

	if (isTrans) {
		GdiTransparentBlt(
			_hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			srcX,
			srcY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			transColor
		);
	}
	else {
		BitBlt(
			_hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			srcX,
			srcY,
			SRCCOPY
		);
	}

	++frameIdx;
}

void Stripe::FrameRender(
	HDC _hdc, POINT _pos, bool _isLeft,
	POINT _coordinateRevision
) {
	if (frameIdx >= maxFrameIdx) frameIdx = maxFrameIdx - 1;
	int destX;
	int destY;
	if (_isLeft) {
		destX = _pos.x + coordinateRevision.x;
		destY = _pos.y + coordinateRevision.y;
	}
	else {
		destX = _pos.x - coordinateRevision.x - imageInfo->frameWidth;
		destY = _pos.y + coordinateRevision.y;
	}
	destX -= _coordinateRevision.x;
	destY -= _coordinateRevision.y;
	int srcX;
	int srcY;
	if (_isLeft) {
		srcX = (frameIdx % imageInfo->maxFrameX) * imageInfo->frameWidth;
		srcY = (frameIdx / imageInfo->maxFrameX) * imageInfo->frameHeight;
	}
	else {
		srcX = (frameIdx % imageInfo->maxFrameX) * imageInfo->frameWidth;
		srcY = (frameIdx / imageInfo->maxFrameX + imageInfo->maxFrameY)
			* imageInfo->frameHeight;
	}

	if (isTrans) {
		GdiTransparentBlt(
			_hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			srcX,
			srcY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			transColor
		);
	}
	else {
		BitBlt(
			_hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			srcX,
			srcY,
			SRCCOPY
		);
	}

	++frameIdx;
}
