#pragma once
#include "gameNode.h"

struct tagObject
{
	int frameX, frameY, framecount;
	float x, y;
	RECT rc, crc;
	image* img;
	OBJECT type;
};

class Object : public gameNode
{
protected:
	tagObject _ob;

public:
	virtual HRESULT init(float x, float y, OBJECT type, int roomnum);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	Object();
	virtual ~Object();
};

