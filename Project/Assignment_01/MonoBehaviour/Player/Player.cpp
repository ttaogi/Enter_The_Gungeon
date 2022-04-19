#include "Stdafx/stdafx.h"

#include "Player.h"

Player::Player() {
	type = MONOBEHAVIOUR_TYPE::PLAYER;
	pos = F_POINT{0, 0};
	vel = VELOCITY{ 0.0f, 0.0f };
	rc = MakeRectCWH(pos.ToPoint(), PLAYER_RECT_WIDTH, PLAYER_RECT_HEIGHT);
	stripe = NULL;
	animator = new Animator();
	stateMachine = new PlayerStateMachine();
	destroy = false;

	hp = PLAYER_INITIAL_HP;
	hpMax = PLAYER_INITIAL_HP;
	bulletCount = PLAYER_MAX_BULLET;
	damageCool = 0;
	shadow = IMG->FindImage(KEY_PLAYER_SHADOW);
	gunAnimator = new Animator();
	reloadAnimator = new Animator();
}

Player::~Player() {
	SAFE_RELEASE(animator);
	SAFE_DELETE(animator);
	SAFE_RELEASE(gunAnimator);
	SAFE_DELETE(gunAnimator);
	SAFE_RELEASE(reloadAnimator);
	SAFE_DELETE(reloadAnimator);
	SAFE_DELETE(stateMachine);
}

void Player::Init() {
#pragma region		character_animation
	// idle.
	Animation* idleLeft = new Animation();
	idleLeft->Init(
		PLAYER_IDLE_LEFT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_LEFT, idleLeft);

	Animation* idleTop = new Animation();
	idleTop->Init(
		PLAYER_IDLE_TOP_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_TOP, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_TOP, idleTop);

	Animation* idleRight = new Animation();
	idleRight->Init(
		PLAYER_IDLE_RIGHT_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_RIGHT, idleRight);

	Animation* idleBottom = new Animation();
	idleBottom->Init(
		PLAYER_IDLE_BOTTOM_STRIPE,
		256, 160, 64, 80,
		4, 2, 8,
		POINT{ -32, -40 }, CHARACTER_STATE::IDLE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, idleBottom);

	// move.
	Animation* moveLeft = new Animation();
	moveLeft->Init(
		PLAYER_MOVE_LEFT_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_LEFT, moveLeft);

	Animation* moveTop = new Animation();
	moveTop->Init(
		PLAYER_MOVE_TOP_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_TOP, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_TOP, moveTop);

	Animation* moveRight = new Animation();
	moveRight->Init(
		PLAYER_MOVE_RIGHT_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_RIGHT, moveRight);

	Animation* moveBottom = new Animation();
	moveBottom->Init(
		PLAYER_MOVE_BOTTOM_STRIPE,
		384, 184, 64, 92,
		6, 2, 12,
		POINT{ -32, -52 }, CHARACTER_STATE::MOVE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::MOVE_BOTTOM, moveBottom);

	// dodge - vertical.
	Animation* dodgeTop = new Animation();
	dodgeTop->Init(
		PLAYER_DODGE_TOP_STRIPE,
		468, 192, 52, 96,
		9, 2, 18,
		POINT{ -26, -56 }, CHARACTER_STATE::DODGE_TOP, false,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::DODGE_TOP, dodgeTop);

	Animation* dodgeBottom = new Animation();
	dodgeBottom->Init(
		PLAYER_DODGE_BOTTOM_STRIPE,
		468, 192, 52, 96,
		9, 2, 18,
		POINT{ -26, -56 }, CHARACTER_STATE::DODGE_BOTTOM, false,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::DODGE_BOTTOM, dodgeBottom);

	// dodge - horizontal.
	Animation* dodgeLeft = new Animation();
	dodgeLeft->Init(
		PLAYER_DODGE_LEFT_STRIPE,
		720, 184, 80, 92,
		9, 2, 18,
		POINT{ -48, -52 }, CHARACTER_STATE::DODGE_LEFT, false,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::DODGE_LEFT, dodgeLeft);

	Animation* dodgeRight = new Animation();
	dodgeRight->Init(
		PLAYER_DODGE_RIGHT_STRIPE,
		720, 184, 80, 92,
		9, 2, 18,
		POINT{ -32, -52 }, CHARACTER_STATE::DODGE_RIGHT, false,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::DODGE_RIGHT, dodgeRight);

	// dead.
	Animation* dead = new Animation();
	dead->Init(
		PLAYER_DEAD_STRIPE,
		64, 80, 64, 80,
		1, 1, 1,
		POINT{ -32, -40 }, CHARACTER_STATE::DEAD, true,
		true, RGB(255, 0, 255)
	);
	animator->AddAnimation(CHARACTER_STATE::DEAD, dead);
#pragma endregion	character_animation

#pragma region		gun_animation
	// idle.
	Animation* gunIdleLeft = new Animation();
	gunIdleLeft->Init(
		PLAYER_GUN_IDLE_LEFT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_LEFT, gunIdleLeft);

	Animation* gunIdleTop = new Animation();
	gunIdleTop->Init(
		PLAYER_GUN_IDLE_TOP_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_TOP, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_TOP, gunIdleTop);

	Animation* gunIdleRight = new Animation();
	gunIdleRight->Init(
		PLAYER_GUN_IDLE_RIGHT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_RIGHT, gunIdleRight);

	Animation* gunIdleBottom = new Animation();
	gunIdleBottom->Init(
		PLAYER_GUN_IDLE_BOTTOM_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::IDLE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, gunIdleBottom);

	// move.
	Animation* gunMoveLeft = new Animation();
	gunMoveLeft->Init(
		PLAYER_GUN_MOVE_LEFT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_LEFT, gunMoveLeft);

	Animation* gunMoveTop = new Animation();
	gunMoveTop->Init(
		PLAYER_GUN_MOVE_TOP_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_TOP, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_TOP, gunMoveTop);

	Animation* gunMoveRight = new Animation();
	gunMoveRight->Init(
		PLAYER_GUN_MOVE_RIGHT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_RIGHT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_RIGHT, gunMoveRight);

	Animation* gunMoveBottom = new Animation();
	gunMoveBottom->Init(
		PLAYER_GUN_MOVE_BOTTOM_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::MOVE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::MOVE_BOTTOM, gunMoveBottom);

	// dodge(empty).
	Animation* gunDodge = new Animation();
	gunDodge->Init(
		PLAYER_GUN_DODGE_LEFT_STRIPE,
		192, 92, 192, 92,
		1, 1, 1,
		POINT{ -96, -40 }, CHARACTER_STATE::DODGE_LEFT, true,
		true, RGB(255, 0, 255)
	);
	gunAnimator->AddAnimation(CHARACTER_STATE::DODGE_LEFT, gunDodge);
#pragma endregion	gun_animation

#pragma region		reload_animation
	// reloading.
	Animation* gunReloading = new Animation();
	gunReloading->Init(
		RELOADING_RELOADING_STRIPE,
		256, 224, 64, 32,
		4, 7, 27,
		POINT{ -32, -60 }, CHARACTER_STATE::IDLE_TOP, false,
		true, RGB(255, 0, 255)
	);
	reloadAnimator->AddAnimation(CHARACTER_STATE::IDLE_TOP, gunReloading);

	Animation* notReloading = new Animation();
	notReloading->Init(
		RELOADING_DEFAULT_STRIPE,
		64, 32, 64, 32,
		1, 1, 1,
		POINT{ -32, -60 }, CHARACTER_STATE::IDLE_BOTTOM, true,
		true, RGB(255, 0, 255)
	);
	reloadAnimator->AddAnimation(CHARACTER_STATE::IDLE_BOTTOM, notReloading);
#pragma endregion	reload_animation
}

void Player::Update(HWND hWnd, SceneManager* _scnMgr) {
	if (--damageCool < 0) damageCool = 0;
	if (stateMachine != NULL)
		stateMachine->Update(this, _scnMgr);
}

void Player::LateUpdate() { }

void Player::Render(HDC _hdc) {
	if (shadow != NULL) shadow->Render(_hdc, pos.ToPoint().x, pos.ToPoint().y);
	if(animator != NULL && stateMachine != NULL)
		animator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft());
}

void Player::Render(HDC _hdc, POINT _coordinateRevision) {
	if (shadow != NULL)
		shadow->Render(
			_hdc,
			pos.ToPoint().x - _coordinateRevision.x - PLAYER_RECT_WIDTH / 2,
			pos.ToPoint().y - _coordinateRevision.y - PLAYER_RECT_HEIGHT / 2
		);
	if (animator != NULL && stateMachine != NULL) {
		gunAnimator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft(), _coordinateRevision);
		animator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft(), _coordinateRevision);
		reloadAnimator->AnimationRender(_hdc, pos.ToPoint(), stateMachine->GetIsLeft(), _coordinateRevision);
	}
}

// render on minimap.
void Player::Render(HDC _hdc, RECT _destArea) {
	int destX = _destArea.left + (int)(pos.x) * (_destArea.right - _destArea.left) / (MAP_SIZE * PARTITION_SIZE * TILE_SIZE);
	int destY = _destArea.top + (int)(pos.y) * (_destArea.bottom - _destArea.top) / (MAP_SIZE * PARTITION_SIZE * TILE_SIZE);
	
	Image* marker = IMG->FindImage(KEY_UI_PLAYER_MARKER);
	if (marker != NULL)
		marker->Render(_hdc, destX - marker->GetWidth() / 2, destY - marker->GetHeight());
}

void Player::GetAttacked() {
	CHARACTER_STATE state = GetState();

	if (
		state == CHARACTER_STATE::DODGE_LEFT		||
		state == CHARACTER_STATE::DODGE_TOP			||
		state == CHARACTER_STATE::DODGE_RIGHT		||
		state == CHARACTER_STATE::DODGE_BOTTOM		||
		state == CHARACTER_STATE::DODGE_LEFT_TOP	||
		state == CHARACTER_STATE::DODGE_LEFT_BOTTOM	||
		state == CHARACTER_STATE::DODGE_RIGHT_TOP	||
		state == CHARACTER_STATE::DODGE_RIGHT_BOTTOM
	) return;

	if(damageCool <= 0) {
		damageCool = DAMAGE_COOL;
		--hp;
		if (hp <= 0) {
			hp = 0;
			destroy = true;
		}
	}
}

void Player::SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack) {
	stateMachine->SetCallBack_v_CB_pMb(_callBack);
}
