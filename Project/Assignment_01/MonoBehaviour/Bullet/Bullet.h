#pragma once

#include "../MonoBehaviour.h"

#define BULLET_WIDTH	24
#define BULLET_HEIGHT	24
#define BULLET_SPEED	30.0f

class Bullet : public MonoBehaviour {
private:
	bool byPlayer;
	int dmg;
	void Update(HWND hWnd) {}
public:
	Bullet(F_POINT _pos, VELOCITY _vel, bool _byPlayer, int _dmg = 10);
	~Bullet();

	void Init();
	void Update(HWND hWnd, SceneManager* _scnMgr);
	void LateUpdate();
	void Render(HDC _hdc);
	void Render(HDC _hdc, POINT _coordinateRevision);

	bool GetByPlayer() const { return byPlayer; }
};