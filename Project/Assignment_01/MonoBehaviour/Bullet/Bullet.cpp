#include "Stdafx/stdafx.h"

#include "Bullet.h"

Bullet::Bullet(F_POINT _pos, VELOCITY _vel, bool _byPlayer, int _dmg) {
	type = MONOBEHAVIOUR_TYPE::BULLET;
	pos = _pos;
	rc = MakeRectCWH(pos.ToPoint(), BULLET_WIDTH, BULLET_HEIGHT);
	stripe = NULL;
	vel = _vel;
	byPlayer = _byPlayer;
	dmg = _dmg;
}

Bullet::~Bullet() { }

void Bullet::Init() {
	stripe = IMG->FindImage(KEY_BULLET_STRIPE);
}

void Bullet::Update(HWND hWnd, SceneManager * _scnMgr) {
	Move();
}

void Bullet::LateUpdate() { }

void Bullet::Render(HDC _hdc) {
	if (stripe != NULL)
		stripe->Render(_hdc, (int)pos.x - BULLET_WIDTH / 2, (int)pos.y - BULLET_HEIGHT / 2);
}

void Bullet::Render(HDC _hdc, POINT _coordinateRevision) {
	if(stripe != NULL)
		stripe->Render(_hdc,
			(int)pos.x - BULLET_WIDTH / 2 - _coordinateRevision.x,
			(int)pos.y - BULLET_HEIGHT / 2 - _coordinateRevision.y
		);
}
