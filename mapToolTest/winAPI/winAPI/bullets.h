#pragma once

#include "gamenode.h"
#include <vector>

struct tagBullet
{
	image* img;
	RECT rc;
	int currentX;
	float speed;
	float x, y;
	float fireX, fireY;
	float angle;
	int count;
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

	virtual void fire(float x, float y, float angle);
	virtual void move(void);
	virtual void remove(void);
	virtual void draw(HDC hdc);

	virtual vector<tagBullet> &getRC(void) { return _vBullet; }
	virtual void setFire(bool fire, int arrnum) { _vBullet[arrnum].fire = fire; }

	bullet(void);
	virtual ~bullet(void);
};

class bulletE : public gameNode
{
private:
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	const char* _imageName;
	float _range;

public:
	virtual HRESULT init(const char* imageName, float range);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void fire(float x, float y, float angle, float speed);
	virtual void move(void);
	virtual void draw(void);

	void removeBullet(int arrNum);

	bulletE(void);
	virtual ~bulletE(void);
};