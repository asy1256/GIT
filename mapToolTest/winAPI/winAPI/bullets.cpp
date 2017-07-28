#include "StdAfx.h"
#include "bullets.h"
#include "objectManager.h"
#include "characterManager.h"

bullet::bullet(void)
{
}

bullet::~bullet(void)
{
}

HRESULT bullet::init(float range)
{
	gameNode::init();

	_range = range;

	return S_OK;
}

void bullet::release(void)
{
	gameNode::release();

	remove();
}

void bullet::remove(void)
{
	_vBullet.clear();
}

void bullet::update(void)
{
	gameNode::update();

	move();
}

void bullet::render(HDC hdc)
{
	draw(hdc);
}

void bullet::fire(float x, float y, float angle, fireDirection direction)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.direction = direction;
	bullet.fire = true;
	bullet.count = 0;
	bullet.angle = angle;
	bullet.speed = 10.0f;
	bullet.img = IMAGEMANAGER->findImage("pBullet");
	bullet.x = bullet.fireX = x + cosf(angle) * (IMAGEMANAGER->findImage("mPistol")->getFrameWidth() + IMAGEMANAGER->findImage("marin")->getFrameWidth() / 2);
	bullet.y = bullet.fireY = y + (-sinf(angle) * IMAGEMANAGER->findImage("mPistol")->getFrameHeight());
	bullet.rc = RectMakeCenter(x, y, bullet.img->getFrameWidth(), bullet.img->getFrameHeight());
	_vBullet.push_back(bullet);
}

void bullet::move(void)
{
	int idX, idY;
	RECT rc = RectMake(0,0,0,0);
	
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->img->getWidth(), _viBullet->img->getHeight());

		//총알 벽에 부딪히는거 확인
		idX = _viBullet->x / TILESIZE;
		idY = _viBullet->y / TILESIZE;

		switch (_viBullet->direction)
		{
		case RIGHT:
			if (_Tile[idY][idX + 1].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX + 1].rc))
			{
				_viBullet->fire = false;
			}
			break;
		case RIGHT_UP:
			if (((_Tile[idY][idX + 1].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX + 1].rc)) ||
				(_Tile[idY - 1][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY - 1][idX].rc))) ||
				((_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc)) ||
				(_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc))))
			{
				_viBullet->fire = false;
			}
			break;
		case UP:
			if (_Tile[idY - 1][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY - 1][idX].rc))
			{
				_viBullet->fire = false;
			}
			break;
		case LEFT_UP:
			if (((_Tile[idY][idX - 1].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX - 1].rc)) ||
				(_Tile[idY - 1][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY - 1][idX].rc))) ||
				((_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc)) ||
				(_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc))))
			{
				_viBullet->fire = false;
			}
			break;
		case LEFT:
			if (_Tile[idY][idX - 1].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX - 1].rc))
			{
				_viBullet->fire = false;
			}
			break;
		case LEFT_DOWN:
			if (((_Tile[idY][idX - 1].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX - 1].rc)) ||
				(_Tile[idY + 1][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY + 1][idX].rc))) ||
				((_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc)) ||
				(_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc))))
			{
				_viBullet->fire = false;
			}
			break;
		case DOWN:
			if (_Tile[idY + 1][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY + 1][idX].rc))
			{
				_viBullet->fire = false;
			}
			break;
		case RIGHT_DOWN:
			if (((_Tile[idY][idX + 1].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX + 1].rc)) ||
				(_Tile[idY + 1][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY + 1][idX].rc))) ||
				((_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc)) ||
				(_Tile[idY][idX].wall != VOID_WALL && IntersectRect(&rc, &_viBullet->rc, &_Tile[idY][idX].rc))))
			{
				_viBullet->fire = false;
			}
			break;
		}

		//사거리 밖으로 나가면...
		if (_range < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y) || !_viBullet->fire)
		{
			_viBullet = _vBullet.erase(_viBullet++);
		}
		else ++_viBullet;
	}
}

void bullet::draw(HDC hdc)
{
	/*for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (_vBullet[i].fire) { _vBullet[i].img->frameRender(hdc, _vBullet[i].rc.left, _vBullet[i].rc.top, 0, 0); }
		else { _vBullet[i].img->frameRender(hdc, _vBullet[i].rc.left, _vBullet[i].rc.top, 1, 0); }
	}*/
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->img->render(hdc, _viBullet->rc.left, _viBullet->rc.top);
	}
}

bulletE::bulletE(void)
{
}

bulletE::~bulletE(void)
{
}

//공용으로 쓰는 총알 (쏠때마다 만들고 삭제한다)
HRESULT bulletE::init(float range, bool* move)
{
	gameNode::init();
	_range = range;
	_plmove = move;

	return S_OK;
}

void bulletE::release(void)
{
	gameNode::release();

	_vBullet.clear();
}

void bulletE::update(void)
{
	gameNode::update();

	move();
}

void bulletE::render(HDC hdc)
{
	draw(hdc);
}

void bulletE::fire(float x, float y, float angle, float speed, fireDirection direction, int who)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.who = who;
	bullet.fire = true;
	bullet.count = 0;
	bullet.angle = angle;
	bullet.direction = direction;
	bullet.speed = speed;
	if (who == 1) //플레이어
	{
		bullet.img = IMAGEMANAGER->findImage("pBullet");
		bullet.x = bullet.fireX = x + cosf(angle) * (IMAGEMANAGER->findImage("mPistol")->getFrameWidth() + IMAGEMANAGER->findImage("marin")->getFrameWidth() / 2);
		bullet.y = bullet.fireY = y + (-sinf(angle) * IMAGEMANAGER->findImage("mPistol")->getFrameHeight());
	}
	if (who == 2) //총탄KIN
	{
		bullet.img = IMAGEMANAGER->findImage("eBullet");
		bullet.x = bullet.fireX = x + cosf(angle) * (IMAGEMANAGER->findImage("kPistol")->getFrameWidth() + IMAGEMANAGER->findImage("kin")->getFrameWidth() / 2);
		bullet.y = bullet.fireY = y + (-sinf(angle) * IMAGEMANAGER->findImage("kPistol")->getFrameHeight());
	}
	bullet.rc = RectMakeCenter(x, y, bullet.img->getFrameWidth(), bullet.img->getFrameHeight());
	_vBullet.push_back(bullet);
}

void bulletE::move(void)
{
	RECT rc = RectMake(0,0,0,0);
	vector<character*> _vC = _cm->getCharacterVector();
	vector<Object*> _vO = _om->getObjectvector();
	table* tb;

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		if (*_plmove)
		{
			if (_viBullet->who == 1)
			{
				_viBullet->x += cosf(_viBullet->angle) * (_viBullet->speed + 6);
				_viBullet->y += -sinf(_viBullet->angle) * (_viBullet->speed + 6);
			}
			else
			{
				_viBullet->x += cosf(_viBullet->angle) * (_viBullet->speed);
				_viBullet->y += -sinf(_viBullet->angle) * (_viBullet->speed);
			}
		}
		else
		{
			_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
			_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;
		}

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->img->getWidth(), _viBullet->img->getHeight());

		//캐릭터 피격판정
		if (_viBullet->who == 1)
		{
			for (int i = 0; i < _vC.size(); ++i)
			{
				if (IntersectRect(&rc, &_vC[i]->getCharacterData().rc, &_viBullet->rc) && _vC[i]->getCharacterData().sqence == DONE &&
					_vC[i]->getCharacterData().life && _viBullet->fire && _vC[i]->getCharacterData().obj != NONE)
				{
					_viBullet->fire = false;
					--_vC[i]->getCharacterData().hp;
					if (_vC[i]->getCharacterData().hp <= 0)
					{
						SOUNDMANAGER->play("edie");
						_vC[i]->getCharacterData().hp = 0;
						_vC[i]->getCharacterData().life = false;
						_vC[i]->getCharacterData().frameX = 0;
					}
				}
			}
		}
		else
		{
			if (IntersectRect(&rc, &_pl->getCharacterData().rc, &_viBullet->rc) && _viBullet->fire &&
				_pl->getCharacterData().life && !_pl->_dodge)
			{
				_viBullet->fire = false;
				if (!_pl->getPlayerData().hit) { --_pl->getCharacterData().hp; }
				_pl->getPlayerData().hit = true;
				if (_pl->getCharacterData().hp <= 0)
				{
					_pl->getCharacterData().life = false;
				}
			}
		}

		for (int i = 0; i < _vO.size(); ++i)
		{
			if (_vO[i]->getObjectData().type == TABLE_WIDTH || _vO[i]->getObjectData().type == TABLE_LENGTH)
			{
				if (_vO[i]->getObjectData().borken) { continue; }
				tb = (table*)_vO[i];
				if (IntersectRect(&rc, &tb->getTableData().hrc, &_viBullet->rc) && tb->getTableData().stand && !tb->getObjectData().borken)
				{
					_viBullet->fire = false;
					if (!tb->getTableData().hit)
					{
						--tb->getTableData().hp;
						tb->getTableData().hit = true;
					}	
				}
			}
		}

		//사거리 밖으로 나가면...
		if (_range < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y) || !_viBullet->fire)
		{
			_viBullet = _vBullet.erase(_viBullet++);
		}
		else ++_viBullet;
	}
}

void bulletE::draw(HDC hdc)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->img->render(hdc, _viBullet->rc.left, _viBullet->rc.top);
	}
}

void bulletE::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}