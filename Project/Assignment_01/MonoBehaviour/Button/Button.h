#pragma once

#include <string>

#include "../MonoBehaviour.h"
#include "../../Image/Image.h"
#include "../../Manager/SceneManager/SceneManager.h"

class Button : public MonoBehaviour {
private:
	std::function<void()> callBack_v_CB_v;
	std::wstring str;
public:
	Button(RECT _rc = RECT{ 0, 0, 0, 0 }, std::wstring _str = L"", Image* _stripe = NULL);
	~Button();

	void Init();
	void Update(HWND hWnd);
	void Update(HWND hWnd, SceneManager* _scnMgr) {}
	void LateUpdate();
	void Render(HDC _hdc);
	void Render(HDC _hdc, POINT _coordinateRevision);

	void SetCallBack_v_CB_v(std::function<void()> _callBack);
};