#pragma once
#include "character.h"
class bulletB : public character
{
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	bulletB();
	virtual ~bulletB();
};

