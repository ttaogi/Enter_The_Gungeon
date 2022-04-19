#include "Stdafx/stdafx.h"

#include "Tile.h"

TILE::TILE() {
	type = TILE_TYPE::SIZE;
	rc = RECT{ 0, 0, 0, 0 };
	backImg = frontImg = NULL;
}

TILE::TILE(TILE_TYPE _type, RECT _rc) {
	rc = _rc;
	backImg = frontImg = NULL;
	SetType(_type);
}

void TILE::SetType(TILE_TYPE _type) {
	switch (_type) {
	case TILE_TYPE::GROUND:
		type = TILE_TYPE::GROUND;
		backImg = IMG->FindImage(KEY_TILE_GROUND);
		break;
	case TILE_TYPE::WALL:
		type = TILE_TYPE::WALL;
		backImg = IMG->FindImage(KEY_TILE_WALL);
		break;
	case TILE_TYPE::PILLAR:
		type = TILE_TYPE::PILLAR;
		backImg = IMG->FindImage(KEY_TILE_PILLAR);
		break;
	case TILE_TYPE::PILLAR_TOP:
		type = TILE_TYPE::PILLAR_TOP;
		backImg = IMG->FindImage(KEY_TILE_PILLAR_TOP);
		break;
	default:
		type = TILE_TYPE::SIZE;
		backImg = NULL;
		break;
	}
}

std::wstring TILE::ToWString() const {
	switch (type) {
	case TILE_TYPE::GROUND:
		return wstring(L"GROUND____");
	case TILE_TYPE::WALL:
		return wstring(L"WALL______");
	case TILE_TYPE::PILLAR:
		return wstring(L"PILLAR____");
	case TILE_TYPE::PILLAR_TOP:
		return wstring(L"PILLAR_TOP");
	default:
		return wstring(L"SIZE______");
	}
}
