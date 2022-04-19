#pragma once

#include "../Utility/EnumClasses.h"

typedef struct tagImage {
	DWORD	resID;
	HDC		hMemDC;
	HBITMAP	hBit;
	HBITMAP	hOBit;
	int		width;
	int		height;
	BYTE	loadType;

	int x, y;
	int currentFrameX, currentFrameY;
	int maxFrameX, maxFrameY;
	int frameWidth, frameHeight;

	tagImage() {
		resID = 0;
		hMemDC = NULL;
		hBit = NULL;
		hOBit = NULL;
		width = 0;
		height = 0;
		loadType = LOAD_RESOURCE;
		x = y = 0;
		currentFrameX = currentFrameY = 0;
		maxFrameX = maxFrameY = 0;
		frameWidth = frameHeight = 0;
	}
} IMAGE_INFO, * LPIMAGE_INFO;

class Image {
protected:
	LPIMAGE_INFO	imageInfo;
	wchar_t*		fileName;
	bool			isTrans;
	COLORREF		transColor;

	BLENDFUNCTION	blendFunc;
	LPIMAGE_INFO	blendImage;
public:
	Image();
	~Image();

	HRESULT Init(int width, int height);
	HRESULT Init(const DWORD resID, int width, int height,
		bool isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	HRESULT Init(const wchar_t* fileName, int width, int height,
		bool isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	HRESULT Init(const wchar_t* _fileName, float _x, float _y,
		int _width, int _height, BOOL _isTrans, COLORREF _transColor);
	HRESULT Init(const wchar_t* _fileName, int _width, int _height,
		int _maxFrameX, int _maxFrameY, BOOL _isTrans, COLORREF _transColor);
	HRESULT Init(const wchar_t* _fileName, float _x, float _y,
		int _width, int _height, int _maxFrameX, int _maxFrameY,
		bool _isTrans = FALSE, COLORREF _transColor = RGB(0, 0, 0));
	HRESULT InitForAlphaBlend();

	void SetTransColor(bool isTrans, COLORREF transColor);

	void Release();

	void Render(HDC hdc);
	void Render(HDC hdc, int destX, int destY);
	void Render(HDC hdc, int destX, int destY,
		int srcX, int srcY, int srcWidth, int srcHeight);
	void Render(HDC hdc, int destX, int destY,
		int destWidth, int destHeight);
	void AlphaRender(HDC hdc, BYTE alpha);
	void AlphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void AlphaRender(HDC hdc, int destX, int destY,
		int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha);

	void FrameRender(HDC hdc, int destX, int destY);
	void FrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	void LoopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);
	void LoopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha);

	inline HDC GetMemDC() { return imageInfo->hMemDC; }
	int GetWidth() const { if (imageInfo != NULL) return imageInfo->width; else return -1; }
	int GetHeight() const { if (imageInfo != NULL) return imageInfo->height; else return -1; }
};