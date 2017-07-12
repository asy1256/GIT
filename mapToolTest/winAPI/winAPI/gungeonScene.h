#pragma once
#include "gameNode.h"
#include "camera.h"

class gungeonScene : public gameNode
{
private:
	image* _cimg;
	image* _miniimg;
	image* _sample;

	tagTile _tile[TILEY][TILEX];

	POINT _campt, _mouse, _ptadd;
	POINT _minimappt, _basept;

	bool _mapOpen;

	camera* _cam;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual HDC getBackDC(void) { return _cimg->getMemDC(); }

	virtual void setup(void);
	virtual void keycontrol(void);
	virtual void draw(void);
	virtual void minimap(void);

	gungeonScene();
	virtual ~gungeonScene();
};

