#pragma once
#include "character.h"
class bulletKing : public character
{
private:
	tagMonsters _bk;

public:
	virtual HRESULT init(float x, float y, OBJECT obj, int roomnum, int spawn, int* plX, int* plY, bool* blankshot);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void move(void);
	virtual void angleup(void);
	virtual void frameup(void);
	virtual void sequenceup(void);

	virtual tagCharacter &getCharacterData(void) { return _ch; }
	virtual tagMonsters &getMonsterData(void) { return _bk; }

	bulletKing();
	virtual ~bulletKing();
};