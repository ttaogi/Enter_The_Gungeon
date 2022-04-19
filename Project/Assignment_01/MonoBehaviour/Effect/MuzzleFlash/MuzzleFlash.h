#pragma once

#include "../../MonoBehaviour.h"
#include "../../../Utility/EnumClasses.h"

class MuzzleFlash : public MonoBehaviour {
private:
	DIRECTION dir;
	void Update(HWND hWnd) {}
public:
	MuzzleFlash(POINT _pos, DIRECTION _dir);
	~MuzzleFlash();

	void Init();
	void Update(HWND hWnd, SceneManager* _scnMgr);
	void LateUpdate();
	void Render(HDC _hdc);
	void Render(HDC _hdc, POINT _coordinateRevision);
};