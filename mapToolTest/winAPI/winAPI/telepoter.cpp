#include "stdafx.h"
#include "telepoter.h"

telepoter::telepoter()
{
}

telepoter::~telepoter()
{
}

HRESULT telepoter::init(float x, float y, OBJECT type, int roomnum)
{
	Object::init(x, y, type);

	_tp.action = _tp.activated = _tp.turnon = false;

	_ob.roomnum = roomnum;
	_ob.type = type;
	_ob.img = IMAGEMANAGER->findImage("telpo");
	_ob.x = x;
	_ob.y = y;
	_ob.rc = RectMake(_ob.x, _ob.y, _ob.img->getFrameWidth(), _ob.img->getFrameHeight());

	//_ob.frameY = 3;
	//_tp.turnon = true;

	return S_OK;
}

void telepoter::release(void)
{
}

void telepoter::update(void)
{
	frameup();
}

void telepoter::render(HDC hdc)
{
}

void telepoter::frameup(void)
{
	if (DATABASE->roomclear[_ob.roomnum - 1] && !_tp.action) { _tp.action = true; }

	//텔레포터 활성화
	if (_tp.action && !_tp.turnon)
	{
		++_ob.framecount;
		if (_ob.framecount >= 10)
		{
			_ob.framecount = 0;
			++_ob.frameX;
			if (_ob.frameX >= 5)
			{
				_ob.frameX = 0;
				++_ob.frameY;
				if (_ob.frameY >= 3)
				{
					_tp.action = false;
					_tp.turnon = true;
				}
			}
		}
	}
	//기본상태
	if (_tp.turnon && !_tp.activated)
	{
		++_ob.framecount;
		if (_ob.framecount >= 10)
		{
			_ob.framecount = 0;
			++_ob.frameX;
			if (_ob.frameX >= 5)
			{
				_ob.frameX = 0;
				++_ob.frameY;
				if (_ob.frameY >= 5)
				{
					_ob.frameY = 3;
				}
			}
		}
	}
	//텔레포터 작동시킨상태
	if (_tp.turnon && _tp.activated)
	{
		++_ob.framecount;
		if (_ob.framecount >= 10)
		{
			_ob.framecount = 0;
			++_ob.frameX;
			if (_ob.frameX >= 5)
			{
				_ob.frameX = 0;
				++_ob.frameY;
				if (_ob.frameY >= 7)
				{
					_ob.frameY = 5;
				}
			}
		}
	}
}