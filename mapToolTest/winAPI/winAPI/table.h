#pragma once

#include "Object.h"

class table : public Object
{
private:
	bool _action;
	bool _stand;
	int _hp;

	RECT _hrc;

public:
	virtual HRESULT init(float x, float y, OBJECT type);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual RECT &getTrc(void) { return _hrc; }

	table();
	virtual ~table();
};

