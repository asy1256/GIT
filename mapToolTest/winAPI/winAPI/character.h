#pragma once
#include "gameNode.h"

struct tagCharacter
{
	int frameX, frameY, framecount;
	int gframeX, gframecount;
	int idX, idY;
	float x, y, angle;
	RECT rc, crc, grc;
	image* img;
	image* gun;
};

class character : public gameNode
{
protected:
	tagCharacter _ch;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	character();
	virtual ~character();
};

