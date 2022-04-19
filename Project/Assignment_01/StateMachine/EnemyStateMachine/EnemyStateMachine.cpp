#include "Stdafx/stdafx.h"

#include "EnemyStateMachine.h"
#include "../../Manager/SceneManager/BattleStageSceneManager/BattleStageSceneManager.h"
#include "../../MonoBehaviour/Bullet/Bullet.h"
#include "../../MonoBehaviour/Effect/MuzzleFlash/MuzzleFlash.h"
#include "../../MonoBehaviour/Enemy/Enemy.h"
#include "../../MonoBehaviour/Player/Player.h"

EnemyStateMachine::EnemyStateMachine() : StateMachine() {
	shotCool = ENEMY_SHOT_COOL;
}

EnemyStateMachine::~EnemyStateMachine() { }

void EnemyStateMachine::Update(MonoBehaviour * _mb, SceneManager * _scnMgr, Room* _room) {
	Enemy* enemy = (Enemy*)_mb;
	BattleStageSceneManager* scnMgr = (BattleStageSceneManager*)_scnMgr;
	DIRECTION faceDir = DIRECTION::DIRECTION_NUM;
	POINT posEnemy = enemy->GetPos();
	POINT posPlayer = scnMgr->GetPlayerP()->GetPos();
	int deltaX = posPlayer.x - posEnemy.x;
	int deltaY = posPlayer.y - posEnemy.y;
	int roomWidth = _room->GetWidth();
	int roomHeight = _room->GetHeight();
	bool attack = false;

	if (--shotCool < 0) shotCool = 0;

	{
		RECT rcCheckPlayerVisit = _room->GetRcCheckPlayerVisit();

		// convert pos to index at roomMap.
		POINT idxEnemy{
			(posEnemy.x - rcCheckPlayerVisit.left) / TILE_SIZE + 1,
			(posEnemy.y - rcCheckPlayerVisit.top) / TILE_SIZE + 1
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
			else if(deltaY < 0) {
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
				idxPlayer.x < 1				||
				idxPlayer.x > roomWidth - 2	||
				idxPlayer.y < 1				||
				idxPlayer.y > roomHeight - 2
			)
				faceDir = DIRECTION::DIRECTION_NUM;
			else
				faceDir = SearchRoute(_room, roomWidth, roomHeight, idxEnemy, idxPlayer);
		}
	}

	if (enemy->GetAnimator() != NULL) {
		switch (curState) {
		case CHARACTER_STATE::IDLE_LEFT:
		case CHARACTER_STATE::IDLE_TOP:
		case CHARACTER_STATE::IDLE_RIGHT:
		case CHARACTER_STATE::IDLE_BOTTOM:
			if (attack == true) {
				switch (faceDir) {
				case DIRECTION::LEFT:
					if(curState != CHARACTER_STATE::IDLE_LEFT) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
						curState = CHARACTER_STATE::IDLE_LEFT;
					}
					break;
				case DIRECTION::TOP:
					if (curState != CHARACTER_STATE::IDLE_TOP) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
						curState = CHARACTER_STATE::IDLE_TOP;
					}
					break;
				case DIRECTION::RIGHT:
					if (curState != CHARACTER_STATE::IDLE_RIGHT) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
						curState = CHARACTER_STATE::IDLE_RIGHT;
					}
					break;
				case DIRECTION::BOTTOM:
					if (curState != CHARACTER_STATE::IDLE_BOTTOM) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
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
						mfFPos = F_POINT{ enemy->GetFPos().x - 45, enemy->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::IDLE_TOP: // 25, -14
						mfFPos = F_POINT{ enemy->GetFPos().x + 25, enemy->GetFPos().y - 14 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::IDLE_RIGHT: // 45, 16
						mfFPos = F_POINT{ enemy->GetFPos().x + 45, enemy->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::IDLE_BOTTOM: // -23, 42
						mfFPos = F_POINT{ enemy->GetFPos().x - 23, enemy->GetFPos().y + 42 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					case CHARACTER_STATE::MOVE_LEFT: // -50, -4
						mfFPos = F_POINT{ enemy->GetFPos().x - 50, enemy->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::MOVE_TOP: // 30, -26
						mfFPos = F_POINT{ enemy->GetFPos().x + 30, enemy->GetFPos().y - 26 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::MOVE_RIGHT: // 50, -4
						mfFPos = F_POINT{ enemy->GetFPos().x + 50, enemy->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::MOVE_BOTTOM: // -28, 36
						mfFPos = F_POINT{ enemy->GetFPos().x - 28, enemy->GetFPos().y + 36 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					}
					if (mf != NULL) {
						shotCool = ENEMY_SHOT_COOL;
						mf->Init();
						bullet = new Bullet(mfFPos, vel, false, 1);
						bullet->Init();
						callBack(mf);
						callBack(bullet);
					}
				}
			}
			else {
				switch (faceDir) {
				case DIRECTION::LEFT:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
					curState = CHARACTER_STATE::MOVE_LEFT;
					enemy->Move(POINT{ -16, 0 });
					break;
				case DIRECTION::TOP:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
					curState = CHARACTER_STATE::MOVE_TOP;
					enemy->Move(POINT{ 0, -16 });
					break;
				case DIRECTION::RIGHT:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
					curState = CHARACTER_STATE::MOVE_RIGHT;
					enemy->Move(POINT{ 16, 0 });
					break;
				case DIRECTION::BOTTOM:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
					curState = CHARACTER_STATE::MOVE_BOTTOM;
					enemy->Move(POINT{ 0, 16 });
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
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_LEFT);
					curState = CHARACTER_STATE::IDLE_LEFT;
					break;
				case DIRECTION::TOP:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_TOP);
					curState = CHARACTER_STATE::IDLE_TOP;
					break;
				case DIRECTION::RIGHT:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_RIGHT);
					curState = CHARACTER_STATE::IDLE_RIGHT;
					break;
				case DIRECTION::BOTTOM:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
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
						mfFPos = F_POINT{ enemy->GetFPos().x - 45, enemy->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::IDLE_TOP: // 25, -14
						mfFPos = F_POINT{ enemy->GetFPos().x + 25, enemy->GetFPos().y - 14 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::IDLE_RIGHT: // 45, 16
						mfFPos = F_POINT{ enemy->GetFPos().x + 45, enemy->GetFPos().y + 16 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::IDLE_BOTTOM: // -23, 42
						mfFPos = F_POINT{ enemy->GetFPos().x - 23, enemy->GetFPos().y + 42 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					case CHARACTER_STATE::MOVE_LEFT: // -50, -4
						mfFPos = F_POINT{ enemy->GetFPos().x - 50, enemy->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::LEFT);
						break;
					case CHARACTER_STATE::MOVE_TOP: // 30, -26
						mfFPos = F_POINT{ enemy->GetFPos().x + 30, enemy->GetFPos().y - 26 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::TOP);
						break;
					case CHARACTER_STATE::MOVE_RIGHT: // 50, -4
						mfFPos = F_POINT{ enemy->GetFPos().x + 50, enemy->GetFPos().y - 4 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::RIGHT);
						break;
					case CHARACTER_STATE::MOVE_BOTTOM: // -28, 36
						mfFPos = F_POINT{ enemy->GetFPos().x - 28, enemy->GetFPos().y + 36 };
						mf = new MuzzleFlash(mfFPos.ToPoint(), DIRECTION::BOTTOM);
						break;
					}
					if (mf != NULL) {
						shotCool = ENEMY_SHOT_COOL;
						mf->Init();
						bullet = new Bullet(mfFPos, vel, false, 1);
						bullet->Init();
						callBack(mf);
						callBack(bullet);
					}
				}
			}
			else {
				switch (faceDir) {
				case DIRECTION::LEFT:
					if(curState != CHARACTER_STATE::MOVE_LEFT) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_LEFT);
						curState = CHARACTER_STATE::MOVE_LEFT;
					}
					enemy->Move(POINT{ -16, 0 });
					break;
				case DIRECTION::TOP:
					if (curState != CHARACTER_STATE::MOVE_TOP) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_TOP);
						curState = CHARACTER_STATE::MOVE_TOP;
					}
					enemy->Move(POINT{ 0, -16 });
					break;
				case DIRECTION::RIGHT:
					if (curState != CHARACTER_STATE::MOVE_RIGHT) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_RIGHT);
						curState = CHARACTER_STATE::MOVE_RIGHT;
					}
					enemy->Move(POINT{ 16, 0 });
					break;
				case DIRECTION::BOTTOM:
					if (curState != CHARACTER_STATE::MOVE_BOTTOM) {
						enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
						enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::MOVE_BOTTOM);
						curState = CHARACTER_STATE::MOVE_BOTTOM;
					}
					enemy->Move(POINT{ 0, 16 });
					break;
				default:
					enemy->GetAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					enemy->GetGunAnimator()->ChangeAnimation(CHARACTER_STATE::IDLE_BOTTOM);
					curState = CHARACTER_STATE::IDLE_BOTTOM;
					break;
				}
			}
			break;
		}
	}
}

void EnemyStateMachine::SetState(MonoBehaviour* _mb, CHARACTER_STATE _state) {
	_mb->GetAnimator()->ChangeAnimation(_state);
	curState = _state;
}

void EnemyStateMachine::SetCallBack_v_CB_pMb(std::function<void(MonoBehaviour*)> _callBack) {
	callBack = std::move(_callBack);
}



struct NODE {
	POINT id;
	int f;
	int g;
	int h;
	POINT parent;

	bool operator<(const NODE n) const {
		return this->f > n.f;
	}
};

void PushQueue(vector<NODE>& _que, NODE _node) {
	bool redundant = false;
	for (int i = 0; i < _que.size(); ++i) {
		if (_que[i].id.x == _node.id.x && _que[i].id.y == _node.id.y) {
			redundant = true;
			if (_que[i].g > _node.g) {
				_que[i] = _node;
			}
			break;
		}
	}
	if (redundant == false) {
		_que.push_back(_node);
	}
	for (int i = (int)_que.size() - 1; i > 0; --i) {
		for (int j = 0; j < i; ++j) {
			if (_que[j] < _que[j + 1]) {
				NODE tmp = _que[j];
				_que[j] = _que[j + 1];
				_que[j + 1] = tmp;
			}
		}
	}
}

bool HasNode(vector<NODE>& _que, POINT _pos) {
	for (auto iter = _que.begin(); iter != _que.end(); ++iter)
		if (iter->id.x == _pos.x && iter->id.y == _pos.y) return true;
	return false;
}

void SearchNearTile(
	vector<NODE>& _open, vector<NODE>& _close,
	POINT _playerPos, Room* _room,
	int _width, int _height, NODE _parent
) {
	if (_parent.id.y >= 1) {
		POINT top{ _parent.id.x, _parent.id.y - 1 };

		if (_room->GetRoomMap(top.x, top.y).GetType() == TILE_TYPE::GROUND &&
			!HasNode(_close, top)
		) {
			NODE n{ top,
				_parent.g + 1 + (abs(_playerPos.x - top.x) + abs(_playerPos.y - top.y)),
				_parent.g + 1,
				(abs(_playerPos.x - top.x) + abs(_playerPos.y - top.y)),
				_parent.id
			};
			PushQueue(_open, n);
		}
	}
	if (_parent.id.x >= 1) {
		POINT left{ _parent.id.x - 1, _parent.id.y };

		if (_room->GetRoomMap(left.x, left.y).GetType() == TILE_TYPE::GROUND &&
			!HasNode(_close, left)
		) {
			NODE n{ left,
				_parent.g + 1 + (abs(_playerPos.x - left.x) + abs(_playerPos.y - left.y)),
				_parent.g + 1,
				(abs(_playerPos.x - left.x) + abs(_playerPos.y - left.y)),
				_parent.id
			};
			PushQueue(_open, n);
		}
	}
	if (_parent.id.x < _width - 1) {
		POINT right{ _parent.id.x + 1, _parent.id.y };

		if (_room->GetRoomMap(right.x, right.y).GetType() == TILE_TYPE::GROUND &&
			!HasNode(_close, right)
		) {
			NODE n{ right,
				_parent.g + 1 + (abs(_playerPos.x - right.x) + abs(_playerPos.y - right.y)),
				_parent.g + 1,
				(abs(_playerPos.x - right.x) + abs(_playerPos.y - right.y)),
				_parent.id
			};
			PushQueue(_open, n);
		}
	}
	if (_parent.id.y < _height - 1) {
		POINT bottom{ _parent.id.x, _parent.id.y + 1 };

		if (_room->GetRoomMap(bottom.x, bottom.y).GetType() == TILE_TYPE::GROUND &&
			!HasNode(_close, bottom)
		) {
			NODE n{ bottom,
				_parent.g + 1 + (abs(_playerPos.x - bottom.x) + abs(_playerPos.y - bottom.y)),
				_parent.g + 1,
				(abs(_playerPos.x - bottom.x) + abs(_playerPos.y - bottom.y)),
				_parent.id
			};
			PushQueue(_open, n);
		}
	}
}

DIRECTION SearchRoute(
	Room* _room, int _width, int _height,
	POINT _enemyPos, POINT _playerPos
) {
	if (_enemyPos.x == _playerPos.x && _enemyPos.y == _playerPos.y) return DIRECTION::DIRECTION_NUM;

	vector<NODE> open;
	vector<NODE> close;
	bool find = false;

	open.push_back(NODE{ _enemyPos, 0, 0, 0, POINT{-1, -1} });

	while (true) {
		NODE curNode = open.front();
		open.erase(open.begin());
		close.push_back(curNode);
		if (curNode.id.x == _playerPos.x && curNode.id.y == _playerPos.y) {
			find = true;
			break;
		}

		SearchNearTile(open, close, _playerPos, _room, _width, _height, curNode);
	}

	if (find == true) {
		NODE n = close.back();

		while (true) {
			if (n.parent.x == _enemyPos.x && n.parent.y == _enemyPos.y)
				break;
			for (int i = 0; i < close.size(); ++i) {
				if (close[i].id.x == n.parent.x && close[i].id.y == n.parent.y) {
					n = close[i];
					break;
				}
			}
		}

		if (n.id.x == _enemyPos.x - 1)		return DIRECTION::LEFT;
		else if (n.id.x == _enemyPos.x + 1)	return DIRECTION::RIGHT;
		else if (n.id.y == _enemyPos.y - 1)	return DIRECTION::TOP;
		else if (n.id.y == _enemyPos.y + 1)	return DIRECTION::BOTTOM;
		else								return DIRECTION::DIRECTION_NUM;
	}
	else
		return DIRECTION::DIRECTION_NUM;
}
