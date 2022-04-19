#pragma once

#include <Windows.h>
#include <iostream>

inline RECT MakeRectCWH(POINT pos, int width, int height) {
	return RECT{ pos.x - width / 2, pos.y - height / 2, pos.x + width / 2, pos.y + height / 2 };
}

inline void PaintRect(HDC hdc, RECT rc) {
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}



inline void PrintRect(RECT rc) {
	printf("%04d\t%04d\t%04d\t%04d\n", rc.left, rc.top, rc.right, rc.bottom);
}

inline void PrintPos(POINT pos) {
	printf("%04d\t%04d\n", pos.x, pos.y);
}


#pragma warning(push)
#pragma warning(disable: 4244)
inline void WStringToString(std::string& str, const std::wstring& wstr) {
	str.assign(wstr.begin(), wstr.end());
}
#pragma warning(pop)

