#pragma once

#include "../Tile/Tile.h"
#include "../../MonoBehaviour/Bullet/Bullet.h"

class Path {
private:
	POINT pos;
	int width, height;
	POINT partitions[2];
	int rooms[2];
	POINT entrances[2];
	TILE** map;
	bool firstVisit;
public:
	Path(
		POINT _pos, int _width, int _height,
		POINT _part1, POINT _part2,
		int _room1, int _room2,
		POINT _entranceLt, POINT _entranceRb,
		bool _horizontal
	);
	~Path();

	void Render(HDC _hdc, POINT _coordinateRevision);
	void Render(HDC _hdc, int _destX, int _destY, float _scaleWidth, float _scaleHeight);

	void CollisionDetect(Player& _player, HDC _hdc, int _destX, int _destY, float _scaleWidth, float _scaleHeight);
	void CollisionDetect(Bullet* _bullet);

	POINT GetPartitions(int _idx) { if (_idx >= 0 && _idx <= 1)return partitions[_idx]; else return POINT{ -1, -1 }; }
	bool GetFirstVisit() const { return firstVisit; }
	void SetFirstVisit(bool _firstVisit) { firstVisit = _firstVisit; }

	void Close();
	void Open();
	void Print() const;
};