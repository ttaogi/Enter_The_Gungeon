#pragma once

#include <iostream>
#include <Windows.h>

#include "../../DesignPattern/SingletonBase/SingletonBase.h"

class FontManager : public SingletonBase<FontManager> {
public:
	FontManager() {};
	~FontManager() {};

	void drawText(
		HDC _hdc, int _destX, int _destY, wchar_t* _fontName,
		int _fontSize, int _fontWidth, wchar_t* _printString,
		int _length, COLORREF _color
	);
};