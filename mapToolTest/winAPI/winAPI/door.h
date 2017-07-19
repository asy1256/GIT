#pragma once
#include "Object.h"

class door : public Object
{
private:
	int _roomnum;
	bool _action;
	bool _open;
	bool _sight;

public:
	virtual HRESULT init(float x, float y, OBJECT type, int roomnum);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	door();
	virtual ~door();
};

