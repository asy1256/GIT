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
	_action = _stand = false;
	_hp = 14;

	_ob.x = x;
	_ob.y = y;
	_ob.type = type;
	if (_ob.type == TABLE_WIDTH) { _ob.img = IMAGEMANAGER->findImage("wTable"); }
	else if (_ob.type == TABLE_LENGTH) { _ob.img = IMAGEMANAGER->findImage("hTable"); }


	return S_OK;
}

void table::release(void)
{

}

void table::update(void)
{

}

void table::render(void)
{

}