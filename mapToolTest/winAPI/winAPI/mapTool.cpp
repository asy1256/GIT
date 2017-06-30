#include "stdafx.h"
#include "mapTool.h"

mapTool::mapTool()
{
}

mapTool::~mapTool()
{
}

HRESULT mapTool::init(void)
{
	gameNode::init();
	setup();

	return S_OK;
}

void mapTool::release(void)
{
	gameNode::release();
	_cam->release();
	SAFE_DELETE(_cam);
}

void mapTool::update(void)
{
	gameNode::update();
	keycontrol();
	mousemove();
	_cam->update();
}

void mapTool::render(void)
{
	draw();
	_cam->render();
}

void mapTool::setup(void)
{
	_sample = IMAGEMANAGER->addFrameImage("tile", "sampletile.bmp", 640, 640, 10, 10, true);
	_img = IMAGEMANAGER->addImage("bimg", "backImage.bmp", TILEWIDTH, TILEHEIGHT);
	_book = IMAGEMANAGER->addImage("book", "samplebook.bmp", 400, 768, true);
	_alhpa = IMAGEMANAGER->addImage("minimap", "minimap.bmp", 256, 256, true);
	IMAGEMANAGER->addImage("ok", "tileok.bmp", 64, 64, true);
	IMAGEMANAGER->addImage("no", "tileno.bmp", 64, 64, true);
	_campt = PointMake(WINSIZEX / 2, WINSIZEY / 2);
	_mouse = _ptadd = PointMake(0, 0);
	_sampleOpen = false;
	_dragrc = RectMake(0, 0, 0, 0);

	_cam = new camera;
	_cam->init(&_campt, _img);

	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			_tile[y][x].obj = NONE;
			_tile[y][x].terrain = EMPTY;
			_tile[y][x].frameX = 3;
			_tile[y][x].frameY = 2;
			_tile[y][x].rc = RectMake(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE);
		}
	}
}

void mapTool::keycontrol(void)
{
	//ī�޶� �̵�
	{
		if (KEYMANAGER->isStayKeyDown('W') && _campt.y > (WINSIZEY / 2))
		{
			_campt.y -= 12;
			_ptadd.y -= 12;
		}
		if (KEYMANAGER->isStayKeyDown('A') && _campt.x >(WINSIZEX / 2))
		{
			_campt.x -= 12;
			_ptadd.x -= 12;
		}
		if (KEYMANAGER->isStayKeyDown('S') && _campt.y < (TILEHEIGHT - WINSIZEY / 2))
		{
			_campt.y += 12;
			_ptadd.y += 12;
		}
		if (KEYMANAGER->isStayKeyDown('D') && _campt.x < (TILEWIDTH - WINSIZEX / 2))
		{
			_campt.x += 12;
			_ptadd.x += 12;
		}
	}
	//Ÿ�Ͽ� �׸��׸���
	{
		if (KEYMANAGER->isOnceKeyDown('E')) //Ÿ�� ���ú� ����
		{
			_sampleOpen = (_sampleOpen == false) ? true : false;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) //�ѹ� Ŭ��
		{
			if (_sampleOpen) { tileselect(); }
			else
			{
				_start.x = _mouse.x;
				_start.y = _mouse.y;
			}
		}
		else if (KEYMANAGER->isStayKeyDown(VK_LBUTTON)) //���ϴ� ��ŭ�� ���� ����
		{
			if (_sampleOpen) {}
			else
			{
				int width, height;
				if (_start.x < _mouse.x && _start.y < _mouse.y)
				{
					width = _mouse.x - _start.x;
					height = _mouse.y - _start.y;
					_dragrc = RectMake(_start.x, _start.y, width, height);
				}
				if (_start.x < _mouse.x && _start.y > _mouse.y)
				{
					width = _mouse.x - _start.x;
					height = _start.y - _mouse.y;
					_dragrc = RectMake(_start.x, _mouse.y, width, height);
				}
				if (_start.x > _mouse.x && _start.y < _mouse.y)
				{
					width = _start.x - _mouse.x;
					height = _mouse.y - _start.y;
					_dragrc = RectMake(_mouse.x, _start.y, width, height);
				}
				if (_start.x > _mouse.x && _start.y > _mouse.y)
				{
					width = _start.x - _mouse.x;
					height = _start.y - _mouse.y;
					_dragrc = RectMake(_mouse.x, _mouse.y, width, height);
				}

				//_end.x = _mouse.x / TILESIZE;
				//_end.y = _mouse.y / TILESIZE;
			}
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) //�������� Ÿ�� ��ο�
		{
			tiledraw();
		}
	}
}

void mapTool::tileselect(void)
{

}

void mapTool::tiledraw(void)
{
	int startX = _dragrc.left / TILESIZE;
	int startY = _dragrc.top / TILESIZE;
	int endX = _dragrc.right / TILESIZE;
	int endY = _dragrc.bottom / TILESIZE;

	//������ �Ѿ��
	if (endX - startX < 2 || endY - startY < 3)
	{
		_dragrc = RectMake(0, 0, 0, 0);
		return;
	}

	for (int y = startY; y < endY + 1; ++y)
	{
		for (int x = startX; x < endX + 1; ++x)
		{
			if (_tile[y][x].obj != NONE)
			{
				_dragrc = RectMake(0, 0, 0, 0);
				return;
			}
		}
	}

	for (int y = startY; y < endY + 1; ++y)
	{
		for (int x = startX; x < endX + 1; ++x)
		{
			//�� �𼭸�
			if (y == startY && x == startX)//������
			{
				_tile[y][x].frameX = 7;
				_tile[y][x].frameY = 0;
				_tile[y][x].terrain = STON_FLOOR;
				_tile[y][x].obj = BLANK;
				continue;
			}
			if (y == startY && x == endX)//��������
			{
				_tile[y][x].frameX = 9;
				_tile[y][x].frameY = 0;
				_tile[y][x].terrain = STON_FLOOR;
				_tile[y][x].obj = BLANK;
				continue;
			}
			if (y == endY - 2 && x == startX)//���ʾƷ�
			{
				_tile[y][x].frameX = 7;
				_tile[y][x].frameY = 2;
				_tile[y][x].obj = STON_WALL;
				continue;
			}
			if (y == endY - 2 && x == endX)//�����ʾƷ�
			{
				_tile[y][x].frameX = 9;
				_tile[y][x].frameY = 2;
				_tile[y][x].obj = STON_WALL;
				continue;
			}
			//�纯
			if (y == startY)//��
			{
				_tile[y][x].frameX = 8;
				_tile[y][x].frameY = 0;
				_tile[y][x].terrain = STON_FLOOR;
				_tile[y][x].obj = BLANK;
			}
			else if (y == endY - 2)//�Ʒ�
			{
				_tile[y][x].frameX = 8;
				_tile[y][x].frameY = 2;
				_tile[y][x].obj = STON_WALL;
			}
			else if (x == startX && y < endY - 2)//����
			{
				_tile[y][x].frameX = 7;
				_tile[y][x].frameY = 1;
				_tile[y][x].obj = STON_WALL;
			}
			else if (x == endX && y < endY - 2)//������
			{
				_tile[y][x].frameX = 9;
				_tile[y][x].frameY = 1;
				_tile[y][x].obj = STON_WALL;
			}
			else if (y == endY - 1)
			{
				_tile[y][x].frameX = 4;
				_tile[y][x].frameY = 3;
				_tile[y][x].obj = STON_WALL;
			}
			else if (y == endY)
			{
				_tile[y][x].frameX = 4;
				_tile[y][x].frameY = 4;
				_tile[y][x].obj = STON_WALL;
			}
			else
			{
				_tile[y][x].frameX = 5;
				_tile[y][x].frameY = 1;
				_tile[y][x].obj = STON_WALL;
			}
		}
	}

	_dragrc = RectMake(0, 0, 0, 0);
}

void mapTool::mousemove(void)
{
	_mouse = _ptMouse;
	POINT temp = _mouse;

	_mouse.x = temp.x + _ptadd.x;
	_mouse.y = temp.y + _ptadd.y;
}

void mapTool::draw(void)
{
	//�� ���ñ�� �� �ٴ��� �׸��ϴ�.
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; }
			if (_tile[y][x].terrain == EMPTY)
			{
				_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].frameX, _tile[y][x].frameY);
			}
			else
			{
				_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].frameX, _tile[y][x].frameY);
			}
		}
	}
	//������Ʈ�� �׸��ϴ�.
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; }
			if (_tile[y][x].obj == NONE) { continue; }
			_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].frameX, _tile[y][x].frameY);
		}
	}
	//���� �� ���콺�� ������ Ȥ�� ��ġ�� Ÿ��
	int x = _mouse.x / TILESIZE;
	int y = _mouse.y / TILESIZE;
	if (_tile[y][x].obj == NONE)
	{
		IMAGEMANAGER->findImage("ok")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
	}
	else
	{
		IMAGEMANAGER->findImage("no")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
	}
	

	//�巡���� �κ� �׷�����
	selectdraw();

	_alhpa->alphaRender(getBackDC(), _cam->getRC().right - _alhpa->getWidth(), _cam->getRC().top);

	//���ú�
	if (_sampleOpen)
	{
		_book->render(getBackDC(), _cam->getRC().right - _book->getWidth(), _cam->getRC().top);
	}
}

void mapTool::selectdraw(void)
{
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_dragrc, &_tile[y][x].rc)) { continue; }
			if (_tile[y][x].obj == NONE)
			{
				IMAGEMANAGER->findImage("ok")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
			}
			else
			{
				IMAGEMANAGER->findImage("no")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
			}
		}
	}
}