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
	CARPET
};

enum OBJECT
{
	NONE,
	BLANK,
	STON_WALL,
	BOOK_WALL,
	WOOD_BARREL,
	BOOM_BARREL,
	TABLE_WIDTH,
	TABLE_LENGTH,
	BULLET_KIN,
	BULLET_BANDANA,
	SHOTGUN_RED,
	GUN_NUT
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