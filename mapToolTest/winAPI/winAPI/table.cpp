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
	_tb.action = _tb.stand = _tb.hit =  false;
	_tb.left = _tb.right = _tb.up = _tb.down = false;
	_tb.hp = 14;
	_tb.hitcount = 0;
	_tb.hrc = RectMake(0, 0, 0, 0);

	_ob.type = type;
	if (_ob.type == TABLE_WIDTH)
	{
		_ob.img = IMAGEMANAGER->findImage("wTable");
		_ob.x = x + TILESIZE;
		_ob.y = y;
		_ob.crc = RectMakeCenter(_ob.x, _ob.y + 29, 124, 60);
	}
	else if (_ob.type == TABLE_LENGTH)
	{
		_ob.img = IMAGEMANAGER->findImage("hTable");
		_ob.x = x + TILESIZE / 2;
		_ob.y = y - 30;
		_ob.crc = RectMakeCenter(_ob.x, _ob.y + 22, 62, 120);
	}
	_ob.rc = RectMakeCenter(_ob.x, _ob.y, _ob.img->getFrameWidth(), _ob.img->getFrameHeight());
	_ob.borken = false;


	return S_OK;
}

void table::release(void)
{

}

void table::update(void)
{
	Object::update();
	standup();
	hitup();

	_ob.rc = RectMakeCenter(_ob.x, _ob.y, _ob.img->getFrameWidth(), _ob.img->getFrameHeight());
}

void table::render(HDC hdc)
{
	_ob.img->frameRender(hdc, _ob.rc.left, _ob.rc.top, _ob.frameX, _ob.frameY);
}

void table::standup(void)
{
	if (_tb.action && !_tb.stand)
	{
		++_ob.framecount;
		if (_ob.framecount >= 5)
		{
			_ob.framecount = 0;
			++_ob.frameX;
			if (_ob.frameX >= 6)
			{
				_tb.action = false;
				_tb.stand = true;

				int idX = _ob.x / TILESIZE;
				int idY = _ob.y / TILESIZE;

				if (_ob.type == TABLE_WIDTH)
				{
					_Tile[idY][idX].pass = true;
					_Tile[idY][idX - 1].pass = true;
				}
				else
				{
					_Tile[idY][idX].pass = true;
					_Tile[idY + 1][idX].pass = true;
				}
			}
		}
	}

	if (_tb.left && _tb.stand)
	{
		if (_ob.type == TABLE_WIDTH)
		{
			_ob.crc = RectMake(_ob.rc.left + 6, _ob.y + 14, 20, 60);
			_tb.hrc = RectMake(_ob.rc.left + 6, _ob.y - 10, 20, 84);
		}
		else
		{
			_ob.crc = RectMake(_ob.x - 44, _ob.y + 4, 20, 76);
			_tb.hrc = RectMake(_ob.x - 44, _ob.y - 6, 20, 86);
		}
	}
	if (_tb.right && _tb.stand)
	{
		if (_ob.type == TABLE_WIDTH)
		{
			_ob.crc = RectMake(_ob.rc.right - 26, _ob.y + 14, 20, 60);
			_tb.hrc = RectMake(_ob.rc.right - 26, _ob.y - 10, 20, 84);
		}
		else
		{
			_ob.crc = RectMake(_ob.x + 24, _ob.y + 4, 20, 76);
			_tb.hrc = RectMake(_ob.x + 24, _ob.y - 6, 20, 86);
		}
	}
	if (_tb.up && _tb.stand)
	{
		if (_ob.type == TABLE_WIDTH)
		{
			_ob.crc = RectMake(_ob.x - TILESIZE + 4, _ob.y - 10, 126, 30);
			_tb.hrc = RectMake(_ob.x - TILESIZE + 4, _ob.y - 10, 126, 30);
		}
		else
		{
			_ob.crc = RectMake(_ob.x - 30, _ob.y, 60, 20);
			_tb.hrc = RectMake(_ob.x - 30, _ob.y - 30, 60, 50);
		}
	}
	if (_tb.down && _tb.stand)
	{
		if (_ob.type == TABLE_WIDTH)
		{
			_ob.crc = RectMake(_ob.x - TILESIZE, _ob.rc.bottom - 30, 126, 30);
			_tb.hrc = RectMake(_ob.x - TILESIZE, _ob.rc.bottom - 30, 126, 30);
		}
		else
		{
			_ob.crc = RectMake(_ob.x - 30, _ob.rc.bottom - 20, 60, 20);
			_tb.hrc = RectMake(_ob.x - 30, _ob.rc.bottom - 50, 60, 50);
		}
	}
}

void table::hitup(void)
{
	if (_tb.hit)
	{
		++_tb.hitcount;
		if (_tb.hitcount >= 30)
		{
			_tb.hitcount = 0;
			_tb.hit = false;
		}
	}

	if (_ob.frameX != 11)
	{
		if (_tb.hp <= 0)
		{
			_ob.borken = true;
			_tb.hp = 0;
			++_ob.framecount;
			if (_ob.framecount >= 6)
			{
				_ob.framecount = 0;
				++_ob.frameX;
				if (_ob.frameX >= 12) { _ob.frameX = 11; }
			}
		}
		else if (_tb.hp <= 4)
		{
			_ob.frameX = 8;
		}
		else if (_tb.hp <= 11)
		{
			_ob.frameX = 7;
		}
	}
}