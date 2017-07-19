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
	int _degree;
	bool _dodge, _move, _fire;
	bool _right, _up, _left, _down;

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

	inline tagCharacter &getCharacterData(void) { return _ch; }

	player();
	virtual ~player();
};