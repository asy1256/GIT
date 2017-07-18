#pragma once
#include "gameNode.h"
#include "player.h"

class camera : public gameNode
{
private:
	image* _img;
	POINT* _pt;
	RECT _rc;

	player* _pl;

	bool _play;

public:
	virtual HRESULT init(POINT* pt, player* pl, image* img, bool play);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	RECT getRC(void) { return _rc; }

	camera();
	virtual ~camera();
};

