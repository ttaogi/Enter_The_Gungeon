#pragma warning(disable: 6385)
#pragma warning(disable: 6386)

#include "Stdafx/stdafx.h"

#include "Path.h"

Path::Path(
	POINT _pos, int _width, int _height,
	POINT _part1, POINT _part2, int _room1, int _room2,
	POINT _entranceLt, POINT _entranceRb,
	bool _horizontal
) {
	pos = _pos;
	width = _width;
	height = _height;
	partitions[0] = _part1;
	partitions[1] = _part2;
	rooms[0] = _room1;
	rooms[1] = _room2;
	entrances[0] = _entranceLt;
	entrances[1] = _entranceRb;
	firstVisit = true;

	map = (TILE**)malloc(_width * sizeof(TILE*));
	for (int i = 0; i < _width; ++i)
		map[i] = (TILE*)malloc(_height * sizeof(TILE));
	for (int i = 0; i < _width; ++i)
		for (int j = 0; j < _height; ++j)
			map[i][j] = TILE{
				TILE_TYPE::SIZE,
				RECT{
					(pos.x + i) * TILE_SIZE,
					(pos.y + j) * TILE_SIZE,
					(pos.x + i + 1) * TILE_SIZE,
					(pos.y + j + 1) * TILE_SIZE
				}
			};

	if (_horizontal == true) {
		for (int i = 0; i < width / 2; ++i) {
			map[i][_entranceLt.y].SetType(TILE_TYPE::GROUND);
			map[i][_entranceLt.y - 1].SetType(TILE_TYPE::WALL);
			map[i][_entranceLt.y + 1].SetType(TILE_TYPE::WALL);
			map[i + 1][_entranceLt.y - 1].SetType(TILE_TYPE::WALL);
			map[i + 1][_entranceLt.y].SetType(TILE_TYPE::WALL);
			map[i + 1][_entranceLt.y + 1].SetType(TILE_TYPE::WALL);
		}
		for (int i = width - 1; i > width / 2; --i) {
			map[i][_entranceRb.y].SetType(TILE_TYPE::GROUND);
			map[i][_entranceRb.y - 1].SetType(TILE_TYPE::WALL);
			map[i][_entranceRb.y + 1].SetType(TILE_TYPE::WALL);
			map[i - 1][_entranceRb.y - 1].SetType(TILE_TYPE::WALL);
			map[i - 1][_entranceRb.y].SetType(TILE_TYPE::WALL);
			map[i - 1][_entranceRb.y + 1].SetType(TILE_TYPE::WALL);
		}
		for (int i = ((_entranceLt.y < _entranceRb.y) ? _entranceLt.y : _entranceRb.y);
			i <= ((_entranceLt.y > _entranceRb.y) ? _entranceLt.y : _entranceRb.y);
			++i
		) {
			map[width / 2][i].SetType(TILE_TYPE::GROUND);

			if( i != ((_entranceLt.y < _entranceRb.y) ? _entranceLt.y : _entranceRb.y) &&
				i != ((_entranceLt.y > _entranceRb.y) ? _entranceLt.y : _entranceRb.y)
			) {
				map[width / 2 - 1][i].SetType(TILE_TYPE::WALL);
				map[width / 2 + 1][i].SetType(TILE_TYPE::WALL);
			}
		}
	}
	else {
		for (int i = 0; i < height / 2; ++i) {
			map[_entranceLt.x][i].SetType(TILE_TYPE::GROUND);
			map[_entranceLt.x - 1][i].SetType(TILE_TYPE::WALL);
			map[_entranceLt.x + 1][i].SetType(TILE_TYPE::WALL);
			map[_entranceLt.x][i + 1].SetType(TILE_TYPE::WALL);
			map[_entranceLt.x - 1][i + 1].SetType(TILE_TYPE::WALL);
			map[_entranceLt.x + 1][i + 1].SetType(TILE_TYPE::WALL);
		}
		for (int i = height - 1; i > height / 2; --i) {
			map[_entranceRb.x][i].SetType(TILE_TYPE::GROUND);
			map[_entranceRb.x - 1][i].SetType(TILE_TYPE::WALL);
			map[_entranceRb.x + 1][i].SetType(TILE_TYPE::WALL);
			map[_entranceRb.x][i - 1].SetType(TILE_TYPE::WALL);
			map[_entranceRb.x - 1][i - 1].SetType(TILE_TYPE::WALL);
			map[_entranceRb.x + 1][i - 1].SetType(TILE_TYPE::WALL);
		}
		for (int i = ((_entranceLt.x < _entranceRb.x) ? _entranceLt.x : _entranceRb.x);
			i <= ((_entranceLt.x > _entranceRb.x) ? _entranceLt.x : _entranceRb.x);
			++i
		) {
			map[i][height / 2].SetType(TILE_TYPE::GROUND);
			if(i != ((_entranceLt.x < _entranceRb.x) ? _entranceLt.x : _entranceRb.x) &&
				i != ((_entranceLt.x > _entranceRb.x) ? _entranceLt.x : _entranceRb.x)
			) {
				map[i][height / 2 - 1].SetType(TILE_TYPE::WALL);
				map[i][height / 2 + 1].SetType(TILE_TYPE::WALL);
			}
		}
	}
}

Path::~Path() {
	for (int i = 0; i < width; ++i)
		free(map[i]);
	free(map);
}

void Path::Render(HDC _hdc, POINT _coordinateRevision) {
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (map[i][j].GetBackImg() != NULL) {
				map[i][j].GetBackImg()->Render(
					_hdc,
					map[i][j].GetRect().left - _coordinateRevision.x,
					map[i][j].GetRect().top - _coordinateRevision.y
				);
			}
		}
	}
}

// for minimap rendering.
void Path::Render(HDC _hdc, int _destX, int _destY, float _scaleWidth, float _scaleHeight) {
	float scaledWidth = TILE_SIZE * _scaleWidth;
	float scaledHeight = TILE_SIZE * _scaleHeight;
	int destX = _destX + (int)(pos.x * TILE_SIZE * _scaleWidth);
	int destY = _destY + (int)(pos.y * TILE_SIZE * _scaleHeight);

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (map[i][j].GetBackImg() != NULL) {
				map[i][j].GetBackImg()->Render(
					_hdc,
					(int)(destX + i * scaledWidth),
					(int)(destY + j * scaledHeight),
					(int)scaledWidth,
					(int)scaledHeight
				);
			}
		}
	}
}

// update minimap and revise player`s position.
void Path::CollisionDetect(Player& _player, HDC _hdc,
	int _destX, int _destY, float _scaleWidth, float _scaleHeight
) {
	// update minimap.
	if (firstVisit == true) {
		RECT rcCheckPlayerVisit{ pos.x * TILE_SIZE, pos.y * TILE_SIZE, (pos.x + width) * TILE_SIZE, (pos.y + height) * TILE_SIZE };
		if (_player.IsCollided(&rcCheckPlayerVisit) == true) {
			firstVisit = false;
			// render to minimap.
			Render(_hdc, _destX, _destY, _scaleWidth, _scaleHeight);
		}
	}
	// handle collision with player. revise player`s position.
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (map[i][j].GetType() == TILE_TYPE::PILLAR || map[i][j].GetType() == TILE_TYPE::WALL) {
				RECT tileRc = map[i][j].GetRect();
				_player.MoveCollisionCheck(&tileRc);
			}
		}
	}
}

// check collisions with bullets and set destroyed.
void Path::CollisionDetect(Bullet* _bullet) {
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (map[i][j].GetType() == TILE_TYPE::PILLAR || map[i][j].GetType() == TILE_TYPE::WALL) {
				RECT tileRc = map[i][j].GetRect();
				if (_bullet->IsCollided(&tileRc) == true) {
					_bullet->SetDestroy(true);
					return;
				}
			}
		}
	}
}

void Path::Close() {
	map[entrances[0].x][entrances[0].y].SetType(TILE_TYPE::WALL);
	map[entrances[1].x][entrances[1].y].SetType(TILE_TYPE::WALL);
}

void Path::Open() {
	map[entrances[0].x][entrances[0].y].SetType(TILE_TYPE::GROUND);
	map[entrances[1].x][entrances[1].y].SetType(TILE_TYPE::GROUND);
}

void Path::Print() const {
	wprintf_s(L"pos : %d %d\twidth : %d\theight : %d\n", pos.x, pos.y, width, height);
	wprintf_s(L"part1 : %d %d\tpart2 : %d %d\n", partitions[0].x, partitions[0].y, partitions[1].x, partitions[1].y);
	wprintf_s(L"room1 : %d\t room2 : %d\n", rooms[0], rooms[1]);
	wprintf_s(L"entrances1 : %d %d\tentrances2 : %d %d\n", entrances[0].x, entrances[0].y, entrances[1].x, entrances[1].y);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			std::wcout << map[j][i].ToWString() << L"\t";
		wprintf_s(L"\n");
	}
}
