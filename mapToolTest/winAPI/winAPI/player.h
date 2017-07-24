#pragma once

#include "character.h"
#include "bullets.h"

class objectManager;

class player : public character
{
private:
	struct tagPlayer
	{
		int money;
		int blankshot;
		int key;
	};

private:
	tagPlayer _pl;
	fireDirection _direc;
	int _baseframeX, _baseframeY;
	int _degree, _ammo, _blankcount;
	float _reloadtime, _currentreloadtime, _nowloading;
	bool _dodge, _move, _fire, _bfire, _reload;

	image* _reloadgage;
	image* _reloadbar;

	RECT _reloadingbar;

	bullet* _bullet;
	objectManager* _obm;
public:
	bool _right, _up, _left, _down;
public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void keycontrol(void);
	virtual void dodgeup(void);
	virtual void angleup(void);
	virtual void frameup(void);
	virtual void move(int direction);
	virtual void colision(void);

	virtual tagCharacter &getCharacterData(void) { return _ch; }
	virtual tagPlayer &getPlayerData(void) { return _pl; }
	virtual bool &getBlankshot(void) { return _bfire; }
	virtual int &getammo(void) { return _ammo; }

	virtual void setObject(objectManager* &obm) { _obm = obm; }

	player();
	virtual ~player();
};