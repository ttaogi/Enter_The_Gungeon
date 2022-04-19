#include "Stdafx/stdafx.h"

#include "PlayerSkill.h"
#include "../../Bullet/Bullet.h"

PlayerSkill::PlayerSkill(Player* _player) {
	type = MONOBEHAVIOUR_TYPE::PLAYER_SKILL;
	pos = _player->GetFPos();
	vel = VELOCITY{ 0, 0 };
	rc = MakeRectCWH(pos.ToPoint(), 10, 10);
	stripe = NULL;
	animator = NULL;
	stateMachine = NULL;
	destroy = false;

	lifeCount = 0;
	player = _player;

	SOUND->Play(KEY_PLAYER_SKILL_SOUND);
}

PlayerSkill::~PlayerSkill() { }

void PlayerSkill::Init() { }

void PlayerSkill::Update(HWND hWnd, SceneManager* _scnMgr) {
	if (destroy == true) return;

	vector<MonoBehaviour*>* effectVec = _scnMgr->GetEffectVec();
	F_POINT playerPos = player->GetFPos();

	player->SetHp(PLAYER_INITIAL_HP);
	for (auto iter = effectVec->begin(); iter != effectVec->end(); ++iter) {
		if ((*iter)->GetType() == MONOBEHAVIOUR_TYPE::BULLET &&
			(*iter)->GetDestroy() == false &&
			((Bullet*)(*iter))->GetByPlayer() == false
		) {
			(*iter)->SetDestroy(true);
		}
	}
	for (int i = 0; i < 6; ++i) {
		float theta = i * M_PI / 3 + lifeCount * M_PI / 18;
		F_POINT pos{playerPos.x + 20 * cos(theta), playerPos.y + 20 * sin(theta)};
		VELOCITY vel{BULLET_SPEED / 3 * cos(theta), BULLET_SPEED / 3 * sin(theta)};
		Bullet* b = new Bullet(pos, vel, true, 1);
		b->Init();
		_scnMgr->AddEffect(b);
	}

	if (++lifeCount >= LIFE_COUNT) destroy = true;
}

void PlayerSkill::LateUpdate() { }
