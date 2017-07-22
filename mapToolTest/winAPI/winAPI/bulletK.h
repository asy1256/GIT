#pragma once
#include "character.h"
class bulletK : public character
{
private:
	bool _move;
	int _astarcount;
	int _nextTile;
	float _moveangle;

public:
	virtual HRESULT init(float x, float y, OBJECT obj, int roomnum);
	virtual void release(void);
	virtual void update(int plX, int plY);
	virtual void render(HDC hdc);

	bulletK();
	virtual ~bulletK();
};