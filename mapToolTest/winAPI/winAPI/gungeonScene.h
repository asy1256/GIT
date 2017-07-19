#pragma once
#include "gameNode.h"
#include "camera.h"
#include "characterManager.h"
#include "objectManager.h"

class gungeonScene : public gameNode
{
private:
	image* _cimg;
	image* _field;
	image* _miniimg;
	image* _sample;
	image* _black;
	image* _minimapcase;

	tagTile _tile[TILEY][TILEX];

	POINT _campt;
	POINT _minimappt, _basept;

	bool _mapOpen;

	camera* _cam;
	characterManager* _cm;
	objectManager* _om;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual HDC getBackDC(void) { return _cimg->getMemDC(); }

	virtual void setup(void);
	virtual void keycontrol(void);
	virtual void mousemove(void);
	virtual void draw(void);
	virtual void minimap(void);

	gungeonScene();
	virtual ~gungeonScene();
};

