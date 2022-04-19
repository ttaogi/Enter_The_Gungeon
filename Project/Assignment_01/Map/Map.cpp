#include "Stdafx/stdafx.h"

#include "Map.h"

using namespace std;

struct QueEle {
	int prevId;
	POINT p;
};

Map::Map(BattleStageSceneManager* _scnMgr) {
	POINT startPos = POINT{ RND->GetInt(MAP_SIZE), RND->GetInt(MAP_SIZE) };

	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
			map[i][j] = NULL;

	int cnt = 0;
	queue<QueEle> pointQue;
	pointQue.push(QueEle{ -1, startPos });
	while (true) {
		int prevId;

		if (pointQue.empty()) {
			bool find = false;
			for (int i = 0; i < MAP_SIZE; ++i) {
				for (int j = 0; j < MAP_SIZE; ++j) {
					if (map[i][j] == NULL) {
						QueEle e = QueEle{ -1, POINT{-1, -1} };

						if (i - 1 >= 0 && map[i - 1][j] != NULL)
							e = QueEle{ map[i - 1][j]->GetId(), POINT{i, j} };
						else if (i + 1 < MAP_SIZE && map[i + 1][j] != NULL)
							e = QueEle{ map[i + 1][j]->GetId(), POINT{i, j} };
						else if (j - 1 >= 0 && map[i][j - 1] != NULL)
							e = QueEle{ map[i][j - 1]->GetId(), POINT{i, j} };
						else if (j + 1 < MAP_SIZE && map[i][j + 1] != NULL)
							e = QueEle{ map[i][j + 1]->GetId(), POINT{i, j} };

						if(e.prevId != -1) {
							pointQue.push(e);
							find = true;
							break;
						}
					}
				}
				if (find == true) break;
			}
		}

		QueEle p = pointQue.front();
		pointQue.pop();

		if (map[p.p.x][p.p.y] != NULL) continue;

		map[p.p.x][p.p.y] = new Room(p.prevId, cnt, p.p, _scnMgr, this);
		prevId = cnt++;

		if (cnt == ROOM_COUNT) break;

		int newRoomNum = RND->GetIntFromTo(1, 2);
		for (int i = 0; i < newRoomNum; ++i) {
			int startDir = RND->GetInt(MAP_SIZE);
			for (int j = 0; j < 4; ++j) {
				POINT nextPoint;
				if (startDir == 0) {
					nextPoint = POINT{ p.p.x, p.p.y - 1 };
				}
				else if (startDir == 1) {
					nextPoint = POINT{ p.p.x + 1, p.p.y };
				}
				else if (startDir == 2) {
					nextPoint = POINT{ p.p.x, p.p.y + 1 };
				}
				else {
					nextPoint = POINT{ p.p.x - 1, p.p.y };
				}

				if (nextPoint.x >= 0 && nextPoint.x < MAP_SIZE &&
					nextPoint.y >= 0 && nextPoint.y < MAP_SIZE
				) {
					if (map[nextPoint.x][nextPoint.y] == NULL) {
						pointQue.push(QueEle{ prevId, nextPoint });
						break;
					}
				}

				startDir = (startDir + 1) % 4;
			}
		}
	}

	for (int i = ROOM_COUNT - 1; i >= 1; --i)
		ConnectRooms(i);
}

Map::~Map() {
	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
			SAFE_DELETE(map[i][j]);
	for (auto iter = pathVec.begin(); iter != pathVec.end(); ++iter)
		SAFE_DELETE(*iter);
}

void Map::Render(HDC _hdc, POINT _coordinateRevision) {
	POINT partitionOfPlayer = POINT{
		(_coordinateRevision.x + WINSIZE_X / 2) / (PARTITION_SIZE * TILE_SIZE),
		(_coordinateRevision.y + WINSIZE_Y / 2) / (PARTITION_SIZE * TILE_SIZE)
	};

	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			if (map[i][j] != NULL) {
				POINT partitionOfMap = map[i][j]->GetPosInMap();
				if(partitionOfMap.x >= partitionOfPlayer.x - 1 &&
					partitionOfMap.x <= partitionOfPlayer.x + 1 &&
					partitionOfMap.y >= partitionOfPlayer.y - 1 &&
					partitionOfMap.y <= partitionOfPlayer.y + 1
				) {
					map[i][j]->Render(_hdc, _coordinateRevision);
				}
			}
		}
	}
	for (auto iter = pathVec.begin(); iter != pathVec.end(); ++iter) {
		POINT partitionOfPath1 = (*iter)->GetPartitions(0);
		POINT partitionOfPath2 = (*iter)->GetPartitions(1);

		if(
			partitionOfPath1.x >= partitionOfPlayer.x - 1 &&
			partitionOfPath1.x <= partitionOfPlayer.x + 1 &&
			partitionOfPath1.y >= partitionOfPlayer.y - 1 &&
			partitionOfPath1.y <= partitionOfPlayer.y + 1 &&
			partitionOfPath2.x >= partitionOfPlayer.x - 1 &&
			partitionOfPath2.x <= partitionOfPlayer.x + 1 &&
			partitionOfPath2.y >= partitionOfPlayer.y - 1 &&
			partitionOfPath2.y <= partitionOfPlayer.y + 1
		) {
			(*iter)->Render(_hdc, _coordinateRevision);
		}
	}
}

void Map::Render(HDC _hdc, RECT _destArea) {
	int areaWidth = _destArea.right - _destArea.left;
	int areaHeight = _destArea.bottom - _destArea.top;
	float scaleWidth = (float)areaWidth / (MAP_SIZE * PARTITION_SIZE * TILE_SIZE);
	float scaleHeight = (float)areaHeight / (MAP_SIZE * PARTITION_SIZE * TILE_SIZE);

	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			if (map[i][j] != NULL) {
				if (map[i][j]->GetFirstVisit() == false) {
					map[i][j]->Render(_hdc, _destArea.left, _destArea.top, scaleWidth, scaleHeight);
				}
			}
		}
	}
	for (auto iter = pathVec.begin(); iter != pathVec.end(); ++iter) {
		POINT partitionOfPath1 = (*iter)->GetPartitions(0);
		POINT partitionOfPath2 = (*iter)->GetPartitions(1);
		if ((*iter)->GetFirstVisit() == false) {
			(*iter)->Render(_hdc, _destArea.left, _destArea.top, scaleWidth, scaleHeight);
		}
	}
}

// check collisions between player and walls, pillars.
// update minimap and revise player`s position.
void Map::CollisionDetect(Player& _player, HDC _hdc, RECT _destArea) {
	int areaWidth = _destArea.right - _destArea.left;
	int areaHeight = _destArea.bottom - _destArea.top;
	float scaleWidth = (float)areaWidth / (MAP_SIZE * PARTITION_SIZE * TILE_SIZE);
	float scaleHeight = (float)areaHeight / (MAP_SIZE * PARTITION_SIZE * TILE_SIZE);

	POINT partitionOfPlayer = POINT{
		_player.GetPos().x / (PARTITION_SIZE * TILE_SIZE),
		_player.GetPos().y / (PARTITION_SIZE * TILE_SIZE)
	};

	// update minimap and revise player`s position.
	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			if (map[i][j] != NULL) {
				POINT partitionOfMap = map[i][j]->GetPosInMap();
				if (partitionOfMap.x >= partitionOfPlayer.x - 1 &&
					partitionOfMap.x <= partitionOfPlayer.x + 1 &&
					partitionOfMap.y >= partitionOfPlayer.y - 1 &&
					partitionOfMap.y <= partitionOfPlayer.y + 1
				) {
					map[i][j]->CollisionDetect(_player, _hdc, _destArea.left, _destArea.top, scaleWidth, scaleHeight);
				}
			}
		}
	}
	for (auto iter = pathVec.begin(); iter != pathVec.end(); ++iter) {
		POINT partitionOfPath1 = (*iter)->GetPartitions(0);
		POINT partitionOfPath2 = (*iter)->GetPartitions(1);

		if (
			partitionOfPath1.x >= partitionOfPlayer.x - 1 &&
			partitionOfPath1.x <= partitionOfPlayer.x + 1 &&
			partitionOfPath1.y >= partitionOfPlayer.y - 1 &&
			partitionOfPath1.y <= partitionOfPlayer.y + 1 &&
			partitionOfPath2.x >= partitionOfPlayer.x - 1 &&
			partitionOfPath2.x <= partitionOfPlayer.x + 1 &&
			partitionOfPath2.y >= partitionOfPlayer.y - 1 &&
			partitionOfPath2.y <= partitionOfPlayer.y + 1
		) {
			(*iter)->CollisionDetect(_player, _hdc, _destArea.left, _destArea.top, scaleWidth, scaleHeight);
		}
	}
}

// check collisions with bullets and set destroyed.
void Map::CollisionDetect(Bullet* _bullet) {
	POINT partitionOfBullet = POINT{
		_bullet->GetPos().x / (PARTITION_SIZE * TILE_SIZE),
		_bullet->GetPos().y / (PARTITION_SIZE * TILE_SIZE)
	};

	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			if (map[i][j] != NULL) {
				POINT partitionOfMap = map[i][j]->GetPosInMap();
				if (partitionOfMap.x >= partitionOfBullet.x - 1 &&
					partitionOfMap.x <= partitionOfBullet.x + 1 &&
					partitionOfMap.y >= partitionOfBullet.y - 1 &&
					partitionOfMap.y <= partitionOfBullet.y + 1
				) {
					map[i][j]->CollisionDetect(_bullet);
				}
			}
		}
	}
	for (auto iter = pathVec.begin(); iter != pathVec.end(); ++iter) {
		POINT partitionOfPath1 = (*iter)->GetPartitions(0);
		POINT partitionOfPath2 = (*iter)->GetPartitions(1);

		if (
			partitionOfPath1.x >= partitionOfBullet.x - 1 &&
			partitionOfPath1.x <= partitionOfBullet.x + 1 &&
			partitionOfPath1.y >= partitionOfBullet.y - 1 &&
			partitionOfPath1.y <= partitionOfBullet.y + 1 &&
			partitionOfPath2.x >= partitionOfBullet.x - 1 &&
			partitionOfPath2.x <= partitionOfBullet.x + 1 &&
			partitionOfPath2.y >= partitionOfBullet.y - 1 &&
			partitionOfPath2.y <= partitionOfBullet.y + 1
		) {
			(*iter)->CollisionDetect(_bullet);
		}
	}
}

void Map::PrintMap() const {
	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			wprintf_s(L"%d\t", (map[j][i] == NULL) ? -1 : map[j][i]->GetId());
		}
		wprintf_s(L"\n");
	}
	wprintf_s(L"\n");
	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			if (map[j][i] != NULL) map[j][i]->PrintRoom();
			wprintf_s(L"\n");
		}
	}
}

POINT Map::GetStartPlayerPos() const {
	for (int i = 0; i < MAP_SIZE; ++i) {
		for (int j = 0; j < MAP_SIZE; ++j) {
			if (map[i][j] != NULL && map[i][j]->GetId() == 0)
				return map[i][j]->GetCenterPos();
		}
	}
	return POINT();
}

Room* Map::FindRoom(int _id) {
	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
			if (map[i][j] != NULL && map[i][j]->GetId() == _id)
				return map[i][j];
	return NULL;
}

void Map::ConnectRooms(int _id) {
	Room* postRoom = FindRoom(_id);
	Room* prevRoom = FindRoom(postRoom->GetPrevId());
	POINT postRoomPosInMap = postRoom->GetPosInMap();
	POINT prevRoomPosInMap = prevRoom->GetPosInMap();

	POINT pos;
	int width, height;
	POINT entranceLt, entranceRb;
	bool horizontal;

	if (postRoomPosInMap.x == prevRoomPosInMap.x) {
		horizontal = false;
		POINT upperEntrancePos;
		POINT lowerEntrancePos;

		if (postRoomPosInMap.y < prevRoomPosInMap.y) {
			upperEntrancePos = postRoom->GetEntrancePosBottom();
			lowerEntrancePos = prevRoom->GetEntrancePosTop();
			postRoom->OpenEntranceBottom();
			prevRoom->OpenEntranceTop();
		}
		else {
			upperEntrancePos = prevRoom->GetEntrancePosBottom();
			lowerEntrancePos = postRoom->GetEntrancePosTop();
			prevRoom->OpenEntranceBottom();
			postRoom->OpenEntranceTop();
		}

		pos = POINT{
			(upperEntrancePos.x < lowerEntrancePos.x) ? upperEntrancePos.x - 1 : lowerEntrancePos.x - 1,
			upperEntrancePos.y + 1
		};
		width = abs(upperEntrancePos.x - lowerEntrancePos.x) + 3;
		height = lowerEntrancePos.y - upperEntrancePos.y - 1;
		entranceLt = POINT{upperEntrancePos.x - pos.x, 0};
		entranceRb = POINT{lowerEntrancePos.x - pos.x, height - 1};
	}
	else {
		horizontal = true;
		POINT leftEntrancePos;
		POINT rightEntrancePos;

		if (postRoomPosInMap.x < prevRoomPosInMap.x) {
			leftEntrancePos = postRoom->GetEntrancePosRight();
			rightEntrancePos = prevRoom->GetEntrancePosLeft();
			postRoom->OpenEntranceRight();
			prevRoom->OpenEntranceLeft();
		}
		else {
			leftEntrancePos = prevRoom->GetEntrancePosRight();
			rightEntrancePos = postRoom->GetEntrancePosLeft();
			prevRoom->OpenEntranceRight();
			postRoom->OpenEntranceLeft();
		}

		pos = POINT{
			leftEntrancePos.x + 1,
			(leftEntrancePos.y < rightEntrancePos.y) ? leftEntrancePos.y - 1 : rightEntrancePos.y - 1
		};
		width = rightEntrancePos.x - leftEntrancePos.x - 1;
		height = abs(rightEntrancePos.y - leftEntrancePos.y) + 3;
		entranceLt = POINT{0, leftEntrancePos.y - pos.y};
		entranceRb = POINT{ width - 1, rightEntrancePos.y - pos.y };
	}

	pathVec.push_back(
		new Path{
			pos, width, height,
			postRoomPosInMap, prevRoomPosInMap,
			postRoom->GetId(), prevRoom->GetId(),
			entranceLt, entranceRb,
			horizontal
		}
	);
}

void Map::CloseRoom() {
	for (auto iter = pathVec.begin(); iter != pathVec.end(); ++iter)
		(*iter)->Close();
}

void Map::OpenRoom() {
	for (auto iter = pathVec.begin(); iter != pathVec.end(); ++iter)
		(*iter)->Open();
}
