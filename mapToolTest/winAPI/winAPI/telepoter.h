#pragma once
#include "Object.h"
class telepoter : public Object
{
private:
	struct tagTpData
	{
		bool action, turnon, activated;
	};

private:
	tagTpData _tp;

public:
	virtual HRESULT init(float x, float y, OBJECT type);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void frameup(void);

	virtual tagObject &getObjectData(void) { return _ob; }
	virtual tagTpData &getTpData(void) { return _tp; }

	telepoter();
	virtual ~telepoter();
};

