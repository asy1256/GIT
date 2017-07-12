#pragma once
#include "character.h"
class player : public character
{
private:

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	player();
	virtual ~player();
};

