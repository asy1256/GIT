#pragma once
#include "gameNode.h"
#include "camera.h"
#include "characterManager.h"
#include "objectManager.h"

class gungeonScene : public gameNode
{
private:
	struct tagTel
	{
		RECT rc;
		float x, y;
		int* frameX;
		int* frameY;
		bool* turnon;
		bool* activated;
	};
private:
	image* _cimg;
	image* _field;
	image* _miniimg;
	image* _sample;
	image* _black;
	image* _minimapcase;
	image* _weaponcase;
	image* _sheel;
	image* _ui;
	image* _telepot;
	image* _icon;

private:

	RECT _sheelrc[12];
	vector<RECT> _telicon;
	vector<tagTel> _telimg;

	POINT _campt;
	POINT _minimappt, _basept;

	bool _mapOpen;
	bool _minimapdraw;

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
	virtual void iconup(void);
	virtual void mousemove(void);
	virtual void draw(void);
	virtual void minimap(void);

	virtual void objectColision(void);

	gungeonScene();
	virtual ~gungeonScene();
};