#pragma once
#include "character.h"
class player : public character
{
private:

public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	inline tagCharacter &getCharacterData(void) { return _ch; }

	player();
	virtual ~player();
};