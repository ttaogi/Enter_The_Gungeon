#include "Stdafx/stdafx.h"

#include <string>

#include "Image.h"

// library for alpha blend.
#pragma comment (lib, "msimg32.lib")

Image::Image()
	: imageInfo(NULL), fileName(NULL), isTrans(FALSE),
	transColor(RGB(0, 0, 0)), blendImage(NULL)
{ }

Image::~Image() {
	Release();
}

HRESULT Image::Init(int _width, int _height) {
	if (imageInfo != NULL) Release();

	HDC hdc = GetDC(_hWnd);

	imageInfo = new IMAGE_INFO();
	imageInfo->loadType = LOAD_EMPTY;
	imageInfo->resID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);
	imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _width, _height);
	imageInfo->hOBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);
	imageInfo->width = _width;
	imageInfo->height = _height;

	fileName = NULL;
	isTrans = FALSE;
	transColor = RGB(0, 0, 0);

	if (imageInfo->hBit == NULL) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::Init(const DWORD _resID, int _width, int _height,
	bool _isTrans, COLORREF _transColor
) {
	if (imageInfo != NULL) Release();

	HDC hdc = GetDC(_hWnd);

	imageInfo = new IMAGE_INFO();
	imageInfo->loadType = LOAD_RESOURCE;
	imageInfo->resID = _resID;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);
	imageInfo->hBit = (HBITMAP)LoadBitmap(_hInstance, MAKEINTRESOURCE(imageInfo->resID));
	imageInfo->hOBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);
	imageInfo->width = _width;
	imageInfo->height = _height;

	fileName = NULL;
	isTrans = _isTrans;
	transColor = _transColor;

	if (imageInfo->hBit == 0) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::Init(const wchar_t* _fileName, int _width, int _height,
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

	fileName = new wchar_t[wcslen(_fileName) + 1];
	wcscpy_s(fileName, wcslen(_fileName) + 1, _fileName);

	isTrans = _isTrans;
	transColor = _transColor;

	if (imageInfo->hBit == 0) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::Init(const wchar_t* _fileName, float _x, float _y,
	int _width, int _height, BOOL _isTrans, COLORREF _transColor
) {
	if (imageInfo != NULL) this->Release();

	HDC hdc = GetDC(_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->loadType = LOAD_FILE;
	imageInfo->resID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, _fileName, IMAGE_BITMAP,
		_width, _height, LR_LOADFROMFILE);
	imageInfo->hOBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);
	imageInfo->x = (int)_x;
	imageInfo->y = (int)_y;
	imageInfo->width = _width;
	imageInfo->height = _height;

	fileName = new wchar_t[wcslen(_fileName) + 1];
	wcscpy_s(fileName, wcslen(_fileName) + 1, _fileName);

	isTrans = _isTrans;
	transColor = _transColor;

	if (imageInfo->hBit == 0) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}


HRESULT Image::Init(const wchar_t* _fileName, int _width, int _height,
	int _maxFrameX, int _maxFrameY, BOOL _isTrans, COLORREF _transColor
) {
	if (imageInfo != NULL) this->Release();

	HDC hdc = GetDC(_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->loadType = LOAD_FILE;
	imageInfo->resID = 0;
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, _fileName, IMAGE_BITMAP,
		_width, _height, LR_LOADFROMFILE);
	imageInfo->hOBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBit);
	imageInfo->width = _width;
	imageInfo->height = _height;
	imageInfo->currentFrameX = 0;
	imageInfo->currentFrameY = 0;
	imageInfo->maxFrameX = _maxFrameX;
	imageInfo->maxFrameY = _maxFrameY;
	imageInfo->frameWidth = _width / _maxFrameX;
	imageInfo->frameHeight = _height / _maxFrameY;

	fileName = new wchar_t[wcslen(_fileName) + 1];
	wcscpy_s(fileName, wcslen(_fileName) + 1, _fileName);

	isTrans = _isTrans;
	transColor = _transColor;

	if (imageInfo->hBit == 0) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::Init(const wchar_t* _fileName, float _x, float _y,
	int _width, int _height, int _maxFrameX, int _maxFrameY,
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
	imageInfo->x = (int)_x;
	imageInfo->y = (int)_y;
	imageInfo->width = _width;
	imageInfo->height = _height;
	imageInfo->currentFrameX = 0;
	imageInfo->currentFrameY = 0;
	imageInfo->maxFrameX = _maxFrameX;
	imageInfo->maxFrameY = _maxFrameY;
	imageInfo->frameWidth = _width / _maxFrameX;
	imageInfo->frameHeight = _height / _maxFrameY;

	fileName = new wchar_t[wcslen(_fileName) + 1];
	wcscpy_s(fileName, wcslen(_fileName) + 1, _fileName);

	isTrans = _isTrans;
	transColor = _transColor;

	if (imageInfo->hBit == 0) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT Image::InitForAlphaBlend() {
	if (imageInfo == NULL) return E_FAIL;

	HDC hdc = GetDC(_hWnd);

	blendFunc.BlendFlags = 0;
	blendFunc.AlphaFormat = 0;
	blendFunc.BlendOp = AC_SRC_OVER;

	blendImage = new IMAGE_INFO();
	blendImage->loadType = LOAD_FILE;
	blendImage->resID = 0;
	blendImage->hMemDC = CreateCompatibleDC(hdc);

	blendImage->hBit = CreateCompatibleBitmap(hdc, imageInfo->width, imageInfo->height);
	blendImage->hOBit = (HBITMAP)SelectObject(blendImage->hMemDC, blendImage->hBit);
	blendImage->width = WINSIZE_X;
	blendImage->height = WINSIZE_Y;

	if (blendImage->hBit == 0) { // fail to get resource.
		Release();
		ReleaseDC(_hWnd, hdc);
		return E_FAIL;
	}

	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

void Image::SetTransColor(bool _isTrans, COLORREF _transColor) {
	isTrans = _isTrans;
	transColor = _transColor;
}

void Image::Release() {
	if (imageInfo != NULL) {
		SelectObject(imageInfo->hMemDC, imageInfo->hOBit);
		DeleteObject(imageInfo->hBit);
		DeleteDC(imageInfo->hMemDC);

		SAFE_DELETE(imageInfo);
		SAFE_DELETE_ARRAY(fileName);

		isTrans = FALSE;
		transColor = RGB(0, 0, 0);
	}
	if (blendImage != NULL) {
		SelectObject(blendImage->hMemDC, blendImage->hOBit);
		DeleteObject(blendImage->hBit);
		DeleteDC(blendImage->hMemDC);

		SAFE_DELETE(blendImage);
	}
}

void Image::Render(HDC hdc) {
	if (isTrans) {
		GdiTransparentBlt(
			hdc, 0, 0,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			transColor
		);
	}
	else {
		BitBlt(
			hdc, 0, 0,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			SRCCOPY
		);
	}
}

void Image::Render(HDC hdc, int destX, int destY) {
	if (isTrans) {
		GdiTransparentBlt(
			hdc, destX, destY,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			transColor
		);
	}
	else {
		BitBlt(
			hdc, destX, destY,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			SRCCOPY
		);
	}
}

void Image::Render(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight) {
	if (isTrans) {
		GdiTransparentBlt(
			hdc, destX, destY,
			srcWidth,
			srcHeight,
			imageInfo->hMemDC,
			srcX, srcY,
			srcWidth,
			srcHeight,
			transColor
		);
	}
	else {
		BitBlt(
			hdc, destX, destY,
			srcWidth,
			srcHeight,
			imageInfo->hMemDC,
			srcX, srcY,
			SRCCOPY
		);
	}
}

void Image::Render(HDC hdc, int destX, int destY, int destWidth, int destHeight) {
	GdiTransparentBlt(
		hdc, destX, destY,
		destWidth,
		destHeight,
		imageInfo->hMemDC,
		0, 0,
		imageInfo->width,
		imageInfo->height,
		transColor
	);
}

void Image::AlphaRender(HDC hdc, BYTE alpha) {
	if (!blendImage) InitForAlphaBlend();

	blendFunc.SourceConstantAlpha = alpha;

	if (isTrans) {
		BitBlt(
			blendImage->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			hdc,
			0, 0,
			SRCCOPY
		);
		GdiTransparentBlt(
			blendImage->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			transColor
		);
		AlphaBlend(
			hdc,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			blendImage->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			blendFunc
		);
	}
	else {
		AlphaBlend(
			hdc,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			blendFunc
		);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, BYTE alpha) {
	if (!blendImage) InitForAlphaBlend();

	blendFunc.SourceConstantAlpha = alpha;

	if (isTrans) {
		BitBlt(
			blendImage->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			hdc,
			destX, destY,
			SRCCOPY
		);
		GdiTransparentBlt(
			blendImage->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			transColor
		);
		AlphaBlend(
			hdc,
			destX, destY,
			imageInfo->width,
			imageInfo->height,
			blendImage->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			blendFunc
		);
	}
	else {
		AlphaBlend(
			hdc,
			destX, destY,
			imageInfo->width,
			imageInfo->height,
			imageInfo->hMemDC,
			0, 0,
			imageInfo->width,
			imageInfo->height,
			blendFunc
		);
	}
}

void Image::AlphaRender(HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha) {
	if (!blendImage) InitForAlphaBlend();

	blendFunc.SourceConstantAlpha = alpha;

	if (isTrans) {
		BitBlt(
			blendImage->hMemDC,
			destX, destY,
			srcWidth,
			srcHeight,
			hdc,
			destX, destY,
			SRCCOPY
		);
		GdiTransparentBlt(
			blendImage->hMemDC,
			destX, destY,
			srcWidth,
			srcHeight,
			imageInfo->hMemDC,
			srcX, srcY,
			srcWidth,
			srcHeight,
			transColor
		);
		AlphaBlend(
			hdc,
			destX, destY,
			srcWidth,
			srcHeight,
			blendImage->hMemDC,
			destX, destY,
			srcWidth,
			srcHeight,
			blendFunc
		);
	}
	else {
		BitBlt(
			hdc, destX, destY,
			srcWidth,
			srcHeight,
			imageInfo->hMemDC,
			srcX, srcY,
			SRCCOPY
		);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY) {
	if (isTrans) {
		GdiTransparentBlt(
			hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			imageInfo->currentFrameX * imageInfo->frameWidth,
			imageInfo->currentFrameY * imageInfo->frameHeight,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			transColor
		);
	}
	else {
		BitBlt(
			hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			imageInfo->currentFrameX * imageInfo->frameWidth,
			imageInfo->currentFrameY * imageInfo->frameHeight,
			SRCCOPY
		);
	}
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY) {
	imageInfo->currentFrameX = (currentFrameX < imageInfo->maxFrameX) ? (currentFrameX) : imageInfo->maxFrameX - 1;
	imageInfo->currentFrameY = (currentFrameY < imageInfo->maxFrameY) ? (currentFrameY) : imageInfo->maxFrameY - 1;

	if (isTrans) {
		GdiTransparentBlt(
			hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			imageInfo->currentFrameX * imageInfo->frameWidth,
			imageInfo->currentFrameY * imageInfo->frameHeight,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			transColor
		);
	}
	else {
		BitBlt(
			hdc, destX, destY,
			imageInfo->frameWidth,
			imageInfo->frameHeight,
			imageInfo->hMemDC,
			imageInfo->currentFrameX * imageInfo->frameWidth,
			imageInfo->currentFrameY * imageInfo->frameHeight,
			SRCCOPY
		);
	}
}

void Image::LoopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY) {
	// - offset value, revise.
	if (offsetX < 0) offsetX = imageInfo->width + (offsetX % imageInfo->width);
	if (offsetY < 0) offsetY = imageInfo->height + (offsetY % imageInfo->height);

	RECT rcSrc;
	int srcWidth;
	int srcHeight;

	RECT rcDest;

	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	for (int y = 0; y < drawAreaH; y += srcHeight) {
		rcSrc.top = (y + offsetY) % imageInfo->height;
		rcSrc.bottom = imageInfo->height;
		srcHeight = rcSrc.bottom - rcSrc.top;

		if (y + srcHeight > drawAreaH) {
			rcSrc.bottom -= (y + srcHeight) - drawAreaH;
			srcHeight = rcSrc.bottom - rcSrc.top;
		}

		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + srcHeight;

		for (int x = 0; x < drawAreaW; x += srcWidth) {
			rcSrc.left = (x + offsetX) % imageInfo->width;
			rcSrc.right = imageInfo->width;
			srcWidth = rcSrc.right - rcSrc.left;

			if (x + srcWidth > drawAreaW) {
				rcSrc.right -= (x + srcWidth) - drawAreaW;
				srcWidth = rcSrc.right - rcSrc.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + srcWidth;

			Render(
				hdc, rcDest.left, rcDest.top,
				rcSrc.left, rcSrc.top,
				srcWidth, srcHeight
			);
		}
	}
}

void Image::LoopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha) {
	// - offset value, revise.
	if (offsetX < 0) offsetX = imageInfo->width + (offsetX % imageInfo->width);
	if (offsetY < 0) offsetY = imageInfo->height + (offsetY % imageInfo->height);

	RECT rcSrc;
	int srcWidth;
	int srcHeight;

	RECT rcDest;

	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	for (int y = 0; y < drawAreaH; y += srcHeight) {
		rcSrc.top = (y + offsetY) % imageInfo->height;
		rcSrc.bottom = imageInfo->height;
		srcHeight = rcSrc.bottom - rcSrc.top;

		if (y + srcHeight > drawAreaH) {
			rcSrc.bottom -= (y + srcHeight) - drawAreaH;
			srcHeight = rcSrc.bottom - rcSrc.top;
		}

		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + srcHeight;

		for (int x = 0; x < drawAreaW; x += srcWidth) {
			rcSrc.left = (x + offsetX) % imageInfo->width;
			rcSrc.right = imageInfo->width;
			srcWidth = rcSrc.right - rcSrc.left;

			if (x + srcWidth > drawAreaW) {
				rcSrc.right -= (x + srcWidth) - drawAreaW;
				srcWidth = rcSrc.right - rcSrc.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + srcWidth;

			AlphaRender(
				hdc, rcDest.left, rcDest.top,
				rcSrc.left, rcSrc.top,
				srcWidth, srcHeight, alpha
			);
		}
	}
}
