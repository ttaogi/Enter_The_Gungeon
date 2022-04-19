#pragma warning(disable: 6385)

#include "Stdafx/stdafx.h"

#include "Room.h"
#include "../Map.h"
#include "../../MonoBehaviour/Enemy/Enemy.h"
#include "../../MonoBehaviour/Enemy/Boss/Boss.h"
#include "../../Manager/SceneManager/BattleStageSceneManager/BattleStageSceneManager.h"

using namespace std;

Room::Room(int _prevId, int _id, POINT _posInMap, BattleStageSceneManager* _scnMgr, Map* _map) {
	prevId = _prevId;
	id = _id;
	width = RND->GetIntFromTo(ROOM_SIZE_MIN, ROOM_SIZE_MAX);
	height = RND->GetIntFromTo(ROOM_SIZE_MIN, ROOM_SIZE_MAX);
	firstVisit = true;
	type = (_id == 0) ? ROOM_TYPE::START : (_id == ROOM_COUNT - 1) ? ROOM_TYPE::BOSS : ROOM_TYPE::NORMAL;
	posInMap = _posInMap;
	posInPartition = POINT{ (PARTITION_SIZE - width) / 2, (PARTITION_SIZE - height) / 2 };
	rcCheckPlayerVisit = RECT{
		posInMap.x * PARTITION_SIZE * TILE_SIZE + posInPartition.x * TILE_SIZE + TILE_SIZE,
		posInMap.y * PARTITION_SIZE * TILE_SIZE + posInPartition.y * TILE_SIZE + TILE_SIZE,
		posInMap.x * PARTITION_SIZE * TILE_SIZE + posInPartition.x * TILE_SIZE + (width - 1) * TILE_SIZE,
		posInMap.y * PARTITION_SIZE * TILE_SIZE + posInPartition.y * TILE_SIZE + (height - 1) * TILE_SIZE
	};
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			roomMap[i][j] = TILE(
				TILE_TYPE::GROUND,
				RECT{
					rcCheckPlayerVisit.left - TILE_SIZE + i * TILE_SIZE,
					rcCheckPlayerVisit.top - TILE_SIZE + j * TILE_SIZE,
					rcCheckPlayerVisit.left + i * TILE_SIZE,
					rcCheckPlayerVisit.top + j * TILE_SIZE
				}
			);
		}
	}
	for (int i = 0; i < width; ++i) {
		roomMap[i][0].SetType(TILE_TYPE::WALL);
		roomMap[i][height - 1].SetType(TILE_TYPE::WALL);
	}
	for (int j = 1; j < height - 1; ++j) {
		roomMap[0][j].SetType(TILE_TYPE::WALL);
		roomMap[width - 1][j].SetType(TILE_TYPE::WALL);
	}
	scnMgr = _scnMgr;
	map = _map;
}

Room::~Room() { }

void Room::Render(HDC _hdc, POINT _coordinateRevision) {
	int top = rcCheckPlayerVisit.top - TILE_SIZE;
	int left = rcCheckPlayerVisit.left - TILE_SIZE;

	if (firstVisit == false) {
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				if (roomMap[i][j].GetBackImg() != NULL) {
					roomMap[i][j].GetBackImg()->Render(
						_hdc,
						roomMap[i][j].GetRect().left - _coordinateRevision.x,
						roomMap[i][j].GetRect().top - _coordinateRevision.y
					);
				}
			}
		}
	}
}

// for minimap rendering.
void Room::Render(HDC _hdc, int _destX, int _destY, float _scaleWidth, float _scaleHeight) {
	float scaledWidth = TILE_SIZE * _scaleWidth;
	float scaledHeight = TILE_SIZE * _scaleHeight;
	int destX = _destX + (int)((posInMap.x * PARTITION_SIZE * TILE_SIZE + posInPartition.x * TILE_SIZE) * _scaleWidth);
	int destY = _destY + (int)((posInMap.y * PARTITION_SIZE * TILE_SIZE + posInPartition.y * TILE_SIZE) * _scaleHeight);

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (roomMap[i][j].GetBackImg() != NULL) {
				roomMap[i][j].GetBackImg()->Render(
					_hdc,
					destX + (int)(i * scaledWidth),
					destY + (int)(j * scaledHeight),
					(int)scaledWidth,
					(int)scaledHeight
				);
			}
		}
	}
}

// update minimap and revise player`s position.
void Room::CollisionDetect(Player& _player, HDC _hdc ,
	int _destX, int _destY, float _scaleWidth, float _scaleHeight
) {
	// update minimap.
	if (firstVisit == true) {
		if (_player.IsCollided(&rcCheckPlayerVisit) == true) {
			firstVisit = false;
			if(id != 0) CloseRoom();

			// render to minimap.
			Render(_hdc, _destX, _destY, _scaleWidth, _scaleHeight);
			
			if(id != 0 && id != ROOM_COUNT - 1) {
				int enemyCount = RND->GetIntFromTo(ENEMY_COUNT_MIN, ENEMY_COUNT_MAX);
				closeCount = enemyCount;
				for (int i = 0; i < enemyCount; ++i) {
					Enemy* enemy = new Enemy(this, rcCheckPlayerVisit);
					enemy->SetCallBack_v_CB_pMb(std::bind(&BattleStageSceneManager::AddEffect, scnMgr, std::placeholders::_1));
					enemy->Init();
					scnMgr->AddEnemy(enemy, this);
				}
			}
			else if (id == ROOM_COUNT - 1) {
				Boss* boss = new Boss(this, rcCheckPlayerVisit);
				closeCount = 1;
				boss->SetCallBack_v_CB_pMb(std::bind(&BattleStageSceneManager::AddEffect, scnMgr, std::placeholders::_1));
				boss->Init();
				scnMgr->AddEnemy(boss, this);
				scnMgr->ShowBossCutScene();
			}
		}
	}
	// handle collision with player. revise player`s position.
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (roomMap[i][j].GetType() == TILE_TYPE::PILLAR || roomMap[i][j].GetType() == TILE_TYPE::WALL) {
				RECT tileRc = roomMap[i][j].GetRect();
				_player.MoveCollisionCheck(&tileRc);
			}
		}
	}
}

// check collisions with bullets and set destroyed.
void Room::CollisionDetect(Bullet* _bullet) {
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (roomMap[i][j].GetType() == TILE_TYPE::PILLAR || roomMap[i][j].GetType() == TILE_TYPE::WALL) {
				RECT tileRc = roomMap[i][j].GetRect();
				if (_bullet->IsCollided(&tileRc) == true) {
					_bullet->SetDestroy(true);
					return;
				}
			}
		}
	}
}

POINT Room::GetCenterPos() const {
	return POINT{
		(rcCheckPlayerVisit.left + rcCheckPlayerVisit.right) / 2,
		(rcCheckPlayerVisit.top + rcCheckPlayerVisit.bottom) / 2
	};
}

POINT Room::GetEntrancePosLeft() const {
	return POINT{
		posInMap.x * PARTITION_SIZE + posInPartition.x,
		posInMap.y * PARTITION_SIZE + posInPartition.y + height / 2
	};
}

POINT Room::GetEntrancePosTop() const {
	return POINT{
		posInMap.x * PARTITION_SIZE + posInPartition.x + width / 2,
		posInMap.y * PARTITION_SIZE + posInPartition.y
	};
}

POINT Room::GetEntrancePosRight() const {
	return POINT{
		posInMap.x * PARTITION_SIZE + posInPartition.x + width - 1,
		posInMap.y * PARTITION_SIZE + posInPartition.y + height / 2
	};
}

POINT Room::GetEntrancePosBottom() const {
	return POINT{
		posInMap.x * PARTITION_SIZE + posInPartition.x + width / 2,
		posInMap.y * PARTITION_SIZE + posInPartition.y + height - 1
	};
}

void Room::OpenEntranceLeft() {
	roomMap[0][height / 2].SetType(TILE_TYPE::GROUND);
}

void Room::OpenEntranceTop() {
	roomMap[width / 2][0].SetType(TILE_TYPE::GROUND);
}

void Room::OpenEntranceRight() {
	roomMap[width - 1][height / 2].SetType(TILE_TYPE::GROUND);
}

void Room::OpenEntranceBottom() {
	roomMap[width / 2][height - 1].SetType(TILE_TYPE::GROUND);
}

void Room::PrintRoom() const {
	wprintf_s(L"\n\n%d->\troom : %d\twidth : %d\theight : %d\n", prevId, id, width, height);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			wcout << roomMap[j][i].ToWString() << L"\t";
		}
		wcout << endl;
	}
}

void Room::CloseCountDecrease() {
	--closeCount;
	if (closeCount == 0) OpenRoom();
}

void Room::CloseRoom() {
	map->CloseRoom();
	if(id == ROOM_COUNT - 1) {
		SOUND->AllStop();
		SOUND->Play(KEY_BOSS_THEME_SOUND);
	}
}

void Room::OpenRoom() {
	map->OpenRoom();
	if (id == ROOM_COUNT - 1) {
		SOUND->AllStop();
		SOUND->Play(KEY_BATTLE_STAGE_THEME_SOUND);
	}
}

void Room::GameClear() {
	closeCount = 0;
	OpenRoom();
	scnMgr->GameClear();
}
