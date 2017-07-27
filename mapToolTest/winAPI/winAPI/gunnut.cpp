#include "stdafx.h"
#include "gunnut.h"

gunnut::gunnut()
{
}

gunnut::~gunnut()
{
}

HRESULT gunnut::init(float x, float y, OBJECT obj, int roomnum, int spawn, int* plX, int* plY, bool* blankshot)
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
	_bk.shotdely = RND->getFromIntTo(180, 210);

	_ch.sqence = HOLD;
	_ch.spawnnum = spawn;
	_ch.life = true;
	_ch.spawning = false;
	_ch.maxhp = _ch.hp = 20;
	_ch.x = x + TILESIZE / 2;
	_ch.y = y + TILESIZE / 2;
	_ch.roomnum = roomnum;
	_ch.obj = obj;
	_ch.img = IMAGEMANAGER->findImage("gunnut");

	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth() / 2, _ch.img->getFrameHeight() / 2);
	_drc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth() / 2, 20);

	return S_OK;
}

void gunnut::release(void)
{
	character::release();
}

void gunnut::update(void)
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

			_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth() / 2, _ch.img->getFrameHeight() / 2);
			_drc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
			_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth() / 2, 20);

			_ch.idX = _ch.x / TILESIZE;
			_ch.idY = (_ch.crc.top + 10) / TILESIZE;
		}
	}
}

void gunnut::render(HDC hdc)
{
	if (_ch.sqence == SPAWN)
	{
		_bk.img->frameRender(hdc, _ch.rc.left + TILESIZE, _ch.rc.top + TILESIZE, _bk.fX, 0);
	}
	if (_ch.sqence == DONE)
	{
		if (!_bk.shoting) { _ch.img->frameRender(hdc, _drc.left, _drc.top, _ch.frameX, _ch.frameY); }
		else { _ch.img->frameRender(hdc, _drc.left, _drc.top, _ch.frameX, _bk.basefY); }
	}
	Rectangle(hdc, _ch.crc.left, _ch.crc.top, _ch.crc.right, _ch.crc.bottom);
}

void gunnut::move(void)
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
		if (200 < getDistance(_ch.x, _ch.y, _Tile[*_bk.plY][*_bk.plX].rc.left + TILESIZE / 2, _Tile[*_bk.plY][*_bk.plX].rc.top + TILESIZE / 2) || !_bk.shoting)
		{
			int nidx = _Path[_bk.nextTile].x;
			int nidy = _Path[_bk.nextTile].y;
			_bk.moveangle = getAngle(_ch.x, _ch.y, _Tile[nidy][nidx].rc.left + TILESIZE / 2, _Tile[nidy][nidx].rc.top + TILESIZE / 2);
			_ch.x += cosf(_bk.moveangle) * 1.5f;
			_ch.y += -sinf(_bk.moveangle) * 1.5f;
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

void gunnut::angleup(void)
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

	if ((_bk.degree >= 0 && _bk.degree <= 44) || (_bk.degree >= 315 && _bk.degree <= 365)) { _ch.frameY = 0; }
	else if ((_bk.degree >= 45 && _bk.degree <= 134)) { _ch.frameY = 1; }
	else if ((_bk.degree >= 135 && _bk.degree <= 224)) { _ch.frameY = 2; }
	else if ((_bk.degree >= 225 && _bk.degree <= 314)) { _ch.frameY = 3; }
}

void gunnut::frameup(void)
{
	if (!_bk.shoting)
	{
		//°É¾î´Ù´Ï´Â°Å
		++_ch.framecount;
		if (_ch.framecount >= 12)
		{
			++_ch.frameX;
			_ch.framecount = 0;
			if (_ch.frameX >= 7) { _ch.frameX = 1; }
		}
		//ÃÑ½î´Â ´ë±â½Ã°£
		++_bk.shotcount;
		if (_bk.shotcount >= _bk.shotdely)
		{
			_bk.shotcount = 0;
			_bk.shotdely = RND->getFromIntTo(180, 210);
			_bk.shoting = true;
			_ch.framecount = 0;
			_ch.frameX = 0;
			if (_ch.fdirec == RIGHT) { _bk.basefY = 4; }
			else if (_ch.fdirec == RIGHT_UP) { _bk.basefY = 5; }
			else if (_ch.fdirec == UP) { _bk.basefY = 6; }
			else if (_ch.fdirec == LEFT_UP) { _bk.basefY = 7; }
			else if (_ch.fdirec == LEFT) { _bk.basefY = 8; }
			else if (_ch.fdirec == LEFT_DOWN) { _bk.basefY = 9; }
			else if (_ch.fdirec == DOWN) { _bk.basefY = 10; }
			else if (_ch.fdirec == RIGHT_DOWN) { _bk.basefY = 11; }
			_sdirec = _ch.fdirec;
		}
	}
	else
	{
		//ÃÑ ½ò¶§
		++_ch.framecount;
		if (_ch.framecount >= 8)
		{
			if (_ch.framecount == 8)
			{
				if ((_sdirec == LEFT_DOWN || _sdirec == DOWN || _sdirec == RIGHT_DOWN) && _sdirec == 6) { _ch.fire = true; }
				else { if (_ch.frameX == 5) { _ch.fire = true; } }
			}
			_ch.framecount = 0;
			++_ch.frameX;
			if (_sdirec == LEFT_UP && _ch.frameX >= 9)
			{
				_ch.frameX = 0;
				_bk.shoting = false;
				if ((_bk.degree >= 0 && _bk.degree <= 89)) { _ch.frameY = 3; }
				else if ((_bk.degree >= 90 && _bk.degree <= 179)) { _ch.frameY = 2; }
				else if ((_bk.degree >= 180 && _bk.degree <= 269)) { _ch.frameY = 0; }
				else if ((_bk.degree >= 270 && _bk.degree <= 360)) { _ch.frameY = 1; }
			}
			else if ((_sdirec == RIGHT_UP || _ch.fdirec == UP) && _ch.frameX >= 10)
			{
				_ch.frameX = 0;
				_bk.shoting = false;
				if ((_bk.degree >= 0 && _bk.degree <= 89)) { _ch.frameY = 3; }
				else if ((_bk.degree >= 90 && _bk.degree <= 179)) { _ch.frameY = 2; }
				else if ((_bk.degree >= 180 && _bk.degree <= 269)) { _ch.frameY = 0; }
				else if ((_bk.degree >= 270 && _bk.degree <= 360)) { _ch.frameY = 1; }
			}
			else if (_sdirec == DOWN && _ch.frameX >= 12)
			{
				_ch.frameX = 0;
				_bk.shoting = false;
				if ((_bk.degree >= 0 && _bk.degree <= 89)) { _ch.frameY = 3; }
				else if ((_bk.degree >= 90 && _bk.degree <= 179)) { _ch.frameY = 2; }
				else if ((_bk.degree >= 180 && _bk.degree <= 269)) { _ch.frameY = 0; }
				else if ((_bk.degree >= 270 && _bk.degree <= 360)) { _ch.frameY = 1; }
			}
			else
			{
				if (_ch.frameX >= 11)
				{
					_ch.frameX = 0;
					_bk.shoting = false;
					if ((_bk.degree >= 0 && _bk.degree <= 89)) { _ch.frameY = 3; }
					else if ((_bk.degree >= 90 && _bk.degree <= 179)) { _ch.frameY = 2; }
					else if ((_bk.degree >= 180 && _bk.degree <= 269)) { _ch.frameY = 0; }
					else if ((_bk.degree >= 270 && _bk.degree <= 360)) { _ch.frameY = 1; }
				}
			}
		}
	}
}

void gunnut::sequenceup(void)
{
	++_bk.fcount;
	if (_bk.fcount >= 6)
	{
		_bk.fcount = 0;
		++_bk.fX;
		if (_bk.fX >= 15) { _ch.sqence = DONE; }
	}
}