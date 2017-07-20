#include "stdafx.h"
#include "player.h"

player::player()
{
}

player::~player()
{
}

HRESULT player::init(float x, float y)
{
	character::init();
	_baseframeX = 8;
	_baseframeY = 1;
	_dodge = false;
	_move = false;
	_fire = false;
	_reload = false;
	_right = _up = _left = _down = false;
	_direc = DOWN;
	_ammo = 10;
	_currentreloadtime = 0;
	_reloadtime = 60;

	_reloadgage = IMAGEMANAGER->findImage("reloading");
	_reloadbar = IMAGEMANAGER->findImage("reloadingbar");

	_ch.x = x;
	_ch.y = y;
	_ch.img = IMAGEMANAGER->findImage("marin");
	_ch.gun = IMAGEMANAGER->findImage("mPistol");
	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
	_ch.grc = RectMakeCenter(_ch.rc.right, _ch.y, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());
	_ch.idX = _ch.x / TILESIZE;
	_ch.idY = _ch.crc.top + 10 / TILESIZE;

	_reloadingbar = RectMakeCenter(_ch.x, _ch.rc.top - _reloadgage->getHeight(), _reloadgage->getWidth(), _reloadgage->getHeight());
	_nowloading = (_currentreloadtime / _reloadtime) * _reloadgage->getWidth();

	_bullet = new bullet;
	_bullet->init(800);

	return S_OK;
}

void player::release(void)
{
	character::release();
	_bullet->release();
	SAFE_DELETE(_bullet);
}

void player::update(void)
{
	character::update();
	keycontrol();
	angleup();
	frameup();
	colision();

	_bullet->update();
	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
	if ((_degree <= 89 && _degree >= 0) || (_degree >= 270 && _degree <= 360))
	{
		_ch.grc = RectMakeCenter(_ch.rc.right + 10, _ch.y + 10, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());
	}
	else if ((_degree <= 269 && _degree >= 90))
	{
		_ch.grc = RectMakeCenter(_ch.rc.left - _ch.gun->getFrameWidth() + 10, _ch.y + 10, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());
	}
	_ch.idX = _ch.x / TILESIZE;
	_ch.idY = _ch.crc.top + 10 / TILESIZE;

	if (_reload)
	{
		_reloadingbar = RectMakeCenter(_ch.x, _ch.rc.top - _reloadgage->getHeight(), _reloadgage->getWidth(), _reloadgage->getHeight());
		_nowloading = (_currentreloadtime / _reloadtime) * _reloadgage->getWidth();
	}
}

void player::render(HDC hdc)
{
	if (!_dodge) { _ch.gun->frameRender(hdc, _ch.grc.left, _ch.grc.top, _ch.gframeX, 0); }
	if (_reload)
	{
		_reloadgage->render(hdc, _reloadingbar.left, _reloadingbar.top);
		_reloadbar->render(hdc, _reloadingbar.left + _nowloading, _reloadingbar.top);
	}
	_ch.img->frameRender(hdc, _ch.rc.left, _ch.rc.top, _ch.frameX, _ch.frameY);
	_bullet->render(hdc);
}

void player::keycontrol(void)
{
	//ÀÌµ¿
	if (!_dodge)
	{
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			_up = true;
			_move = true;
			_ch.y -= 6;
			if (_ch.y > (WINSIZEY / 2)) { _ptadd.y -= 6; }
		}
		if (KEYMANAGER->isOnceKeyUp('W')) { _up = false; }
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			_left = true;
			_move = true;
			_ch.x -= 6;
			if (_ch.x > (WINSIZEX / 2)) { _ptadd.x -= 6; }
		}
		if (KEYMANAGER->isOnceKeyUp('A')) { _left = false; }
		if (KEYMANAGER->isStayKeyDown('S'))
		{
			_down = true;
			_move = true;
			_ch.y += 6;
			if (_ch.y < (TILEHEIGHT - WINSIZEY / 2)) { _ptadd.y += 6; }
		}
		if (KEYMANAGER->isOnceKeyUp('S')) { _down = false; }
		if (KEYMANAGER->isStayKeyDown('D'))
		{
			_right = true;
			_move = true;
			_ch.x += 6;
			if (_ch.x < (TILEWIDTH - WINSIZEX / 2)) { _ptadd.x += 6; }
		}
		if (KEYMANAGER->isOnceKeyUp('D')) { _right = false; }
	}
	else
	{
		dodgeup();
	}

	if (!_right && !_up && !_left && !_down) { _move = false; }

	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		if (_ammo != 10 && !_reload) { _reload = true; }
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		if (!_dodge && _ammo > 0) { _fire = true; }
	}
	
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		if (!_dodge)
		{
			_ch.frameX = 0;
			if (_right && _up) { _ch.frameY = 2; }
			else if (_right && _down) { _ch.frameY = 7; }
			else if (_left && _up) { _ch.frameY = 4; }
			else if (_left && _down) { _ch.frameY = 5; }
			else if (_right) { _ch.frameY = 7; }
			else if (_up) { _ch.frameY = 3; }
			else if (_left) { _ch.frameY = 5; }
			else if (_down) { _ch.frameY = 6; }
		}
		_dodge = true;
	}
}

void player::dodgeup(void)
{
	if (_right && _up) { _ch.x += 8; _ch.y -= 8; _ptadd.x += 8; _ptadd.y -= 8; }
	else if (_right && _down) { _ch.x += 8; _ch.y += 8; _ptadd.x += 8; _ptadd.y += 8; }
	else if (_left && _up) { _ch.x -= 8; _ch.y -= 8; _ptadd.x -= 8; _ptadd.y -= 8; }
	else if (_left && _down) { _ch.x -= 8; _ch.y += 8; _ptadd.x -= 8; _ptadd.y += 8; }
	else if (_right) { _ch.x += 8; _ptadd.x += 8; }
	else if (_up) { _ch.y -= 8; _ptadd.y -= 8; }
	else if (_left) { _ch.x -= 8; _ptadd.x -= 8; }
	else if (_down) { _ch.y += 8; _ptadd.y += 8; }
}

void player::angleup(void)
{
	_ch.angle = getAngle(_ch.x, _ch.y, _mouse.x, _mouse.y);
	_degree = RADIAN_TO_DEGREE(_ch.angle);
	
	if ((_degree >= 0 && _degree <= 44))
	{
		_direc = RIGHT_UP;
		if (_move) { _baseframeY = 2; _baseframeX = 10; }
		else { _baseframeY = 0; _baseframeX = 12; }
	}
	else if ((_degree >= 45 && _degree <= 89))
	{
		_direc = UP;
		if (_move) { _baseframeY = 3; _baseframeX = 10; }
		else { _baseframeY = 0; _baseframeX = 8; }
	}
	else if ((_degree >= 90 && _degree <= 134))
	{
		_direc = LEFT_UP;
		if (_move) { _baseframeY = 3; _baseframeX = 10; }
		else { _baseframeY = 0; _baseframeX = 4; }
	}
	else if ((_degree >= 135 && _degree <= 179))
	{
		_direc = LEFT;
		if (_move) { _baseframeY = 4; _baseframeX = 10; }
		else { _baseframeY = 0; _baseframeX = 0; }
	}
	else if ((_degree >= 180 && _degree <= 224))
	{
		_direc = LEFT_DOWN;
		if (_move) { _baseframeY = 7; _baseframeX = 10; }
		else { _baseframeY = 1; _baseframeX = 0; }
	}
	else if ((_degree >= 225 && _degree <= 269))
	{
		_direc = DOWN;
		if (_move) { _baseframeY = 6; _baseframeX = 10; }
		else { _baseframeY = 1; _baseframeX = 4; }
	}
	else if ((_degree >= 270 && _degree <= 314))
	{
		_direc = RIGHT_DOWN;
		if (_move) { _baseframeY = 6; _baseframeX = 10; }
		else { _baseframeY = 1; _baseframeX = 8; }
	}
	else if ((_degree >= 315 && _degree <= 360))
	{
		_direc = RIGHT;
		if (_move) { _baseframeY = 5; _baseframeX = 10; }
		else { _baseframeY = 1; _baseframeX = 12; }
	}
	if (!_dodge) { _ch.frameY = _baseframeY; }
}

void player::frameup(void)
{
	if (_ch.frameX < _baseframeX && !_dodge) { _ch.frameX = _baseframeX; }
	if (_reload)
	{
		++_currentreloadtime;
		if (_currentreloadtime >= _reloadtime)
		{
			_currentreloadtime = 0;
			_reload = false;
			_ammo = 10;
		}
	}
	if (_fire && !_dodge)
	{
		++_ch.gframecount;
		if (_ch.gframecount >= 5)
		{
			++_ch.gframeX;
			_ch.gframecount = 0;
			if (_ch.gframeX >= 4)
			{
				_ch.gframeX = 0;
				--_ammo;
				_fire = false;
				_bullet->fire(_ch.x, _ch.y, _ch.angle);
			}
		}
	}
	if (_dodge)
	{
		++_ch.framecount;
		if (_ch.framecount >= 4)
		{
			++_ch.frameX;
			_ch.framecount = 0;
			if (_ch.frameX >= 9)
			{
				_ch.frameX = 0;
				_dodge = false;
				_right = _up = _left = _down = false;
			}
		}
	}
	else
	{
		if (_move)
		{
			++_ch.framecount;
			if (_ch.framecount >= 12)
			{
				++_ch.frameX;
				_ch.framecount = 0;
				if (_ch.frameX >= _baseframeX + 5)
				{
					_ch.frameX = _baseframeX;
				}
			}
		}
		else
		{
			++_ch.framecount;
			if (_ch.framecount >= 7)
			{
				++_ch.frameX;
				_ch.framecount = 0;
				if (_ch.frameX >= _baseframeX + 3)
				{
					_ch.frameX = _baseframeX;
				}
			}
		}
	}
}

void player::colision(void)
{

}

void player::lol(void)
{

}