#include "Stdafx/stdafx.h"

#include "PlayerStateMachine.h"
#include "../../Manager/SceneManager/BattleStageSceneManager/BattleStageSceneManager.h"
#include "../../MonoBehaviour/Bullet/Bullet.h"
#include "../../MonoBehaviour/Effect/MuzzleFlash/MuzzleFlash.h"
#include "../../MonoBehaviour/Enemy/Enemy.h"
#include "../../MonoBehaviour/Player/Player.h"
#include "../../MonoBehaviour/Player/PlayerSkill/PlayerSkill.h"

PlayerStateMachine::PlayerStateMachine() : StateMachine() {
	shotCool = PLAYER_SHOT_COOL;
	skillCool = 0;
}

PlayerStateMachine::~PlayerStateMachine() { }

void PlayerStateMachine::Update(MonoBehaviour* _mb, SceneManager* _scnMgr) {
	Player* player = (Player*)_mb;
	BattleStageSceneManager* scnMgr = (BattleStageSceneManager*)_scnMgr;
	DIRECTION inputDir = DIRECTION::DIRECTION_NUM;
	DIRECTION mouseDir = DIRECTION::DIRECTION_NUM;
	bool dodge = false;
	bool shot = false;
	int mouseX = _ptMouse.x - WINSIZE_X / 2;
	int mouseY = _ptMouse.y - WINSIZE_Y / 2;

	if (player->GetDestroy() == true) {
		if (curState != CHARACTER_STATE::DEAD) {
			curState = CHARACTER_STATE::DEAD;
			if (player->GetAnimator() != NULL && player->GetGunAnimator() != NULL) {
				player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DEAD);
				player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_LEFT);
				player->GetReloadAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
			}
		}
		return;
	}

	if (--shotCool < 0) shotCool = 0;
	if (--skillCool < 0) skillCool = 0;

	if (KEY->IsOnceKeyDown('Q') && skillCool <= 0) {
		PlayerSkill* ps = new PlayerSkill(player);
		ps->Init();
		callBack(ps);

		skillCool = PLAYER_SKILL_COOL;
	}

	{
		// inputDir.
		DIRECTION inputDirX = DIRECTION::DIRECTION_NUM;
		if (KEY->IsStayKeyDown(0x44)) { // 'D'
			inputDir = inputDirX = DIRECTION::RIGHT;
		}
		if (KEY->IsStayKeyDown(0x41)) { // 'A'
			inputDir = inputDirX = DIRECTION::LEFT;
		}
		if (KEY->IsStayKeyDown(0x53)) { // 'S'
			switch (inputDirX) {
			case DIRECTION::DIRECTION_NUM:
				inputDir = DIRECTION::BOTTOM;
				break;
			case DIRECTION::RIGHT:
				inputDir = DIRECTION::RIGHT_BOTTOM;
				break;
			case DIRECTION::LEFT:
				inputDir = DIRECTION::LEFT_BOTTOM;
				break;
			}
		}
		if (KEY->IsStayKeyDown(0x57)) { // 'W'
			switch (inputDirX) {
			case DIRECTION::DIRECTION_NUM:
				inputDir = DIRECTION::TOP;
				break;
			case DIRECTION::RIGHT:
				inputDir = DIRECTION::RIGHT_TOP;
				break;
			case DIRECTION::LEFT:
				inputDir = DIRECTION::LEFT_TOP;
				break;
			}
		}
		// mouseDir.
		if (mouseX > 0) {
			if (abs(mouseY) < mouseX)		mouseDir = DIRECTION::RIGHT;
			else if (mouseY < 0)			mouseDir = DIRECTION::TOP;
			else							mouseDir = DIRECTION::BOTTOM;
		}
		else if (mouseX < 0) {
			if (abs(mouseY) < abs(mouseX))	mouseDir = DIRECTION::LEFT;
			else if (mouseY < 0)			mouseDir = DIRECTION::TOP;
			else							mouseDir = DIRECTION::BOTTOM;
		}
		else {
			if (mouseY < 0)					mouseDir = DIRECTION::TOP;
			else if (mouseY > 0)			mouseDir = DIRECTION::BOTTOM;
			else							mouseDir = DIRECTION::BOTTOM;
		}
		// dodge.
		if (KEY->IsOnceKeyDown(VK_RBUTTON)) dodge = true;
		// shot.
		if (KEY->IsOnceKeyDown(VK_LBUTTON) || KEY->IsStayKeyDown(VK_LBUTTON)) shot = true;
	}

	// update state, change animation and move.
	if (player->GetAnimator() != NULL && player->GetGunAnimator() != NULL) {
		switch (curState) {
		case CHARACTER_STATE::IDLE_LEFT:
		case CHARACTER_STATE::IDLE_TOP:
		case CHARACTER_STATE::IDLE_RIGHT:
		case CHARACTER_STATE::IDLE_BOTTOM:
			if (inputDir != DIRECTION::DIRECTION_NUM) {
				if(dodge == false) { // to move state.
					switch (mouseDir) {
					case DIRECTION::LEFT:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
						curState = CHARACTER_STATE::MOVE_LEFT;
						break;
					case DIRECTION::TOP:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
						curState = CHARACTER_STATE::MOVE_TOP;
						break;
					case DIRECTION::RIGHT:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
						curState = CHARACTER_STATE::MOVE_RIGHT;
						break;
					case DIRECTION::BOTTOM:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
						curState = CHARACTER_STATE::MOVE_BOTTOM;
						break;
					}
				}
				else { // to dodge state.
					switch (inputDir) {
					case DIRECTION::LEFT:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_LEFT);
						curState = CHARACTER_STATE::DODGE_LEFT;
						break;
					case DIRECTION::TOP:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_TOP);
						curState = CHARACTER_STATE::DODGE_TOP;
						break;
					case DIRECTION::RIGHT:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_RIGHT);
						curState = CHARACTER_STATE::DODGE_RIGHT;
						break;
					case DIRECTION::BOTTOM:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_BOTTOM);
						curState = CHARACTER_STATE::DODGE_BOTTOM;
						break;
					case DIRECTION::LEFT_TOP:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_TOP);
						curState = CHARACTER_STATE::DODGE_LEFT_TOP;
						break;
					case DIRECTION::LEFT_BOTTOM:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_BOTTOM);
						curState = CHARACTER_STATE::DODGE_LEFT_BOTTOM;
						break;
					case DIRECTION::RIGHT_TOP:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_TOP);
						curState = CHARACTER_STATE::DODGE_RIGHT_TOP;
						break;
					case DIRECTION::RIGHT_BOTTOM:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_BOTTOM);
						curState = CHARACTER_STATE::DODGE_RIGHT_BOTTOM;
						break;
					}
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_LEFT);
				} // move.
				switch (inputDir) {
				case DIRECTION::LEFT:
					player->Move(POINT{ -20, 0 });
					break;
				case DIRECTION::TOP:
					player->Move(POINT{ 0, -20 });
					break;
				case DIRECTION::RIGHT:
					player->Move(POINT{ 20, 0 });
					break;
				case DIRECTION::BOTTOM:
					player->Move(POINT{ 0, 20 });
					break;
				case DIRECTION::LEFT_TOP:
					player->Move(POINT{ -14, -14 });
					break;
				case DIRECTION::LEFT_BOTTOM:
					player->Move(POINT{ -14, 14 });
					break;
				case DIRECTION::RIGHT_TOP:
					player->Move(POINT{ 14, -14 });
					break;
				case DIRECTION::RIGHT_BOTTOM:
					player->Move(POINT{ 14, 14 });
					break;
				}
			}
			else { // face to mouse.
				switch (mouseDir) {
				case DIRECTION::LEFT:
					if(curState != CHARACTER_STATE::IDLE_LEFT) {
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
						curState = CHARACTER_STATE::IDLE_LEFT;
					}
					break;
				case DIRECTION::TOP:
					if (curState != CHARACTER_STATE::IDLE_TOP) {
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
						curState = CHARACTER_STATE::IDLE_TOP;
					}
					break;
				case DIRECTION::RIGHT:
					if (curState != CHARACTER_STATE::IDLE_RIGHT) {
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
						curState = CHARACTER_STATE::IDLE_RIGHT;
					}
					break;
				case DIRECTION::BOTTOM:
					if (curState != CHARACTER_STATE::IDLE_BOTTOM) {
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
						player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
						curState = CHARACTER_STATE::IDLE_BOTTOM;
					}
					break;
				}
			}
			break;
		case CHARACTER_STATE::MOVE_LEFT:
		case CHARACTER_STATE::MOVE_TOP:
		case CHARACTER_STATE::MOVE_RIGHT:
		case CHARACTER_STATE::MOVE_BOTTOM:
			if (inputDir == DIRECTION::DIRECTION_NUM) { // to idle state.
				switch (mouseDir) {
				case DIRECTION::LEFT:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					curState = CHARACTER_STATE::IDLE_LEFT;
					break;
				case DIRECTION::TOP:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					curState = CHARACTER_STATE::IDLE_TOP;
					break;
				case DIRECTION::RIGHT:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					curState = CHARACTER_STATE::IDLE_RIGHT;
					break;
				case DIRECTION::BOTTOM:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					curState = CHARACTER_STATE::IDLE_BOTTOM;
					break;
				}
			}
			else {
				if (dodge == false) {
					switch (mouseDir) { // face to mouse.
					case DIRECTION::LEFT:
						if(curState != CHARACTER_STATE::MOVE_LEFT) {
							player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
							player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
							curState = CHARACTER_STATE::MOVE_LEFT;
						}
						break;
					case DIRECTION::TOP:
						if (curState != CHARACTER_STATE::MOVE_TOP) {
							player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
							player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
							curState = CHARACTER_STATE::MOVE_TOP;
						}
						break;
					case DIRECTION::RIGHT:
						if (curState != CHARACTER_STATE::MOVE_RIGHT) {
							player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
							player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
							curState = CHARACTER_STATE::MOVE_RIGHT;
						}
						break;
					case DIRECTION::BOTTOM:
						if (curState != CHARACTER_STATE::MOVE_BOTTOM) {
							player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
							player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
							curState = CHARACTER_STATE::MOVE_BOTTOM;
						}
						break;
					}
				}
				else { // to dodge state.
					switch (inputDir) {
					case DIRECTION::LEFT:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_LEFT);
						curState = CHARACTER_STATE::DODGE_LEFT;
						break;
					case DIRECTION::TOP:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_TOP);
						curState = CHARACTER_STATE::DODGE_TOP;
						break;
					case DIRECTION::RIGHT:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_RIGHT);
						curState = CHARACTER_STATE::DODGE_RIGHT;
						break;
					case DIRECTION::BOTTOM:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_BOTTOM);
						curState = CHARACTER_STATE::DODGE_BOTTOM;
						break;
					case DIRECTION::LEFT_TOP:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_TOP);
						curState = CHARACTER_STATE::DODGE_LEFT_TOP;
						break;
					case DIRECTION::LEFT_BOTTOM:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_BOTTOM);
						curState = CHARACTER_STATE::DODGE_LEFT_BOTTOM;
						break;
					case DIRECTION::RIGHT_TOP:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_TOP);
						curState = CHARACTER_STATE::DODGE_RIGHT_TOP;
						break;
					case DIRECTION::RIGHT_BOTTOM:
						player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_BOTTOM);
						curState = CHARACTER_STATE::DODGE_RIGHT_BOTTOM;
						break;
					}
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::DODGE_LEFT);
				}
				// move.
				switch (inputDir) {
				case DIRECTION::LEFT:
					player->Move(POINT{ -20, 0 });
					break;
				case DIRECTION::TOP:
					player->Move(POINT{ 0, -20 });
					break;
				case DIRECTION::RIGHT:
					player->Move(POINT{ 20, 0 });
					break;
				case DIRECTION::BOTTOM:
					player->Move(POINT{ 0, 20 });
					break;
				case DIRECTION::LEFT_TOP:
					player->Move(POINT{ -14, -14 });
					break;
				case DIRECTION::LEFT_BOTTOM:
					player->Move(POINT{ -14, 14 });
					break;
				case DIRECTION::RIGHT_TOP:
					player->Move(POINT{ 14, -14 });
					break;
				case DIRECTION::RIGHT_BOTTOM:
					player->Move(POINT{ 14, 14 });
					break;
				}
			}
			break;
		case CHARACTER_STATE::DODGE_LEFT:
		case CHARACTER_STATE::DODGE_TOP:
		case CHARACTER_STATE::DODGE_RIGHT:
		case CHARACTER_STATE::DODGE_BOTTOM:
		case CHARACTER_STATE::DODGE_LEFT_TOP:
		case CHARACTER_STATE::DODGE_LEFT_BOTTOM:
		case CHARACTER_STATE::DODGE_RIGHT_TOP:
		case CHARACTER_STATE::DODGE_RIGHT_BOTTOM:
			if (player->GetAnimator()->IsEnd()) { // to idle state.
				switch (mouseDir) {
				case DIRECTION::LEFT:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					curState = CHARACTER_STATE::IDLE_LEFT;
					break;
				case DIRECTION::TOP:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					curState = CHARACTER_STATE::IDLE_TOP;
					break;
				case DIRECTION::RIGHT:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					curState = CHARACTER_STATE::IDLE_RIGHT;
					break;
				case DIRECTION::BOTTOM:
					player->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					player->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					curState = CHARACTER_STATE::IDLE_BOTTOM;
					break;
				}
			}
			else { // move.
				switch (curState) {
				case CHARACTER_STATE::DODGE_LEFT:
					player->Move(POINT{ -16, 0 });
					break;
				case CHARACTER_STATE::DODGE_TOP:
					player->Move(POINT{ 0, -16 });
					break;
				case CHARACTER_STATE::DODGE_RIGHT:
					player->Move(POINT{ 16, 0 });
					break;
				case CHARACTER_STATE::DODGE_BOTTOM:
					player->Move(POINT{ 0, 16 });
					break;
				case CHARACTER_STATE::DODGE_LEFT_TOP:
					player->Move(POINT{ -11, -11 });
					break;
				case CHARACTER_STATE::DODGE_LEFT_BOTTOM:
					player->Move(POINT{ -11, 11 });
					break;
				case CHARACTER_STATE::DODGE_RIGHT_TOP:
					player->Move(POINT{ 11, -11 });
					break;
				case CHARACTER_STATE::DODGE_RIGHT_BOTTOM:
					player->Move(POINT{ 11, 11 });
					break;
				}
			}
			break;
		}
	}

	// shot.
	if (shot == true && shotCool == 0) {
		if(player->GetBulletCount() >= 1) {
			VELOCITY vel;
			F_POINT mfFPos;
			MuzzleFlash* mf = NULL;
			Bullet* bullet = NULL;

			// calculate velocity.
			{ 
				float length = (float)sqrt(mouseX * mouseX + mouseY * mouseY);
				float velX;
				float velY;
				float theta = RND->GetFloatFromTo(-0.15f, 0.15f);
				if (length != 0) {
					velX = BULLET_SPEED * mouseX / length;
					velY = BULLET_SPEED * mouseY / length;
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
				mfFPos = F_POINT{ player->GetFPos().x - 45, player->GetFPos().y + 16 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
				break;
			case CHARACTER_STATE::IDLE_TOP: // 25, -14
				mfFPos = F_POINT{ player->GetFPos().x + 25, player->GetFPos().y - 14 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
				break;
			case CHARACTER_STATE::IDLE_RIGHT: // 45, 16
				mfFPos = F_POINT{ player->GetFPos().x + 45, player->GetFPos().y + 16 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
				break;
			case CHARACTER_STATE::IDLE_BOTTOM: // -23, 42
				mfFPos = F_POINT{ player->GetFPos().x - 23, player->GetFPos().y + 42 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
				break;
			case CHARACTER_STATE::MOVE_LEFT: // -50, -4
				mfFPos = F_POINT{ player->GetFPos().x - 50, player->GetFPos().y - 4 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
				break;
			case CHARACTER_STATE::MOVE_TOP: // 30, -26
				mfFPos = F_POINT{ player->GetFPos().x + 30, player->GetFPos().y - 26 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
				break;
			case CHARACTER_STATE::MOVE_RIGHT: // 50, -4
				mfFPos = F_POINT{ player->GetFPos().x + 50, player->GetFPos().y - 4 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
				break;
			case CHARACTER_STATE::MOVE_BOTTOM: // -28, 36
				mfFPos = F_POINT{ player->GetFPos().x - 28, player->GetFPos().y + 36 };
				mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
				break;
			}
			if (mf != NULL) {
				shotCool = PLAYER_SHOT_COOL;
				mf->Init();
				bullet = new Bullet(mfFPos, vel, true, 1);
				bullet->Init();
				callBack(mf);
				callBack(bullet);

				SOUND->Play(KEY_DEFAULT_GUN_SOUND);
				player->SetBulletCount(player->GetBulletCount() - 1);
			}
		}
		else {
			// reload animator has only 2 states : IDLE_TOP, IDLE_BOTTOM.
			// IDLE_TOP means reloading is ongoing.
			// IDLE_BOTTOM is default state.
			if (player->GetReloadAnimator()->GetCurState() == CHARACTER_STATE::IDLE_BOTTOM) {
				player->GetReloadAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
				SOUND->Play(KEY_PLAYER_RELOAD_SOUND);
			}
		}
	}
	if (player->GetReloadAnimator()->IsEnd()) {
		player->GetReloadAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
		player->SetBulletCount(PLAYER_MAX_BULLET);
	}
}

void PlayerStateMachine::SetState(MonoBehaviour* _mb, CHARACTER_STATE _state) {
	if (_mb->GetAnimator() != NULL) {
		_mb->GetAnimator()->ChangeAnimation(_state);
		curState = _state;
	}
}

void PlayerStateMachine::SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack) {
	callBack = std::move(_callBack);
}
