#include "stdafx.h"
#include "player.h"

player::player()
{
}

player::~player()
{
}

HRESULT player::init(float x, float y)
{
	character::init();

	_ch.x = x;
	_ch.y = y;
	_ch.img = IMAGEMANAGER->findImage("Player");
	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	return S_OK;
}

void player::release(void)
{
	character::release();
}

void player::update(void)
{
	character::update();

	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
}

void player::render(HDC hdc)
{
	_ch.img->frameRender(hdc, _ch.rc.left, _ch.rc.top, 0, 0);
}