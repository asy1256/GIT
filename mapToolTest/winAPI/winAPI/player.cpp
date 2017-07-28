#include "stdafx.h"
#include "player.h"
#include "objectManager.h"

player::player()
	:_obm(NULL)
{
}

player::~player()
{
}

HRESULT player::init(float x, float y)
{
	character::init();
	memset(&_pl, 0, sizeof(_pl));
	_baseframeX = 8;
	_baseframeY = 1;
	_dodge = false;
	_move = false;
	_fire = false;
	_bfire = false;
	_reload = false;
	_right = _up = _left = _down = false;
	_direc = DOWN;
	_ammo = 10;
	_currentreloadtime = _blankcount = 0;
	_reloadtime = 60;

	_reloadgage = IMAGEMANAGER->findImage("reloading");
	_reloadbar = IMAGEMANAGER->findImage("reloadingbar");

	_pl.blankshot = 2;
	_pl.money = 0;
	_pl.key = 1;
	_pl.hitcount = 0;
	_pl.teleporting = false;
	_pl.hit = false;

	_ch.life = true;
	_ch.maxhp = _ch.hp = 6;
	_ch.x = x;
	_ch.y = y;
	_ch.img = IMAGEMANAGER->findImage("marin");
	_ch.gun = IMAGEMANAGER->findImage("mPistol");
	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
	_ch.grc = RectMakeCenter(_ch.rc.right, _ch.y, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());
	_ch.idX = (_ch.crc.left + 20) / TILESIZE;
	_ch.idY = (_ch.crc.top + 10) / TILESIZE;

	_reloadingbar = RectMakeCenter(_ch.x, _ch.rc.top - _reloadgage->getHeight(), _reloadgage->getWidth(), _reloadgage->getHeight());
	_nowloading = (_currentreloadtime / _reloadtime) * _reloadgage->getWidth();

	return S_OK;
}

void player::release(void)
{
	character::release();
}

void player::update(void)
{
	character::update();
	keycontrol();
	angleup();
	frameup();
	colision();

	_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
	_ch.crc = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
	if ((_degree <= 89 && _degree >= 0) || (_degree >= 270 && _degree <= 360))
	{
		_ch.grc = RectMakeCenter(_ch.rc.right + 10, _ch.y + 10, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());
	}
	else if ((_degree <= 269 && _degree >= 90))
	{
		_ch.grc = RectMakeCenter(_ch.rc.left - _ch.gun->getFrameWidth() + 20, _ch.y + 10, _ch.gun->getFrameWidth(), _ch.gun->getFrameHeight());
	}
	_ch.idX = (_ch.crc.left + 20) / TILESIZE;
	_ch.idY = (_ch.crc.bottom - 5) / TILESIZE;

	if (_reload)
	{
		_reloadingbar = RectMakeCenter(_ch.x, _ch.rc.top - _reloadgage->getHeight(), _reloadgage->getWidth(), _reloadgage->getHeight());
		_nowloading = (_currentreloadtime / _reloadtime) * _reloadgage->getWidth();
	}
}

void player::render(HDC hdc)
{
	if (!_dodge)
	{
		if ((_degree >= 0 && _degree <= 89) || (_degree >= 270 && _degree <= 360))
		{
			_ch.gun->frameRender(hdc, _ch.grc.left, _ch.grc.top, _ch.gframeX, 0);
		}
		else
		{
			_ch.gun->frameRender(hdc, _ch.grc.left, _ch.grc.top, _ch.gframeX, 1);
		}
	}
	if (_reload)
	{
		_reloadgage->render(hdc, _reloadingbar.left, _reloadingbar.top);
		_reloadbar->render(hdc, _reloadingbar.left + _nowloading, _reloadingbar.top);
	}
	_ch.img->frameRender(hdc, _ch.rc.left, _ch.rc.top, _ch.frameX, _ch.frameY);
}

void player::keycontrol(void)
{
	//이동
	if (!_dodge)
	{
		if (KEYMANAGER->isStayKeyDown('S'))
		{
			_down = true;
			_move = true;
			move(4);
		}
		if (KEYMANAGER->isOnceKeyUp('S')) { _down = false; }
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			_up = true;
			_move = true;
			move(3);
		}
		if (KEYMANAGER->isOnceKeyUp('W')) { _up = false; }
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			_left = true;
			_move = true;
			move(2);
		}
		if (KEYMANAGER->isOnceKeyUp('A')) { _left = false; }
		if (KEYMANAGER->isStayKeyDown('D'))
		{
			_right = true;
			_move = true;
			move(1);
		}
		if (KEYMANAGER->isOnceKeyUp('D')) { _right = false; }
	}
	else
	{
		dodgeup();
	}

	if (!_right && !_up && !_left && !_down) { _move = false; }
	//공포탄
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		if (!_bfire && _pl.blankshot != 0)
		{
			SOUNDMANAGER->play("blankshoot");
			_bfire = true;
			DATABASE->blank = true; 
			DATABASE->bang = true;
			--_pl.blankshot;
		}
	}
	//밥상뒤집거나 뭐 등 등 해봅시다.
	if (KEYMANAGER->isOnceKeyDown('E'))
	{
		if (_obm != NULL)
		{
			for (int i = 0; i < _obm->getObjectvector().size(); ++i)
			{
				if (_obm->getObjectvector()[i]->getObjectData().borken) { continue; }
				RECT rc = RectMake(0, 0, 0, 0);
				if (_obm->getObjectvector()[i]->getObjectData().type == TABLE_WIDTH ||
					_obm->getObjectvector()[i]->getObjectData().type == TABLE_LENGTH)
				{
					table* temp = (table*)_obm->getObjectvector()[i];
					if (IntersectRect(&rc, &temp->getObjectData().rc, &_ch.rc) && !temp->getTableData().action && !temp->getTableData().stand)
					{
						SOUNDMANAGER->play("turntable");
						temp->getTableData().action = true;
						if (rc.top == temp->getObjectData().rc.top) { temp->getTableData().down = true; temp->getObjectData().frameY = 3; }
						if (rc.bottom == temp->getObjectData().rc.bottom) { temp->getTableData().up = true; temp->getObjectData().frameY = 1; }
						if (rc.right == temp->getObjectData().rc.right) { temp->getTableData().left = true; temp->getObjectData().frameY = 2; }
						if (rc.left == temp->getObjectData().rc.left) { temp->getTableData().right = true; temp->getObjectData().frameY = 0; }
						break;
					}
				}
				if (_obm->getObjectvector()[i]->getObjectData().type == TELEPOTER)
				{
					telepoter* tp = (telepoter*)_obm->getObjectvector()[i];
					if (IntersectRect(&rc, &_ch.crc, &tp->getObjectData().rc) && tp->getTpData().activated && tp->getTpData().turnon)
					{
						SOUNDMANAGER->play("teleport");

						tp->getTpData().activated = false;
						tp->getObjectData().frameY = 3;

						POINT temp;
						temp.x = _ch.x;
						temp.y = _ch.y;
						_ch.x = DATABASE->past.x;
						_ch.y = DATABASE->past.y;

						int wid = temp.x - _ch.x;
						int hei = temp.y - _ch.y;

						_ptadd.x -= wid;
						_ptadd.y -= hei;
						break;
					}
				}
			}
		}
	}
	//재장전
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		if (_ammo != 10 && !_reload)
		{
			SOUNDMANAGER->play("gunreload");
			_reload = true;
		}
	}
	//총쏘기
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		if (!_dodge && _ammo > 0) { _fire = true; }
	}
	//구르기
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		if (!_dodge)
		{
			SOUNDMANAGER->play("dodge");
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

	RECT rc = RectMake(0,0,0,0);
	//벽과충돌해서 막는다... 시발..?
	if (_right)
	{
		if (!_Tile[_ch.idY][_ch.idX + 1].pass &&
			IntersectRect(&rc, &_Tile[_ch.idY][_ch.idX + 1].rc, &_ch.crc))
		{
			_ch.x -= rc.right - rc.left + 7;
			_ptadd.x -= rc.right - rc.left + 7;
		}
	}
	if (_left)
	{
		if (!_Tile[_ch.idY][_ch.idX - 1].pass &&
			IntersectRect(&rc, &_Tile[_ch.idY][_ch.idX - 1].rc, &_ch.crc))
		{
			_ch.x += rc.right - rc.left + 7;
			_ptadd.x += rc.right - rc.left + 7;
		}
	}
	if (_down)
	{
		if (!_Tile[_ch.idY + 1][_ch.idX].pass &&
			IntersectRect(&rc, &_Tile[_ch.idY + 1][_ch.idX].rc, &_ch.crc))
		{
			_ch.y -= rc.bottom - rc.top + 7;
			_ptadd.y -= rc.bottom - rc.top + 7;
		}
	}
	if (_up)
	{
		if (!_Tile[_ch.idY - 1][_ch.idX].pass &&
			IntersectRect(&rc, &_Tile[_ch.idY - 1][_ch.idX].rc, &_ch.crc))
		{
			_ch.y += rc.bottom - rc.top;
			_ptadd.y += rc.bottom - rc.top;
		}
	}
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
	if (_pl.hit)
	{
		++_pl.hitcount;
		if (_pl.hitcount >= 60)
		{
			_pl.hitcount = 0;
			_pl.hit = false;
		}
	}
	if (_pl.teleporting)
	{
		_ch.x = DATABASE->moveTo.x;
		_ch.y = DATABASE->moveTo.y;

		int wid = DATABASE->moveTo.x - DATABASE->past.x;
		int hei = DATABASE->moveTo.y - DATABASE->past.y;

		_ptadd.x += wid;
		_ptadd.y += hei;
		_pl.teleporting = false;
	}
	if (_bfire)
	{
		++_blankcount;
		if (_blankcount >= 60)
		{
			_blankcount = 0;
			_bfire = false;
		}
	}
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
			//if (_ch.gframeX == 0) { _bullet->fire(_ch.x, _ch.y, _ch.angle, _direc); }
			++_ch.gframeX;
			_ch.gframecount = 0;
			if (_ch.gframeX >= 4)
			{
				_ch.gframeX = 0;
				--_ammo;
				_fire = false;
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

void player::move(int direction)
{
	RECT rcCollision;
	POINT tileIndex[2];
	int direc = direction;

	rcCollision = _ch.crc;

	switch (direc)
	{
	case 1:
		_ch.x += 6;
		_ptadd.x += 6;
		_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
		rcCollision = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
		break;
	case 2:
		_ch.x -= 6;
		_ptadd.x -= 6;
		_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
		rcCollision = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
		break;
	case 3:
		_ch.y -= 6;
		_ptadd.y -= 6;
		_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
		rcCollision = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
		break;
	case 4:
		_ch.y += 6;
		_ptadd.y += 6;
		rcCollision = RectMake(_ch.rc.left, _ch.rc.bottom - 20, _ch.img->getFrameWidth(), 20);
		break;
	}

	//검사할 타일 인덱스...
	switch (direc)
	{
	case 1:
		tileIndex[0] = PointMake(_ch.idX, _ch.idY);
		tileIndex[1] = PointMake(_ch.idX + 1, _ch.idY);
		break;
	case 2:
		tileIndex[0] = PointMake(_ch.idX, _ch.idY);
		tileIndex[1] = PointMake(_ch.idX - 1, _ch.idY);
		break;
	case 3:
		tileIndex[0] = PointMake(_ch.idX, _ch.idY);
		tileIndex[1] = PointMake(_ch.idX, _ch.idY - 1);
		break;
	case 4:
		tileIndex[0] = PointMake(_ch.idX, _ch.idY);
		tileIndex[1] = PointMake(_ch.idX, _ch.idY + 1);
		break;
	}

	for (int i = 0; i < 2; i++)
	{
		//쭝돌
		RECT rc = RectMake(0,0,0,0);
		if (!_Tile[tileIndex[i].y][tileIndex[i].x].pass &&
			IntersectRect(&rc, &_Tile[tileIndex[i].y][tileIndex[i].x].rc, &rcCollision))
		{
			switch (direc)
			{
			case 1:
				_ch.x -= rc.right - rc.left;
				_ptadd.x -= rc.right - rc.left;
				break;
			case 2:
				_ch.x += rc.right - rc.left;
				_ptadd.x += rc.right - rc.left;
				break;
			case 3:
				_ch.y += rc.bottom - rc.top;
				_ptadd.y += rc.bottom - rc.top;
				break;
			case 4:
				_ch.y -= rc.bottom - rc.top + 5;
				_ptadd.y -= rc.bottom - rc.top + 5;
				break;
			}
			_ch.rc = RectMakeCenter(_ch.x, _ch.y, _ch.img->getFrameWidth(), _ch.img->getFrameHeight());
			return;
		}
	}
}

void player::colision(void)
{
	for (int i = 0; i < _obm->getObjectvector().size(); ++i)
	{
		if (_obm->getObjectvector()[i]->getObjectData().borken) { continue; }
		if (_obm->getObjectvector()[i]->getObjectData().type == TABLE_WIDTH ||
			_obm->getObjectvector()[i]->getObjectData().type == TABLE_LENGTH)
		{
			RECT rc = RectMake(0, 0, 0, 0);
			table* tb = (table*)_obm->getObjectvector()[i];
			if (IntersectRect(&rc, &tb->getObjectData().crc, &_ch.crc) && tb->getTableData().stand && tb->getTableData().hp != 0)
			{
				int wid = rc.right - rc.left;
				int hei = rc.bottom - rc.top;
				if (wid > hei)
				{
					if (rc.top == tb->getObjectData().crc.top)
					{
						tb->getObjectData().y += hei / 2;
						_ch.y -= hei / 2;
						_ptadd.y -= hei / 2;
					}
					if (rc.bottom == tb->getObjectData().crc.bottom)
					{
						tb->getObjectData().y -= hei / 2;
						_ch.y += hei / 2;
						_ptadd.y += hei / 2;
					}
				}
				else
				{
					if (rc.left == tb->getObjectData().crc.left)
					{
						tb->getObjectData().x += wid / 2;
						_ch.x -= wid / 2;
						_ptadd.x -= wid / 2;
					}
					if (rc.right == tb->getObjectData().crc.right)
					{
						tb->getObjectData().x -= wid / 2;
						_ch.x += wid / 2;
						_ptadd.x += wid / 2;
					}
				}
				break;
			}
		}
	}
}