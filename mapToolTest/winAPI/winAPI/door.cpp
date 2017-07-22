#include "stdafx.h"
#include "door.h"

door::door()
{
}

door::~door()
{
}

HRESULT door::init(float x, float y, OBJECT type, int roomnum)
{
	Object::init(x, y, type, roomnum);
	_action = _open = _sight = false;
	_roomnum = roomnum;

	_ob.type = type;
	_ob.x = x;
	if (_ob.type == DOOR_WIDTH)
	{
		_ob.img = IMAGEMANAGER->findImage("wDoor");
		_ob.y = y - _ob.img->getFrameHeight() + TILESIZE;
		_ob.rc = RectMake(_ob.x, _ob.y, _ob.img->getFrameWidth(), _ob.img->getFrameHeight());
		_ob.crc = RectMake(_ob.x + 50, _ob.y, 20, _ob.img->getFrameHeight());
	}
	else if (_ob.type == DOOR_LENGTH)
	{
		_ob.img = IMAGEMANAGER->findImage("hDoor");
		_ob.y = y;
		_ob.rc = RectMake(_ob.x, _ob.y, _ob.img->getFrameWidth(), _ob.img->getFrameHeight());
		_ob.crc = RectMake(_ob.x, _ob.rc.bottom - 50, _ob.img->getFrameWidth(), 20);
	}

	return S_OK;
}

void door::release(void)
{
}

void door::update(void)
{
	if (_action)
	{
		++_ob.framecount;
		if (_ob.framecount >= 5)
		{
			_ob.framecount = 0;
			++_ob.frameX;
			if (_ob.frameX >= 4)
			{
				if (!_sight)
				{
					_sight = true;
					for (int y = 0; y < TILEY; ++y)
					{
						for (int x = 0; x < TILEX; ++x)
						{
							if (_Tile[y][x].roomnum != _roomnum) continue;
							_Tile[y][x].show = true;
						}
					}
				}
				_ob.frameX = 3;
				_action = false;
				_open = true;
			}
		}
	}

}

void door::render(HDC hdc)
{
	_ob.img->frameRender(hdc, _ob.rc.left, _ob.rc.top, _ob.frameX, _ob.frameY);
}