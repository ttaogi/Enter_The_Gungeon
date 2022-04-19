#pragma once

#include <map>
#include <string>
#include <Windows.h>

#include "../../DesignPattern/SingletonBase/SingletonBase.h"
#include "../../Image/Image.h"

typedef std::map<std::wstring, Image*> MapImageList;
typedef std::map<std::wstring, Image*>::iterator MapImageIter;

class ImageManager : public SingletonBase<ImageManager> {
private:
	MapImageList mImageList;
public:
	HRESULT init();
	void Release();

	Image* AddImage(std::wstring strKey, int width, int height);
	Image* AddImage(
		std::wstring strKey, const wchar_t* fileName,
		int width, int height,
		bool isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0)
	);
	Image* AddImage(
		std::wstring strKey, const wchar_t* fileName,
		float x, float y, int width, int height,
		bool isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0)
	);
	Image* AddFrameImage(
		std::wstring strKey, const wchar_t* fileName,
		int width, int height, int maxFrameX, int maxFrameY,
		bool isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0)
	);
	Image* AddFrameImage(
		std::wstring strKey, const wchar_t* fileName,
		float x, float y, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0)
	);

	Image* FindImage(std::wstring strKey);
	bool DeleteImage(std::wstring strKey);
	bool DeleteAll();

	void Render(std::wstring strKey, HDC hdc);
	void Render(std::wstring strKey, HDC hdc, int destX, int destY);
	void Render(std::wstring strKey, HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight);

	void AlphaRender(std::wstring strKey, HDC hdc, BYTE alpha);
	void AlphaRender(std::wstring strKey, HDC hdc, int destX, int destY, BYTE alpha);
	void AlphaRender(std::wstring strKey, HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha);

	void FrameRender(std::wstring strKey, HDC hdc, int destX, int destY);
	void FrameRender(std::wstring strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	void LoopRender(std::wstring strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);
	void LoopAlphaRender(std::wstring strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha);
};