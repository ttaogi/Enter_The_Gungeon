#include "Stdafx/stdafx.h"

#include "Boss.h"

Boss::Boss(Room* _room, RECT _area) {
	type = MONOBEHAVIOUR_TYPE::BOSS;
	pos = F_POINT{
		(float)(_area.left + _area.right) / 2,
		(float)(_area.top + _area.bottom) / 2
	};
	vel = VELOCITY{ 0.0f, 0.0f };
	rc = MakeRectCWH(pos.ToPoint(), BOSS_RECT_WIDTH, BOSS_RECT_HEIGHT);
	stripe = NULL;
	animator = new Animator();
	stateMachine = new BossStateMachine();
	destroy = false;

	hp = BOSS_INITIAL_HP;
	hpMax = BOSS_INITIAL_HP;
	shadow = IMG->FindImage(KEY_BOSS_SHADOW);
	room = _room;
	gunAnimator = new Animator();
}

Boss::~Boss() {
	SAFE_RELEASE(animator);
	SAFE_DELETE(animator);
	SAFE_RELEASE(gunAnimator);
	SAFE_DELETE(gunAnimator);
	SAFE_DELETE(stateMachine);
}

void Boss::Init(){
#pragma region		character_animation
	// idle.
	Animation* idleLeft = new Animation();
	idleLeft->Init(
		BOSS_IDLE_LEFT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_LEFT, idleLeft);

	Animation* idleTop = new Animation();
	idleTop->Init(
		BOSS_IDLE_TOP_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_TOP, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_TOP, idleTop);

	Animation* idleRight = new Animation();
	idleRight->Init(
		BOSS_IDLE_RIGHT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_RIGHT, idleRight);

	Animation* idleBottom = new Animation();
	idleBottom->Init(
		BOSS_IDLE_BOTTOM_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, idleBottom);

	// move.
	Animation* moveLeft = new Animation();
	moveLeft->Init(
		BOSS_MOVE_LEFT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::MOVE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_LEFT, moveLeft);

	Animation* moveTop = new Animation();
	moveTop->Init(
		BOSS_MOVE_TOP_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::MOVE_TOP, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_TOP, moveTop);

	Animation* moveRight = new Animation();
	moveRight->Init(
		BOSS_MOVE_RIGHT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::MOVE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_RIGHT, moveRight);

	Animation* moveBottom = new Animation();
	moveBottom->Init(
		BOSS_MOVE_BOTTOM_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::MOVE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_BOTTOM, moveBottom);
#pragma endregion	character_animation

#pragma region		gun_animation
	// idle.
	Animation* gunIdleLeft = new Animation();
	gunIdleLeft->Init(
		BOSS_GUN_IDLE_LEFT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_LEFT, gunIdleLeft);

	Animation* gunIdleTop = new Animation();
	gunIdleTop->Init(
		BOSS_GUN_IDLE_TOP_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_TOP, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_TOP, gunIdleTop);

	Animation* gunIdleRight = new Animation();
	gunIdleRight->Init(
		BOSS_GUN_IDLE_RIGHT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_RIGHT, gunIdleRight);

	Animation* gunIdleBottom = new Animation();
	gunIdleBottom->Init(
		BOSS_GUN_IDLE_BOTTOM_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, gunIdleBottom);

	// move.
	Animation* gunMoveLeft = new Animation();
	gunMoveLeft->Init(
		BOSS_GUN_MOVE_LEFT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_LEFT, gunMoveLeft);

	Animation* gunMoveTop = new Animation();
	gunMoveTop->Init(
		BOSS_GUN_MOVE_TOP_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_TOP, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_TOP, gunMoveTop);

	Animation* gunMoveRight = new Animation();
	gunMoveRight->Init(
		BOSS_GUN_MOVE_RIGHT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_RIGHT, gunMoveRight);

	Animation* gunMoveBottom = new Animation();
	gunMoveBottom->Init(
		BOSS_GUN_MOVE_BOTTOM_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_BOTTOM, gunMoveBottom);
#pragma endregion	gun_animation
}

void Boss::Update(HWND hWnd, SceneManager* _scnMgr) {
	if (stateMachine != NULL && destroy == false)
		stateMachine->Update(this, _scnMgr, room);
}

void Boss::LateUpdate() { }

void Boss::Render(HDC _hdc) { }

void Boss::Render(HDC _hdc, POINT _coordinateRevision) {
	if (shadow != NULL)
		shadow->Render(
			_hdc,
			pos.ToPoint().x - _coordinateRevision.x - BOSS_RECT_WIDTH / 2,
			pos.ToPoint().y - _coordinateRevision.y - BOSS_RECT_HEIGHT / 2
		);
	if (animator != NULL && stateMachine != NULL) {
		gunAnimator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft(), _coordinateRevision);
		animator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft(), _coordinateRevision);
	}
}

void Boss::GetAttacked() {
	if (hp >= 1) {
		--hp;
		if (hp == 0) {
			destroy = true;
			room->GameClear();
		}
	}
}
