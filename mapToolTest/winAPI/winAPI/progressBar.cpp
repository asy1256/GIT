#include "StdAfx.h"
#include "progressBar.h"


progressBar::progressBar(void)
{
}


progressBar::~progressBar(void)
{
}

HRESULT progressBar::init(int x, int y)
{
	gameNode::init();

	_x = x;
	_y = y;

	_barUp = IMAGEMANAGER->findImage("gaugeUp");
	_barDown = IMAGEMANAGER->findImage("gaugeDown");

	_rc = RectMake(_x, _y, _barDown->getWidth(), _barDown->getHeight());

	_width = _barUp->getWidth();

	return S_OK;
}

HRESULT progressBar::init(const char* barUp, const char* barDown, int x, int y)
{
	gameNode::init();

	_x = x;
	_y = y;

	_barUp = IMAGEMANAGER->findImage(barUp);
	_barDown = IMAGEMANAGER->findImage(barDown);

	_rc = RectMake(_x, _y, _barDown->getWidth(), _barDown->getHeight());

	_width = _barUp->getWidth();

	return S_OK;
}

void progressBar::release(void)
{
	gameNode::release();
}

void progressBar::update(void)
{
	gameNode::update();

	_rc = RectMake(_x, _y, _barDown->getWidth(), _barUp->getHeight());
}

void progressBar::render(void)
{
	_barDown->render(getMemDC(), _rc.left, _rc.top);
	_barUp->render(getMemDC(), _rc.left, _rc.top, 0, 0, _width,
		_barDown->getHeight());
}

void progressBar::setGuage(float current, float max)
{
	_width = (current / max) * _barDown->getWidth();
}
