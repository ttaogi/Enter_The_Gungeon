#pragma once

#include <queue>
#include <vector>

#include "Path/Path.h"
#include "Room/Room.h"
#include "../MonoBehaviour/Bullet/Bullet.h"
#include "../MonoBehaviour/Player/Player.h"

class BattleStageSceneManager;

class Map {
private:
	Room* map[MAP_SIZE][MAP_SIZE]; // [x][y]
	std::vector<Path*> pathVec;
public:
	Map(BattleStageSceneManager* _scnMgr);
	~Map();

	void Render(HDC _hdc, POINT _coordinateRevision);
	void Render(HDC _hdc, RECT _destArea);
	void CollisionDetect(Player &_player, HDC _hdc, RECT _destArea);
	void CollisionDetect(Bullet* _bullet);

	void PrintMap() const;
	POINT GetStartPlayerPos() const;
	Room* FindRoom(int _id);
	void ConnectRooms(int _id);
	void CloseRoom();
	void OpenRoom();
};