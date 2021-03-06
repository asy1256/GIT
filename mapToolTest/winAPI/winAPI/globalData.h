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

enum WALL
{
	VOID_WALL,
	STON_WALL,
	BOOK_WALL
};

enum OBJECT
{
	NONE,
	BLANK,
	WALL_TOP,
	DECO,
	WOOD_BARREL,
	BOOM_BARREL,
	TABLE_WIDTH,
	TABLE_LENGTH,
	BULLET_KIN,
	BULLET_BANDANA,
	BULLET_KING,
	SHOTGUN_RED,
	GUN_NUT,
	SHOP_MASTER,
	TELEPOTER,
	DOOR_WIDTH,
	DOOR_LENGTH
};

struct tagTile
{
	TERRAIN terrain;
	WALL wall;
	OBJECT obj;
	RECT rc, minirc;
	bool pass, show;
	int roomnum, sponSequence;
	int terrainX, terrainY;
	int wallX, wallY;
	int objframeX, objframeY;
};

enum fireDirection
{
	RIGHT,
	RIGHT_UP,
	UP,
	LEFT_UP,
	LEFT,
	LEFT_DOWN,
	DOWN,
	RIGHT_DOWN
};