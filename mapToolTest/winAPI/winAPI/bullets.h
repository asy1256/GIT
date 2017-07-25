#pragma once

#include "gamenode.h"
#include <vector>

class objectManager;
class characterManager;
class player;

struct tagBullet
{
	image* img;
	fireDirection direction;
	RECT rc;
	int currentX;
	float speed;
	float x, y;
	float fireX, fireY;
	float angle;
	int count;
	int who;
	bool fire;
};

class bullet : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;

public:
	virtual HRESULT init(float range);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void fire(float x, float y, float angle, fireDirection direction);
	virtual void move(void);
	virtual void remove(void);
	virtual void draw(HDC hdc);

	virtual vector<tagBullet> &getBulletVector(void) { return _vBullet; }
	virtual void setFire(bool fire, int arrnum) { _vBullet[arrnum].fire = fire; }

	bullet(void);
	virtual ~bullet(void);
};

class bulletE : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	bool* _plmove;

	objectManager* _om;
	characterManager* _cm;
	player* _pl;

public:
	virtual HRESULT init(float range, bool* move);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void fire(float x, float y, float angle, float speed, fireDirection direction, int who);
	virtual void move(void);
	virtual void draw(HDC hdc);

	virtual vector<tagBullet> &getBullet(void) { return _vBullet; }
	virtual void setObjectManager(objectManager* obm) { _om = obm; }
	virtual void setCharacterManager(characterManager* chm) { _cm = chm; }
	virtual void setPlayer(player* pl) { _pl = pl; }

	void removeBullet(int arrNum);

	bulletE(void);
	virtual ~bulletE(void);
};