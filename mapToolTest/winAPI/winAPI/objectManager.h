#pragma once

#include "gameNode.h"
#include "door.h"

class objectManager : public gameNode
{
private:
	vector<Object*> _vObject;
	vector<Object*>::iterator _viObject;

	Object* _temp;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void setup(void);

	objectManager();
	virtual ~objectManager();
};
