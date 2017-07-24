#pragma once
#include "character.h"
class bulletK : public character
{
private:
	bool _move, _respon;
	int _astarcount;
	int _nextTile;
	float _moveangle;

public:
	virtual HRESULT init(float x, float y, OBJECT obj, int roomnum);
	virtual void release(void);
	virtual void update(int plX, int plY, bool blankshot);
	virtual void render(HDC hdc);

	virtual tagCharacter &getCharacterData(void) { return _ch; }
	virtual bool &getRespon(void) { return _respon; }

	bulletK();
	virtual ~bulletK();
};