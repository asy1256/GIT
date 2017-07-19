#include "StdAfx.h"
#include "bullets.h"

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

void bullet::fire(float x, float y, float angle)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
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
	/*for (int i = 0; i < _vBullet.size(); ++i)
	{
		if (_vBullet[i].fire) { _vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed; }
		else { ++_vBullet[i].count; }
		_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y, _vBullet[i].img->getFrameWidth(), _vBullet[i].img->getFrameHeight());

		if (_range < getDistance(_vBullet[i].fireX, _vBullet[i].fireY, _vBullet[i].x, _vBullet[i].y) && _vBullet[i].fire)
		{
			_vBullet[i].fire = false;
		}
		if (!_vBullet[i].fire && _vBullet[i].count == 20) { _vBullet.erase(_vBullet.begin() + i); break; }
	}*/

	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->img->getWidth(), _viBullet->img->getHeight());

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
HRESULT bulletE::init(const char* imageName, float range)
{
	gameNode::init();

	_imageName = imageName;
	_range = range;

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

void bulletE::render(void)
{
	draw();
}

void bulletE::fire(float x, float y, float angle, float speed)
{
	tagBullet bullets;
	ZeroMemory(&bullets, sizeof(tagBullet));

	bullets.img = IMAGEMANAGER->findImage(_imageName);
	bullets.speed = speed;
	bullets.angle = angle;
	//bullets.radius = bullets.img->getWidth() / 2;
	bullets.x = bullets.fireX = x;
	bullets.y = bullets.fireY = y;
	bullets.rc = RectMakeCenter(bullets.x, bullets.y,
		bullets.img->getWidth(), bullets.img->getHeight());
	_vBullet.push_back(bullets);
}

void bulletE::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle) * _viBullet->speed;
		_viBullet->y += -sinf(_viBullet->angle) * _viBullet->speed;

		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->img->getWidth(), _viBullet->img->getHeight());

		//사거리 밖으로 나가면...
		if (_range < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet = _vBullet.erase(_viBullet++);
		}
		else ++_viBullet;
	}
}

void bulletE::draw(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->img->render(getMemDC(), _viBullet->rc.left, _viBullet->rc.top);
	}
}

void bulletE::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}