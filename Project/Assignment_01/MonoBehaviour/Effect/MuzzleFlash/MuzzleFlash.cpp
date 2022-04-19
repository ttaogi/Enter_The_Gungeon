#include "Stdafx/stdafx.h"

#include "MuzzleFlash.h"

MuzzleFlash::MuzzleFlash(POINT _pos, DIRECTION _dir) {
	type = MONOBEHAVIOUR_TYPE::MUZZLE_FLASH;
	pos = _pos;
	stripe = NULL;
	animator = new Animator();
	stateMachine = NULL;
	destroy = false;
	dir = _dir;
}

MuzzleFlash::~MuzzleFlash() {
	SAFE_RELEASE(animator);
	SAFE_DELETE(animator);
	SAFE_DELETE(stateMachine);
}

void MuzzleFlash::Init() {
	Animation* anim = new Animation();
	switch(dir) {
	case DIRECTION::LEFT:
		anim->Init(
			MUZZLE_FLASH_LEFT_STRIPE,
			224, 56, 56, 56,
			4, 1, 4,
			POINT{ -56, -28 }, CHARACTER_STATE::IDLE_BOTTOM, false,
			true, RGB(255, 0, 255)
		);
		break;
	case DIRECTION::TOP:
		anim->Init(
			MUZZLE_FLASH_TOP_STRIPE,
			224, 56, 56, 56,
			4, 1, 4,
			POINT{ -28, -56 }, CHARACTER_STATE::IDLE_BOTTOM, false,
			true, RGB(255, 0, 255)
		);
		break;
	case DIRECTION::RIGHT:
		anim->Init(
			MUZZLE_FLASH_RIGHT_STRIPE,
			224, 56, 56, 56,
			4, 1, 4,
			POINT{ 0, -28 }, CHARACTER_STATE::IDLE_BOTTOM, false,
			true, RGB(255, 0, 255)
		);
		break;
	case DIRECTION::BOTTOM:
		anim->Init(
			MUZZLE_FLASH_BOTTOM_STRIPE,
			224, 56, 56, 56,
			4, 1, 4,
			POINT{ -28, 0 }, CHARACTER_STATE::IDLE_BOTTOM, false,
			true, RGB(255, 0, 255)
		);
		break;
	}
	animator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, anim);
}

void MuzzleFlash::Update(HWND hWnd, SceneManager * _scnMgr) { }

void MuzzleFlash::LateUpdate() { }

void MuzzleFlash::Render(HDC _hdc) {
	if (animator != NULL && destroy == false) {
		animator->AnimationRender(_hdc, pos.ToPoint(), true);
		if (animator->IsEnd() == true)
			destroy = true;
	}
}

void MuzzleFlash::Render(HDC _hdc, POINT _coordinateRevision) {
	if (animator != NULL && destroy == false) {
		animator->AnimationRender(_hdc, pos.ToPoint(), true, _coordinateRevision);
		if (animator->IsEnd() == true)
			destroy = true;
	}
}
