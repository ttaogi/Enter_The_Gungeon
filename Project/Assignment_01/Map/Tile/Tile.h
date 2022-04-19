#pragma once

#define TILE_SIZE		128

enum class TILE_TYPE { GROUND, WALL, PILLAR, PILLAR_TOP, SIZE };

class TILE {
private:
	TILE_TYPE type;
	RECT rc;
	Image* backImg;
	Image* frontImg;
public:
	TILE();
	TILE(TILE_TYPE _type, RECT _rc);
	~TILE() {};

	TILE_TYPE GetType() const { return type; }
	void SetType(TILE_TYPE _type);
	RECT GetRect() const { return rc; }
	void SetRect(RECT _rc) { rc = _rc; }
	Image* GetBackImg() { return backImg; }
	void SetBackImg(Image* _img) { backImg = _img; }
	Image* GetFrontImg() { return frontImg; }
	void SetFrontImg(Image* _img) { frontImg = _img; }

	std::wstring ToWString() const;
};
