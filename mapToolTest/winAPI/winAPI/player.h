#pragma once

#include "character.h"
#include "bullets.h"

class player : public character
{
private:
	enum direction
	{
		RIGHT,
		RIGHT_UP,
		UP,
		LEFT_UP,
		LEFT,
		LEFT_DOWN,
		DOWN,
		RIGHT_DOWN
	};

private:
	direction _direc;
	int _baseframeX, _baseframeY;
	int _degree, _ammo;
	float _reloadtime, _currentreloadtime, _nowloading;
	bool _dodge, _move, _fire, _reload;
	bool _right, _up, _left, _down;

	image* _reloadgage;
	image* _reloadbar;

	RECT _reloadingbar;

	bullet* _bullet;

public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void keycontrol(void);
	virtual void dodgeup(void);
	virtual void angleup(void);
	virtual void frameup(void);
	virtual void colision(void);
	virtual void lol(void);

	virtual tagCharacter &getCharacterData(void) { return _ch; }
	virtual int &getammo(void) { return _ammo; }

	player();
	virtual ~player();
};