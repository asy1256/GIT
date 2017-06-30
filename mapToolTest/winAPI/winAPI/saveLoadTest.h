#pragma once

#include "gamenode.h"

class saveLoadTest : public gameNode
{
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void read(void);
	void write(void);

	saveLoadTest(void);
	~saveLoadTest(void);
};

