#pragma once
#include "gameNode.h"

struct tagCharacter
{
	int frameX, frameY;
	float x, y;
	RECT rc;
	image* img;
};

class character : public gameNode
{
private:
	tagCharacter _ch;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	character();
	virtual ~character();
};

