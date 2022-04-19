#include "Stdafx/stdafx.h"

#include "Button.h"

Button::Button(RECT _rc, wstring _str, Image* _stripe) {
	type = MONOBEHAVIOUR_TYPE::BUTTON;
	pos = POINT{ 0, 0 };
	rc = _rc;
	stripe = _stripe;
	destroy = false;

	callBack_v_CB_v = nullptr;
	str = _str;
}

Button::~Button() { }

void Button::Init() { }

void Button::Update(HWND hWnd) {
	if (!destroy) {
		if (_mouseClick) {
			if (PtInRect(&rc, _ptMouse)) {
				if (callBack_v_CB_v)
					callBack_v_CB_v();
			}
		}
	}
}

void Button::LateUpdate() { }

void Button::Render(HDC _hdc) {
	if (!destroy) {
		if(stripe == NULL) {
			PaintRect(_hdc, rc);
			TextOut(
				_hdc,
				(rc.left + rc.right) / 2 - 10 * (int)str.length() / 2,
				(rc.top + rc.bottom) / 2 - 10,
				str.c_str(),
				(int)str.length()
			);
		}
		else {
			stripe->Render(_hdc, rc.left, rc.top);
		}
	}
}

void Button::Render(HDC _hdc, POINT _coordinateRevision) {
	if (!destroy) {
		RECT revisedRc{
			rc.left - _coordinateRevision.x, rc.top - _coordinateRevision.y,
			rc.right - _coordinateRevision.x, rc.bottom - _coordinateRevision.y
		};
		if(stripe == NULL) {
			PaintRect(_hdc, revisedRc);
			TextOut(
				_hdc,
				(revisedRc.left + revisedRc.right) / 2 - 10 * (int)str.length() / 2,
				(revisedRc.top + revisedRc.bottom) / 2 - 10,
				str.c_str(),
				(int)str.length()
			);
		}
		else {
			stripe->Render(_hdc, revisedRc.left, revisedRc.top);
		}
	}
}

void Button::SetCallBack_v_CB_v(std::function<void()> _callBack) {
	callBack_v_CB_v = std::move(_callBack);
}
