#pragma once
#include "gameNode.h"

struct tagObject
{
	int frameX, frameY, framecount;
	int roomnum;
	bool borken;
	float x, y;
	RECT rc, crc;
	image* img;
	OBJECT type;
};

class Object : public gameNode
{
protected:
	tagObject _ob;

	int _roomnum;
	bool _action;
	bool _open;
	bool _sight;

public:
	virtual HRESULT init(float x, float y, OBJECT type, int roomnum);
	virtual HRESULT init(float x, float y, OBJECT type);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual tagObject &getObjectData(void) { return _ob; }

	virtual bool &getAction(void) { return _action; }
	virtual bool &getOpen(void) { return _open; }

	Object();
	virtual ~Object();
};

