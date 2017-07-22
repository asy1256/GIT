#pragma once

#include "gameNode.h"
#include "door.h"
#include "table.h"

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
	
	inline vector<Object*> &getObjectvector(void) { return _vObject; }

	objectManager();
	virtual ~objectManager();
};

