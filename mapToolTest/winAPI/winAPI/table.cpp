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
	_action = _stand = false;
	_hp = 14;

	_ob.type = type;
	_ob.x = x;

	if (_ob.type == TABLE_WIDTH)
	{
		_ob.img = IMAGEMANAGER->findImage("wTable");
		_ob.y = y;
	}
	else if (_ob.type == TABLE_LENGTH)
	{
		_ob.img = IMAGEMANAGER->findImage("hTable");
		_ob.y = y - TILESIZE;
	}


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