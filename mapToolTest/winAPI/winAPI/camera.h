#pragma once

#include "gameNode.h"

class camera : public gameNode
{
private:
	image* _img;
	POINT* _pt;
	RECT _rc;

public:
	virtual HRESULT init(POINT* pt, image* img);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	RECT getRC(void) { return _rc; }

	camera();
	virtual ~camera();
};

