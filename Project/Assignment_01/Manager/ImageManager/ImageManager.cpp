#include "Stdafx/stdafx.h"

#include "ImageManager.h"

HRESULT ImageManager::init() { return S_OK; }

void ImageManager::Release() { DeleteAll(); }

Image* ImageManager::AddImage(std::wstring strKey, int width, int height) {
	Image* img = FindImage(strKey);

	if (img) return img;

	img = new Image();
	if (FAILED(img->Init(width, height))) {
		SAFE_DELETE(img);
		return NULL;
	}

	mImageList.insert(pair<wstring, Image*>(strKey, img));

	return img;
}

Image* ImageManager::AddImage(
	wstring strKey, const wchar_t * fileName,
	int width, int height,
	bool isTrans, COLORREF transColor
) {
	Image* img = FindImage(strKey);
	if (img) return img;

	img = new Image();
	if (FAILED(
		img->Init(fileName, width, height, isTrans, transColor)
	)) {
		SAFE_DELETE(img);
		return NULL;
	}

	mImageList.insert(pair<wstring, Image*>(strKey, img));

	return img;
}

Image* ImageManager::AddImage(
	wstring strKey, const wchar_t * fileName,
	float x, float y, int width, int height,
	bool isTrans, COLORREF transColor
) {
	Image* img = FindImage(strKey);

	if (img) return img;

	img = new Image();
	if (FAILED(
		img->Init(fileName, x, y, width, height, isTrans, transColor)
	)) {
		SAFE_DELETE(img);
		return nullptr;
	}

	mImageList.insert(pair<wstring, Image*>(strKey, img));

	return img;
}

Image* ImageManager::AddFrameImage(
	wstring strKey, const wchar_t * fileName,
	int width, int height, int maxFrameX, int maxFrameY,
	bool isTrans, COLORREF transColor
) {
	Image* img = FindImage(strKey);

	if (img) return img;

	img = new Image();
	if (FAILED(
		img->Init(fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor)
	)) {
		SAFE_DELETE(img);
		return nullptr;
	}

	mImageList.insert(pair<wstring, Image*>(strKey, img));

	return img;
}

Image* ImageManager::AddFrameImage(
	wstring strKey, const wchar_t * fileName,
	float x, float y, int width, int height,
	int maxFrameX, int maxFrameY,
	bool isTrans, COLORREF transColor
) {
	Image* img = FindImage(strKey);

	if (img) return img;

	img = new Image();
	if (FAILED(
		img->Init(fileName, x, y, width, height, maxFrameX, maxFrameY, isTrans, transColor)
	)) {
		SAFE_DELETE(img);
		return nullptr;
	}

	mImageList.insert(pair<wstring, Image*>(strKey, img));

	return img;
}

Image* ImageManager::FindImage(std::wstring strKey) {
	if (mImageList.find(strKey) == mImageList.end())
		return nullptr;
	else
		return mImageList.find(strKey)->second;
}

bool ImageManager::DeleteImage(std::wstring strKey) {
	auto key = mImageList.find(strKey);

	if (key != mImageList.end()) {
		key->second->Release();
		SAFE_DELETE(key->second);
		mImageList.erase(key);
		return true;
	}

	return false;
}

bool ImageManager::DeleteAll() {
	MapImageIter iter = mImageList.begin();

	for (; iter != mImageList.end();) {
		if (iter->second) {
			iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = mImageList.erase(iter);
		}
		else {
			++iter;
		}
	}

	mImageList.clear();

	return true;
}

void ImageManager::Render(wstring strKey, HDC hdc) {
	Image* img = FindImage(strKey);
	if (img) img->Render(hdc);
}

void ImageManager::Render(wstring strKey, HDC hdc, int destX, int destY) {
	Image* img = FindImage(strKey);
	if (img) img->Render(hdc, destX, destY);
}

void ImageManager::Render(wstring strKey, HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight) {
	Image* img = FindImage(strKey);
	if (img) img->Render(hdc, destX, destY, srcX, srcY, srcWidth, srcHeight);
}

void ImageManager::AlphaRender(wstring strKey, HDC hdc, BYTE alpha) {
	Image* img = FindImage(strKey);
	if (img) img->AlphaRender(hdc, alpha);
}

void ImageManager::AlphaRender(wstring strKey, HDC hdc, int destX, int destY, BYTE alpha) {
	Image* img = FindImage(strKey);
	if (img) img->AlphaRender(hdc, destX, destY, alpha);

}

void ImageManager::AlphaRender(wstring strKey, HDC hdc, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BYTE alpha) {
	Image* img = FindImage(strKey);
	if (img) img->AlphaRender(hdc, destX, destY, srcX, srcY, srcWidth, srcHeight, alpha);
}

void ImageManager::FrameRender(wstring strKey, HDC hdc, int destX, int destY) {
	Image* img = FindImage(strKey);
	if (img) img->FrameRender(hdc, destX, destY);
}

void ImageManager::FrameRender(wstring strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY) {
	Image* img = FindImage(strKey);
	if (img) img->FrameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}

void ImageManager::LoopRender(std::wstring strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY) {
	Image* img = FindImage(strKey);
	if (img) img->LoopRender(hdc, drawArea, offsetX, offsetY);
}

void ImageManager::LoopAlphaRender(std::wstring strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha) {
	Image* img = FindImage(strKey);
	if (img) img->LoopAlphaRender(hdc, drawArea, offsetX, offsetY, alpha);
}
