#pragma once
#include "character.h"
class bulletK : public character
{
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	bulletK();
	virtual ~bulletK();
};

