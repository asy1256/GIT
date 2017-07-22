#include "stdafx.h"
#include "table.h"

table::table()
{
}

table::~table()
{
}

HRESULT table::init(float x, float y, OBJECT type)
{
	Object::init(x, y, type);
	_tb.action = _tb.stand = false;
	_tb.left = _tb.right = _tb.up = _tb.down = false;
	_tb.hp = 14;
	_tb.hrc = RectMake(0, 0, 0, 0);

	_ob.type = type;
	if (_ob.type == TABLE_WIDTH)
	{
		_ob.img = IMAGEMANAGER->findImage("wTable");
		_ob.x = x + TILESIZE;
		_ob.y = y + TILESIZE / 2;
		_ob.crc = RectMakeCenter(_ob.x, _ob.y + 29, 94, 60);
	}
	else if (_ob.type == TABLE_LENGTH)
	{
		_ob.img = IMAGEMANAGER->findImage("hTable");
		_ob.x = x + TILESIZE / 2;
		_ob.y = y;
		_ob.crc = RectMakeCenter(_ob.x, _ob.y + 18, 62, 94);
	}
	_ob.rc = RectMakeCenter(_ob.x, _ob.y, _ob.img->getFrameWidth(), _ob.img->getFrameHeight());


	return S_OK;
}

void table::release(void)
{

}

void table::update(void)
{

}

void table::render(HDC hdc)
{
	Rectangle(hdc, _ob.crc.left, _ob.crc.top, _ob.crc.right, _ob.crc.bottom);
	_ob.img->frameRender(hdc, _ob.rc.left, _ob.rc.top, 0, 0);
}