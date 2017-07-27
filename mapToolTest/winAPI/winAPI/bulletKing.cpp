#include "stdafx.h"
#include "bulletKing.h"

bulletKing::bulletKing()
{
}

bulletKing::~bulletKing()
{
}

HRESULT bulletKing::init(float x, float y, OBJECT obj, int roomnum, int spawn, int* plX, int* plY, bool* blankshot)
{
	character::init();

	_bk.img = IMAGEMANAGER->findImage("spawn");
	_bk.fcount = _bk.fX = 0;
	_bk.move = _bk.moving = _bk.shoting = false;
	_bk.moveangle = _bk.astarcount = 0;
	_bk.nextTile = 0;
	_bk.plX = plX;
	_bk.plY = plY;
	_bk.blankshot = blankshot;
	_bk.shotcount = 0;
	_bk.degree = 0;
	_bk.shotdely = RND->getFromIntTo(80, 110);

	_ch.sqence = HOLD;
	_ch.spawnnum = spawn;
	_ch.life = true;
	_ch.spawning = false;
	_ch.maxhp = _ch.hp = 3;
	_ch.x = x + TILESIZE / 2;
	_ch.y = y + TILESIZE / 2;
	_ch.roomnum = roomnum;
	_ch.obj = obj;
	_ch.img = IMAGEMANAGER->findImage("king");

	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);

	return S_OK;
}

void bulletKing::release(void)
{
	character::release();
}

void bulletKing::update(void)
{
	character::update();

	if (_ch.life)
	{
		if (_ch.sqence == SPAWN) { sequenceup(); }
		if (_ch.sqence == DONE)
		{
			angleup();
			frameup();
			move();

			_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
			_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);

			_ch.idX = _ch.x / TILESIZE;
			_ch.idY = (_ch.crc.top + 10) / TILESIZE;
		}
	}
}

void bulletKing::render(HDC hdc)
{
	if (_ch.sqence == SPAWN)
	{
		_bk.img->frameRender(hdc, _ch.rc.left - TILESIZE, _ch.rc.top, _bk.fX, 0);
	}
	if (_ch.sqence == DONE)
	{
		if (_ch.life)
		{

		}
		_ch.img->frameRender(hdc, _ch.rc.left, _ch.rc.top, _ch.frameX, _ch.frameY);
	}
}

void bulletKing::move(void)
{
	if (!_bk.move)
	{
		++_bk.astarcount;
		if (_bk.astarcount >= 5)
		{
			_bk.astarcount = 0;
			_bk.nextTile = 0;
			_Path = ASTAR->getPath(_ch.idX, _ch.idY, *_bk.plX, *_bk.plY);
			_bk.move = true;
		}
	}
	else
	{
		if (300 < getDistance(_ch.x, _ch.y, _Tile[*_bk.plY][*_bk.plX].rc.left + TILESIZE / 2, _Tile[*_bk.plY][*_bk.plX].rc.top + TILESIZE / 2))
		{
			int nidx = _Path[_bk.nextTile].x;
			int nidy = _Path[_bk.nextTile].y;
			_bk.moveangle = getAngle(_ch.x, _ch.y, _Tile[nidy][nidx].rc.left + TILESIZE / 2, _Tile[nidy][nidx].rc.top + TILESIZE / 2);
			_ch.x += cosf(_bk.moveangle) * 2.0f;
			_ch.y += -sinf(_bk.moveangle) * 2.0f;
			_bk.moving = true;

			if (2 > getDistance(_ch.x, _ch.y, _Tile[nidy][nidx].rc.left + TILESIZE / 2, _Tile[nidy][nidx].rc.top + TILESIZE / 2))
			{
				++_bk.astarcount;
				if (_bk.astarcount >= 20)
				{
					_bk.astarcount = 0;
					_bk.move = false;
				}
				++_bk.nextTile;
				if (_bk.nextTile >= _Path.size()) { _bk.move = false; }
			}
		}
		else { _bk.moving = false; }
	}
}

void bulletKing::angleup(void)
{
	float x = _Tile[*_bk.plY][*_bk.plX].rc.left + TILESIZE / 2;
	float y = _Tile[*_bk.plY][*_bk.plX].rc.top + TILESIZE / 2;

	_ch.angle = getAngle(_ch.x, _ch.y, x, y);
	_bk.degree = RADIAN_TO_DEGREE(_ch.angle);

	if ((_bk.degree >= 0 && _bk.degree <= 44)) { _ch.fdirec = RIGHT_UP; }
	else if ((_bk.degree >= 45 && _bk.degree <= 89)) { _ch.fdirec = UP; }
	else if ((_bk.degree >= 90 && _bk.degree <= 134)) { _ch.fdirec = LEFT_UP; }
	else if ((_bk.degree >= 135 && _bk.degree <= 179)) { _ch.fdirec = LEFT; }
	else if ((_bk.degree >= 180 && _bk.degree <= 224)) { _ch.fdirec = LEFT_DOWN; }
	else if ((_bk.degree >= 225 && _bk.degree <= 269)) { _ch.fdirec = DOWN; }
	else if ((_bk.degree >= 270 && _bk.degree <= 314)) { _ch.fdirec = RIGHT_DOWN; }
	else if ((_bk.degree >= 315 && _bk.degree <= 360)) { _ch.fdirec = RIGHT; }
}

void bulletKing::frameup(void)
{
	//걸어다니는거
	++_ch.framecount;
	if (_ch.framecount >= 12)
	{
		++_ch.frameX;
		_ch.framecount = 0;
		if (_ch.frameX >= 10) { _ch.frameX = 0; }
	}
	//총쏘는 대기시간
	if (!_bk.shoting)
	{
		++_bk.shotcount;
		if (_bk.shotcount >= _bk.shotdely)
		{
			_bk.shotcount = 0;
			_bk.shotdely = RND->getFromIntTo(80, 110);
			_bk.shoting = true;
		}
	}
	else
	{
		if (!*_bk.blankshot)
		{
			//총 프레임
			++_ch.gframecount;
			if (_ch.gframecount >= 5)
			{
				_ch.gframecount = 0;
				++_ch.gframeX;
				if (_ch.gframeX >= 4)
				{
					_ch.gframeX = 0;
					//_ch.fire = true;
					_bk.shoting = false;
				}
			}
		}
	}
}

void bulletKing::sequenceup(void)
{
	++_bk.fcount;
	if (_bk.fcount >= 6)
	{
		_bk.fcount = 0;
		++_bk.fX;
		if (_bk.fX >= 15) { _ch.sqence = DONE; }
	}
}