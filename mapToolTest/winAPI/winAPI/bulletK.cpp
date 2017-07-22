#include "stdafx.h"
#include "bulletK.h"

bulletK::bulletK()
{
}

bulletK::~bulletK()
{
}

HRESULT bulletK::init(float x, float y, OBJECT obj, int roomnum)
{
	character::init();

	_move = false;
	_moveangle = _astarcount = 0;
	_nextTile = 0;

	_ch.x = x + TILESIZE / 2;
	_ch.y = y + TILESIZE / 2;
	_ch.roomnum = roomnum;
	_ch.obj = obj;
	_ch.img = IMAGEMANAGER->findImage("kin");
	_ch.gun = IMAGEMANAGER->findImage("kPistol");

	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
	_ch.grc = RectMake(_ch.rc.right, _ch.y, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());


	return S_OK;
}

void bulletK::release(void)
{
	character::release();
}

void bulletK::update(int plX, int plY)
{
	character::update();

	if (!_move)
	{
		++_astarcount;
		if (_astarcount >= 20)
		{
			_astarcount = 0;
			_nextTile = 0;
			_Path = ASTAR->getPath(_ch.idX, _ch.idY, plX, plY);
			_move = true;
		}
	}
	else
	{
		if (400 < getDistance(_ch.x, _ch.y, _Tile[plY][plX].rc.left + TILESIZE / 2, _Tile[plY][plX].rc.top + TILESIZE / 2))
		{
			int nidx = _Path[_nextTile].x;
			int nidy = _Path[_nextTile].y;
			_moveangle = getAngle(_ch.x, _ch.y, _Tile[nidy][nidx].rc.left + TILESIZE / 2, _Tile[nidy][nidx].rc.top + TILESIZE / 2);
			_ch.x += cosf(_moveangle) * 3;
			_ch.y += -sinf(_moveangle) * 3;

			if (2 > getDistance(_ch.x, _ch.y, _Tile[nidy][nidx].rc.left + TILESIZE / 2, _Tile[nidy][nidx].rc.top + TILESIZE / 2))
			{
				++_astarcount;
				if (_astarcount >= 60)
				{
					_move = false;
				}
				++_nextTile;
				if (_nextTile >= _Path.size()) { _move = false; }
			}
		}
	}

	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
	_ch.grc = RectMake(_ch.rc.right, _ch.y, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());

	_ch.idX = _ch.x / TILESIZE;
	_ch.idY = (_ch.crc.top + 10) / TILESIZE;
}

void bulletK::render(HDC hdc)
{
	_ch.img->frameRender(hdc, _ch.rc.left, _ch.rc.top, _ch.frameX, _ch.frameY);
	_ch.gun->frameRender(hdc, _ch.grc.left, _ch.grc.top, _ch.gframeX, 0);
}