#include "stdafx.h"
#include "camera.h"

camera::camera()
{
}

camera::~camera()
{
}

HRESULT camera::init(POINT* pt, image* img, bool play)
{
	gameNode::init();

	_pt = pt;
	_img = img;
	_play = play;
	_rc = RectMakeCenter(_pt->x, _pt->y, WINSIZEX, WINSIZEY);

	return S_OK;
}

void camera::release(void)
{
	gameNode::release();
}

void camera::update(void)
{
	gameNode::update();
	_rc = RectMakeCenter(_pt->x, _pt->y, WINSIZEX, WINSIZEY);

	if (_rc.left < 0)
	{
		_rc.left = 0;
		_rc.right = WINSIZEX;
	}
	if (_rc.right > TILEWIDTH)
	{
		_rc.right = TILEWIDTH;
		_rc.left = _rc.right - WINSIZEX;
	}
	if (_rc.top < 0)
	{
		_rc.top = 0;
		_rc.bottom = WINSIZEY;
	}
	if (_rc.bottom > TILEHEIGHT)
	{
		_rc.bottom = TILEHEIGHT;
		_rc.top = _rc.bottom - WINSIZEY;
	}
}

void camera::render(void)
{
	_img->render(getMemDC(), _rc.left, _rc.top, WINSIZEX, WINSIZEY);
}