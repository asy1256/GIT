#pragma once
#include "gameNode.h"

enum monsterSqence
{
	HOLD,
	SPAWN,
	DONE
};

struct tagCharacter
{
	int frameX, frameY, framecount;
	int gframeX, gframecount;
	int idX, idY, roomnum;
	int hp, maxhp;
	int spawnnum;
	float x, y, angle;
	bool fire;
	bool life;
	bool spawning;
	fireDirection fdirec;
	monsterSqence sqence;
	RECT rc, crc, grc;
	OBJECT obj;
	image* img;
	image* gun;
};

struct tagMonsters
{
	image* img;
	int fcount, shotcount;
	int fX, shotdely;
	int basefX, basefY;
	int degree;
	int* plX;
	int* plY;
	int astarcount;
	int nextTile;
	float moveangle;
	bool* blankshot;
	bool move;
	bool moving;
	bool shoting;
};


class character : public gameNode
{
protected:
	tagCharacter _ch;
	vector<POINT> _Path;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void update(int plX, int plY);
	virtual void render(HDC hdc);

	virtual tagCharacter &getCharacterData(void) { return _ch; }

	character();
	virtual ~character();
};

