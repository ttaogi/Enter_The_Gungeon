#include "Stdafx/stdafx.h"

#include "FontManager.h"

void FontManager::drawText(
	HDC _hdc, int _destX, int _destY, wchar_t* _fontName,
	int _fontSize, int _fontWidth, wchar_t* _printString,
	int _length, COLORREF _color
) {
	// make background of text transparent.
	SetBkMode(_hdc, TRANSPARENT);

	HFONT font;
	HFONT oldFont;
	COLORREF oldColor;

	font = CreateFont(
		_fontSize, 0, 0, 5,							// height, width, angle, direction
		_fontWidth, false, false, 0,				// bold, italic, underline, cancel line
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,		// character set, output precision
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,		// clipping precision, output quality
		DEFAULT_PITCH | FF_SWISS, _fontName			// pitch(ja-gan), font
	);

	oldFont = (HFONT)SelectObject(_hdc, font);
	oldColor = GetTextColor(_hdc);
	SetTextColor(_hdc, _color);

	TextOut(_hdc, _destX, _destY, _printString, _length);

	SelectObject(_hdc, oldFont);
	DeleteObject(font);
	SetTextColor(_hdc, oldColor);
}
