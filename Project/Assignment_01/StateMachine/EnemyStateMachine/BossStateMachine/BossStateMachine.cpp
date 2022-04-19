#include "Stdafx/stdafx.h"

#include "BossStateMachine.h"
#include "../../../Manager/SceneManager/BattleStageSceneManager/BattleStageSceneManager.h"
#include "../../../MonoBehaviour/Bullet/Bullet.h"
#include "../../../MonoBehaviour/Effect/MuzzleFlash/MuzzleFlash.h"
#include "../../../MonoBehaviour/Enemy/Boss/Boss.h"
#include "../../../MonoBehaviour/Player/Player.h"

BossStateMachine::BossStateMachine() {
	shotCool = BOSS_SHOT_COOL;
	bulletCount = BULLET_COUNT * 6;
	patternCount = 0;
}

BossStateMachine::~BossStateMachine() { }

void BossStateMachine::Update(MonoBehaviour* _mb, SceneManager* _scnMgr, Room* _room) {
	Boss* boss = (Boss*)_mb;
	BattleStageSceneManager* scnMgr = (BattleStageSceneManager*)_scnMgr;
	DIRECTION faceDir = DIRECTION::DIRECTION_NUM;
	POINT posBoss = boss->GetPos();
	POINT posPlayer = scnMgr->GetPlayerP()->GetPos();
	int deltaX = posPlayer.x - posBoss.x;
	int deltaY = posPlayer.y - posBoss.y;
	RECT rcCheckPlayerVisit = _room->GetRcCheckPlayerVisit();
	int roomWidth = _room->GetWidth();
	int roomHeight = _room->GetHeight();
	bool attack = false;

	if (--shotCool < 0) shotCool = 0;

	{
		// convert pos to index at roomMap.
		POINT idxBoss{
			(posBoss.x - rcCheckPlayerVisit.left) / TILE_SIZE + 1,
			(posBoss.y - rcCheckPlayerVisit.top) / TILE_SIZE + 1
		};
		POINT idxPlayer{
			(posPlayer.x - rcCheckPlayerVisit.left) / TILE_SIZE + 1,
			(posPlayer.y - rcCheckPlayerVisit.top) / TILE_SIZE + 1
		};

		if ((deltaX * deltaX) + (deltaY * deltaY)
			< (ENEMY_ATTACK_RANGE * ENEMY_ATTACK_RANGE)
			) {
			attack = true;
			if (deltaY > 0) {
				if (abs(deltaY) > abs(deltaX)) faceDir = DIRECTION::BOTTOM;
				else if (deltaX > 0) faceDir = DIRECTION::RIGHT;
				else faceDir = DIRECTION::LEFT;
			}
			else if (deltaY < 0) {
				if (abs(deltaY) > abs(deltaX)) faceDir = DIRECTION::TOP;
				else if (deltaX > 0) faceDir = DIRECTION::RIGHT;
				else faceDir = DIRECTION::LEFT;
			}
			else {
				if (deltaX > 0) faceDir = DIRECTION::RIGHT;
				else if (deltaX < 0) faceDir = DIRECTION::LEFT;
				else faceDir = DIRECTION::BOTTOM;
			}
		}
		else {
			if (
				idxPlayer.x < 1 ||
				idxPlayer.x > roomWidth - 2 ||
				idxPlayer.y < 1 ||
				idxPlayer.y > roomHeight - 2
				)
				faceDir = DIRECTION::DIRECTION_NUM;
			else
				faceDir = SearchRoute(_room, roomWidth, roomHeight, idxBoss, idxPlayer);
		}
	}

	if (boss->GetAnimator() != NULL) {
		switch (curState) {
		case CHARACTER_STATE::IDLE_LEFT:
		case CHARACTER_STATE::IDLE_TOP:
		case CHARACTER_STATE::IDLE_RIGHT:
		case CHARACTER_STATE::IDLE_BOTTOM:
			if (attack == true) {
				switch (faceDir) {
				case DIRECTION::LEFT:
					if (curState != CHARACTER_STATE::IDLE_LEFT) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
						curState = CHARACTER_STATE::IDLE_LEFT;
					}
					break;
				case DIRECTION::TOP:
					if (curState != CHARACTER_STATE::IDLE_TOP) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
						curState = CHARACTER_STATE::IDLE_TOP;
					}
					break;
				case DIRECTION::RIGHT:
					if (curState != CHARACTER_STATE::IDLE_RIGHT) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
						curState = CHARACTER_STATE::IDLE_RIGHT;
					}
					break;
				case DIRECTION::BOTTOM:
					if (curState != CHARACTER_STATE::IDLE_BOTTOM) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
						curState = CHARACTER_STATE::IDLE_BOTTOM;
					}
					break;
				}
				if (shotCool <= 0) {
					VELOCITY vel;
					F_POINT mfFPos;
					MuzzleFlash* mf = NULL;
					Bullet* bullet = NULL;

					// calculate velocity.
					{
						float length = (float)sqrt(deltaX * deltaX + deltaY * deltaY);
						float velX;
						float velY;
						float theta = RND->GetFloatFromTo(-0.15f, 0.15f);
						if (length != 0) {
							velX = BULLET_SPEED * deltaX / length;
							velY = BULLET_SPEED * deltaY / length;
						}
						else {
							velX = 0;
							velY = BULLET_SPEED;
						}
						vel.SetX(velX * cos(theta) + velY * sin(theta));
						vel.SetY(velX * -1 * sin(theta) + velY * cos(theta));
					}

					// muzzle flash.
					switch (curState) {
					case CHARACTER_STATE::IDLE_LEFT: // -45, 16
						mfFPos = F_POINT{ boss->GetFPos().x - 45, boss->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::IDLE_TOP: // 25, -14
						mfFPos = F_POINT{ boss->GetFPos().x + 25, boss->GetFPos().y - 14 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::IDLE_RIGHT: // 45, 16
						mfFPos = F_POINT{ boss->GetFPos().x + 45, boss->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::IDLE_BOTTOM: // -23, 42
						mfFPos = F_POINT{ boss->GetFPos().x - 23, boss->GetFPos().y + 42 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					case CHARACTER_STATE::MOVE_LEFT: // -50, -4
						mfFPos = F_POINT{ boss->GetFPos().x - 50, boss->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::MOVE_TOP: // 30, -26
						mfFPos = F_POINT{ boss->GetFPos().x + 30, boss->GetFPos().y - 26 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::MOVE_RIGHT: // 50, -4
						mfFPos = F_POINT{ boss->GetFPos().x + 50, boss->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::MOVE_BOTTOM: // -28, 36
						mfFPos = F_POINT{ boss->GetFPos().x - 28, boss->GetFPos().y + 36 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					}
					if (mf != NULL) {
						if (patternCount == 0) {
							shotCool = BOSS_SHOT_COOL / 24;
							mf->Init();
							bullet = new Bullet(mfFPos, vel, false, 1);
							bullet->Init();
							callBack(mf);
							callBack(bullet);
							if (--bulletCount == 0) {
								patternCount = 1;
								bulletCount = BULLET_COUNT * 4;
							}
						}
						else if (patternCount == 1) {
							shotCool = BOSS_SHOT_COOL / 24;
							delete(mf);
							float dir = ((bulletCount / 4) % 2 == 0) ? (M_PI / 4) : (M_PI / -4);
							for (int i = 0; i < 18; ++i) {
								float theta = i * M_PI / 9;
								F_POINT pos{ posBoss.x + 30 * cos(theta), posBoss.y + 30 * sin(theta) };
								VELOCITY vel{ BULLET_SPEED / 2 * cos(theta + dir), BULLET_SPEED / 2 * sin(theta + dir) };
								Bullet* b = new Bullet(pos, vel, false, 1);
								b->Init();
								callBack(b);
							}
							if (--bulletCount == 0) {
								patternCount = 2;
								bulletCount = BULLET_COUNT;
							}
						}
						else { // 2
							shotCool = BOSS_SHOT_COOL;
							delete(mf);
							F_POINT centerPos{
								RND->GetFloatFromTo(rcCheckPlayerVisit.left + TILE_SIZE, rcCheckPlayerVisit.right - TILE_SIZE),
								RND->GetFloatFromTo(rcCheckPlayerVisit.top + TILE_SIZE, rcCheckPlayerVisit.bottom - TILE_SIZE)
							};
							for (int i = 0; i < 18; ++i) {
								float theta = i * M_PI / 9;
								F_POINT pos{ centerPos.x + 10 * cos(theta), centerPos.y + 10 * sin(theta) };
								VELOCITY vel{ BULLET_SPEED * cos(theta), BULLET_SPEED * sin(theta) };
								Bullet* b = new Bullet(pos, vel, false, 1);
								b->Init();
								callBack(b);
							}
							if (--bulletCount == 0) {
								patternCount = 0;
								bulletCount = BULLET_COUNT * 6;
							}
						}
					}
				}
			}
			else {
				switch (faceDir) {
				case DIRECTION::LEFT:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
					curState = CHARACTER_STATE::MOVE_LEFT;
					boss->Move(POINT{ -16, 0 });
					break;
				case DIRECTION::TOP:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
					curState = CHARACTER_STATE::MOVE_TOP;
					boss->Move(POINT{ 0, -16 });
					break;
				case DIRECTION::RIGHT:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
					curState = CHARACTER_STATE::MOVE_RIGHT;
					boss->Move(POINT{ 16, 0 });
					break;
				case DIRECTION::BOTTOM:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
					curState = CHARACTER_STATE::MOVE_BOTTOM;
					boss->Move(POINT{ 0, 16 });
					break;
				}
			}
			break;
		case CHARACTER_STATE::MOVE_LEFT:
		case CHARACTER_STATE::MOVE_TOP:
		case CHARACTER_STATE::MOVE_RIGHT:
		case CHARACTER_STATE::MOVE_BOTTOM:
			if (attack == true) {
				switch (faceDir) {
				case DIRECTION::LEFT:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					curState = CHARACTER_STATE::IDLE_LEFT;
					break;
				case DIRECTION::TOP:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					curState = CHARACTER_STATE::IDLE_TOP;
					break;
				case DIRECTION::RIGHT:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					curState = CHARACTER_STATE::IDLE_RIGHT;
					break;
				case DIRECTION::BOTTOM:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					curState = CHARACTER_STATE::IDLE_BOTTOM;
					break;
				}
				if (shotCool <= 0) {
					VELOCITY vel;
					F_POINT mfFPos;
					MuzzleFlash* mf = NULL;
					Bullet* bullet = NULL;

					// calculate velocity.
					{
						float length = (float)sqrt(deltaX * deltaX + deltaY * deltaY);
						float velX;
						float velY;
						float theta = RND->GetFloatFromTo(-0.15f, 0.15f);
						if (length != 0) {
							velX = BULLET_SPEED * deltaX / length;
							velY = BULLET_SPEED * deltaY / length;
						}
						else {
							velX = 0;
							velY = BULLET_SPEED;
						}
						vel.SetX(velX * cos(theta) + velY * sin(theta));
						vel.SetY(velX * -1 * sin(theta) + velY * cos(theta));
					}

					// muzzle flash.
					switch (curState) {
					case CHARACTER_STATE::IDLE_LEFT: // -45, 16
						mfFPos = F_POINT{ boss->GetFPos().x - 45, boss->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::IDLE_TOP: // 25, -14
						mfFPos = F_POINT{ boss->GetFPos().x + 25, boss->GetFPos().y - 14 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::IDLE_RIGHT: // 45, 16
						mfFPos = F_POINT{ boss->GetFPos().x + 45, boss->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::IDLE_BOTTOM: // -23, 42
						mfFPos = F_POINT{ boss->GetFPos().x - 23, boss->GetFPos().y + 42 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					case CHARACTER_STATE::MOVE_LEFT: // -50, -4
						mfFPos = F_POINT{ boss->GetFPos().x - 50, boss->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::MOVE_TOP: // 30, -26
						mfFPos = F_POINT{ boss->GetFPos().x + 30, boss->GetFPos().y - 26 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::MOVE_RIGHT: // 50, -4
						mfFPos = F_POINT{ boss->GetFPos().x + 50, boss->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::MOVE_BOTTOM: // -28, 36
						mfFPos = F_POINT{ boss->GetFPos().x - 28, boss->GetFPos().y + 36 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					}
					if (mf != NULL) {
						if (patternCount == 0) {
							shotCool = BOSS_SHOT_COOL / 24;
							mf->Init();
							bullet = new Bullet(mfFPos, vel, false, 1);
							bullet->Init();
							callBack(mf);
							callBack(bullet);
							if (--bulletCount == 0) {
								patternCount = 1;
								bulletCount = BULLET_COUNT * 4;
							}
						}
						else if (patternCount == 1) {
							shotCool = BOSS_SHOT_COOL / 24;
							delete(mf);
							float dir = ((bulletCount / 4) % 2 == 0) ? (M_PI / 4) : (M_PI / -4);
							for (int i = 0; i < 18; ++i) {
								float theta = i * M_PI / 9;
								F_POINT pos{ posBoss.x + 30 * cos(theta), posBoss.y + 30 * sin(theta) };
								VELOCITY vel{ BULLET_SPEED / 2 * cos(theta + dir), BULLET_SPEED / 2 * sin(theta + dir) };
								Bullet* b = new Bullet(pos, vel, false, 1);
								b->Init();
								callBack(b);
							}
							if (--bulletCount == 0) {
								patternCount = 2;
								bulletCount = BULLET_COUNT;
							}
						}
						else { // 2
							shotCool = BOSS_SHOT_COOL;
							delete(mf);
							F_POINT centerPos{
								RND->GetFloatFromTo(rcCheckPlayerVisit.left + TILE_SIZE, rcCheckPlayerVisit.right - TILE_SIZE),
								RND->GetFloatFromTo(rcCheckPlayerVisit.top + TILE_SIZE, rcCheckPlayerVisit.bottom - TILE_SIZE)
							};
							for (int i = 0; i < 18; ++i) {
								float theta = i * M_PI / 9;
								F_POINT pos{ centerPos.x + 10 * cos(theta), centerPos.y + 10 * sin(theta) };
								VELOCITY vel{ BULLET_SPEED * cos(theta), BULLET_SPEED * sin(theta) };
								Bullet* b = new Bullet(pos, vel, false, 1);
								b->Init();
								callBack(b);
							}
							if (--bulletCount == 0) {
								patternCount = 0;
								bulletCount = BULLET_COUNT * 6;
							}
						}
					}
				}
			}
			else {
				switch (faceDir) {
				case DIRECTION::LEFT:
					if (curState != CHARACTER_STATE::MOVE_LEFT) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
						curState = CHARACTER_STATE::MOVE_LEFT;
					}
					boss->Move(POINT{ -16, 0 });
					break;
				case DIRECTION::TOP:
					if (curState != CHARACTER_STATE::MOVE_TOP) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
						curState = CHARACTER_STATE::MOVE_TOP;
					}
					boss->Move(POINT{ 0, -16 });
					break;
				case DIRECTION::RIGHT:
					if (curState != CHARACTER_STATE::MOVE_RIGHT) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
						curState = CHARACTER_STATE::MOVE_RIGHT;
					}
					boss->Move(POINT{ 16, 0 });
					break;
				case DIRECTION::BOTTOM:
					if (curState != CHARACTER_STATE::MOVE_BOTTOM) {
						boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
						boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
						curState = CHARACTER_STATE::MOVE_BOTTOM;
					}
					boss->Move(POINT{ 0, 16 });
					break;
				default:
					boss->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					boss->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					curState = CHARACTER_STATE::IDLE_BOTTOM;
					break;
				}
			}
			break;
		}
	}
}

void BossStateMachine::SetState(MonoBehaviour* _mb, CHARACTER_STATE _state) {
	_mb->GetAnimator()->ChangeAnimation(_state);
	curState = _state;
}

void BossStateMachine::SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack) {
	callBack = std::move(_callBack);
}
