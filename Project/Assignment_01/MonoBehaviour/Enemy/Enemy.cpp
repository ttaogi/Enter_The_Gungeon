#include "Stdafx/stdafx.h"

#include "Enemy.h"

Enemy::Enemy(Room* _room, RECT _area) {
	type = MONOBEHAVIOUR_TYPE::ENEMY;
	pos = F_POINT{
		RND->GetFloatFromTo((float)_area.left + TILE_SIZE, (float)_area.right - TILE_SIZE),
		RND->GetFloatFromTo((float)_area.top + TILE_SIZE, (float)_area.bottom - TILE_SIZE)
	};
	vel = VELOCITY{ 0.0f, 0.0f };
	rc = MakeRectCWH(pos.ToPoint(), ENEMY_RECT_WIDTH, ENEMY_RECT_HEIGHT);
	stripe = NULL;
	animator = new Animator();
	stateMachine = new EnemyStateMachine();
	destroy = false;

	hp = ENEMY_INITIAL_HP;
	hpMax = ENEMY_INITIAL_HP;
	shadow = IMG->FindImage(KEY_ENEMY_SHADOW);
	room = _room;
	gunAnimator = new Animator();
}

Enemy::~Enemy() {
	SAFE_RELEASE(animator);
	SAFE_DELETE(animator);
	SAFE_RELEASE(gunAnimator);
	SAFE_DELETE(gunAnimator);
	SAFE_DELETE(stateMachine);
}

void Enemy::Init() {
#pragma region		character_animation
	// idle.
	Animation* idleLeft = new Animation();
	idleLeft->Init(
		ENEMY_IDLE_LEFT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_LEFT, idleLeft);

	Animation* idleTop = new Animation();
	idleTop->Init(
		ENEMY_IDLE_TOP_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_TOP, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_TOP, idleTop);

	Animation* idleRight = new Animation();
	idleRight->Init(
		ENEMY_IDLE_RIGHT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_RIGHT, idleRight);

	Animation* idleBottom = new Animation();
	idleBottom->Init(
		ENEMY_IDLE_BOTTOM_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, idleBottom);

	// move.
	Animation* moveLeft = new Animation();
	moveLeft->Init(
		ENEMY_MOVE_LEFT_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_LEFT, moveLeft);

	Animation* moveTop = new Animation();
	moveTop->Init(
		ENEMY_MOVE_TOP_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_TOP, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_TOP, moveTop);

	Animation* moveRight = new Animation();
	moveRight->Init(
		ENEMY_MOVE_RIGHT_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_RIGHT, moveRight);

	Animation* moveBottom = new Animation();
	moveBottom->Init(
		ENEMY_MOVE_BOTTOM_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_BOTTOM, moveBottom);
#pragma endregion	character_animation

#pragma region		gun_animation
	// idle.
	Animation* gunIdleLeft = new Animation();
	gunIdleLeft->Init(
		ENEMY_GUN_IDLE_LEFT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_LEFT, gunIdleLeft);

	Animation* gunIdleTop = new Animation();
	gunIdleTop->Init(
		ENEMY_GUN_IDLE_TOP_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_TOP, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_TOP, gunIdleTop);

	Animation* gunIdleRight = new Animation();
	gunIdleRight->Init(
		ENEMY_GUN_IDLE_RIGHT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_RIGHT, gunIdleRight);

	Animation* gunIdleBottom = new Animation();
	gunIdleBottom->Init(
		ENEMY_GUN_IDLE_BOTTOM_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, gunIdleBottom);

	// move.
	Animation* gunMoveLeft = new Animation();
	gunMoveLeft->Init(
		ENEMY_GUN_MOVE_LEFT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_LEFT, gunMoveLeft);

	Animation* gunMoveTop = new Animation();
	gunMoveTop->Init(
		ENEMY_GUN_MOVE_TOP_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_TOP, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_TOP, gunMoveTop);

	Animation* gunMoveRight = new Animation();
	gunMoveRight->Init(
		ENEMY_GUN_MOVE_RIGHT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_RIGHT, gunMoveRight);

	Animation* gunMoveBottom = new Animation();
	gunMoveBottom->Init(
		ENEMY_GUN_MOVE_BOTTOM_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_BOTTOM, gunMoveBottom);
#pragma endregion	gun_animation
}

void Enemy::Update(HWND hWnd, SceneManager * _scnMgr) {
	if (stateMachine != NULL && destroy == false)
		stateMachine->Update(this, _scnMgr, room);
}

void Enemy::LateUpdate() {  }

void Enemy::Render(HDC _hdc) {
	if (shadow != NULL) shadow->Render(_hdc, pos.ToPoint().x, pos.ToPoint().y);
	if (animator != NULL && stateMachine != NULL)
		animator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft());
}

void Enemy::Render(HDC _hdc, POINT _coordinateRevision) {
	if (shadow != NULL)
		shadow->Render(
			_hdc,
			pos.ToPoint().x - _coordinateRevision.x - ENEMY_RECT_WIDTH / 2,
			pos.ToPoint().y - _coordinateRevision.y - ENEMY_RECT_HEIGHT / 2
		);
	if (animator != NULL && stateMachine != NULL) {
		gunAnimator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft(), _coordinateRevision);
		animator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft(), _coordinateRevision);
	}
}

void Enemy::GetAttacked() {
	if(hp >= 1) {
		--hp;
		if (hp == 0) {
			destroy = true;
			room->CloseCountDecrease();
		}
	}
}

void Enemy::SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack) {
	stateMachine->SetCallBack_v_CB_pMb(_callBack);
}
