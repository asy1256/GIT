#pragma once
#include "gameNode.h"

struct tagCharacter
{
	int frameX, frameY, framecount;
	int gframeX, gframecount;
	int idX, idY, roomnum;
	float x, y, angle;
	RECT rc, crc, grc;
	OBJECT obj;
	image* img;
	image* gun;
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

