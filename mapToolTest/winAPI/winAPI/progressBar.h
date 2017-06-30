#pragma once

#include "gamenode.h"

class progressBar : public gameNode
{
private:
	RECT _rc;
	float _x, _y;
	float _width;

	image* _barDown;
	image* _barUp;

public:
	HRESULT init(int x, int y);
	HRESULT init(const char* barUp, const char* barDown, int x, int y);
	void release(void);
	void update(void);
	void render(void);

	void setGuage(float current, float max);
	void setX(int x) {_x = x;}
	void setY(int y) {_y = y;}

	RECT getRect(void) {return _rc;}

	progressBar(void);
	virtual ~progressBar(void);
};

