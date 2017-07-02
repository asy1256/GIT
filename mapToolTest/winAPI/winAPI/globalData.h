#pragma once

#define TILESIZE 64
#define TILESIZE_MINI 6
#define TILESIZE_HARF (TILESIZE / 2)
#define TILESIZE_MINI_HARF (TILESIZE_MINI / 2)
#define TILEX 150
#define TILEY 150
#define TILEWIDTH (TILEX * TILESIZE)
#define TILEWIDTH_MINI (TILEX * TILESIZE_MINI)
#define TILEHEIGHT (TILEY * TILESIZE)
#define TILEHEIGHT_MINI (TILEY * TILESIZE_MINI)

enum TERRAIN
{
	EMPTY,
	STON_FLOOR,
	WOOD_FLOOR,
	STON_WALL,
	BOOK_WALL,
	CARPET
};

enum OBJECT
{
	NONE,
	BLANK,
	WOOD_BARREL,
	BOOM_BARREL,
	TABLE_WIDTH,
	TABLE_LENGTH
};

struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;
	bool pass;
	int roomnum, sponSequence;
	int terrainX, terrainY;
	int objframeX, objframeY;
};