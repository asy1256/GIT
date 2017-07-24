#pragma once

#include "Object.h"

class table : public Object
{
private:
	struct tagTabledata
	{
		bool action, stand, hit;
		bool left, right, up, down;
		int hp;

		RECT hrc;
	};

private:
	tagTabledata _tb;

public:
	virtual HRESULT init(float x, float y, OBJECT type);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void standup(void);
	virtual void hitup(void);

	virtual tagObject &getObjectData(void) { return _ob; }
	virtual tagTabledata &getTableData(void) { return _tb; }

	table();
	virtual ~table();
};

