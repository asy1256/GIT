#pragma once
#include "Object.h"

class door : public Object
{
public:
	virtual HRESULT init(float x, float y, OBJECT type, int roomnum);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual tagObject &getObjectData(void) { return _ob; }

	virtual bool &getAction(void) { return _action; }
	virtual bool &getOpen(void) { return _open; }

	door();
	virtual ~door();
};

