#pragma once

#include "../Tile/Tile.h"
#include "../../Manager/ImageManager/ImageManager.h"
#include "../../MonoBehaviour/Bullet/Bullet.h"
#include "../../MonoBehaviour/Player/Player.h"

#define MAP_SIZE			4
#define PARTITION_SIZE		16
#define ROOM_SIZE_MAX		12
#define ROOM_SIZE_MIN		8

#define ROOM_COUNT			8

#define ENEMY_COUNT_MIN		3
#define ENEMY_COUNT_MAX		5

enum class ROOM_TYPE {START, NORMAL, BOSS, SIZE};

class BattleStageSceneManager;
class Map;

class Room {
private:
	int prevId;
	int id;
	int width, height;
	int closeCount;
	bool firstVisit;
	ROOM_TYPE type;
	POINT posInMap;
	POINT posInPartition;
	RECT rcCheckPlayerVisit; // to check player visit.
	TILE roomMap[ROOM_SIZE_MAX][ROOM_SIZE_MAX]; // [x][y]
	BattleStageSceneManager* scnMgr;
	Map* map;
public:
	Room(int _prevId, int _id, POINT _posInMap, BattleStageSceneManager* _scnMgr, Map* _map);
	~Room();

	void Render(HDC _hdc, POINT _coordinateRevision);
	void Render(HDC _hdc, int _destX, int _destY, float _scaleWidth, float _scaleHeight);

	void CollisionDetect(Player& _player, HDC _hdc, int _destX, int _destY, float _scaleWidth, float _scaleHeight);
	void CollisionDetect(Bullet* _bullet);

	int GetPrevId() const { return prevId; }
	int GetId() const { return id; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	bool GetFirstVisit() const { return firstVisit; }
	void SetFirstVisit(bool _firstVisit) { firstVisit = _firstVisit; }
	POINT GetPosInMap() const { return posInMap; }
	POINT GetPosInPartition() const { return posInPartition; }
	RECT GetRcCheckPlayerVisit() const { return rcCheckPlayerVisit; }
	TILE GetRoomMap(int _x, int _y) const { return roomMap[_x][_y]; }

	POINT GetCenterPos() const;
	POINT GetEntrancePosLeft() const;
	POINT GetEntrancePosTop() const;
	POINT GetEntrancePosRight() const;
	POINT GetEntrancePosBottom() const;
	void OpenEntranceLeft();
	void OpenEntranceTop();
	void OpenEntranceRight();
	void OpenEntranceBottom();
	void PrintRoom() const;
	void CloseCountDecrease();
	void CloseRoom();
	void OpenRoom();
	void GameClear();
};