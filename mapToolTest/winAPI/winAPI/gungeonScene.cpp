#include "stdafx.h"
#include "gungeonScene.h"

gungeonScene::gungeonScene()
{
}

gungeonScene::~gungeonScene()
{
}

HRESULT gungeonScene::init(void)
{
	gameNode::init();
	setup();

	return S_OK;
}

void gungeonScene::release(void)
{
	gameNode::release();
}

void gungeonScene::update(void)
{
	gameNode::update();

	keycontrol();
	_pl->update();
	_cam->update();
}

void gungeonScene::render(void)
{
	draw();
	_cam->render();
}

void gungeonScene::setup(void)
{
	//타일 불러오기
	{
		HANDLE file;
		DWORD read;

		file = CreateFile("stageOne", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		ReadFile(file, _tile, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
		CloseHandle(file);
	}
	//변수 초기화
	{
		_campt.x = DATABASE->pstartx * TILESIZE;
		_campt.y = DATABASE->pstarty * TILESIZE;

		_ptadd = _basept = _minimappt = PointMake(0, 0);

		_mapOpen = false;

		_cimg = IMAGEMANAGER->findImage("bimg");
		_field = IMAGEMANAGER->findImage("field");
		_miniimg = IMAGEMANAGER->findImage("mimg");
		_sample = IMAGEMANAGER->findImage("tile");
		_black = IMAGEMANAGER->findImage("black");
		_minimapcase = IMAGEMANAGER->findImage("minimapcase");

		_pl = new player;
		_pl->init(_campt.x, _campt.y);

		_cam = new camera;
		_cam->init(&_campt, _pl, _cimg, true);
	}

	//바닥
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (_tile[y][x].terrain == EMPTY)
			{
				_sample->frameRender(_field->getMemDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].terrainX, _tile[y][x].terrainY);
			}
			else
			{
				_sample->frameRender(_field->getMemDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].terrainX, _tile[y][x].terrainY);
			}
		}
	}
	//벽
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (_tile[y][x].wall == VOID_WALL) { continue; }
			_sample->frameRender(_field->getMemDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].wallX, _tile[y][x].wallY);
		}
	}
}

void gungeonScene::keycontrol(void)
{
	if (KEYMANAGER->isStayKeyDown('W') && _campt.y > (WINSIZEY / 2))
	{
		_pl->getCharacterData().y -= 12;
		_ptadd.y -= 12;
	}
	if (KEYMANAGER->isStayKeyDown('A') && _campt.x >(WINSIZEX / 2))
	{
		_pl->getCharacterData().x -= 12;
		_ptadd.x -= 12;
	}
	if (KEYMANAGER->isStayKeyDown('S') && _campt.y < (TILEHEIGHT - WINSIZEY / 2))
	{
		_pl->getCharacterData().y += 12;
		_ptadd.y += 12;
	}
	if (KEYMANAGER->isStayKeyDown('D') && _campt.x < (TILEWIDTH - WINSIZEX / 2))
	{
		_pl->getCharacterData().x += 12;
		_ptadd.x += 12;
	}
	//미니맵
	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		_mapOpen = (_mapOpen == false) ? true : false;
	}
}

void gungeonScene::draw(void)
{
	int sy = _cam->getRC().top / TILESIZE;
	int sx = _cam->getRC().left / TILESIZE;
	int ey = _cam->getRC().bottom / TILESIZE + 1;
	int ex = _cam->getRC().right / TILESIZE + 1;
	if (ey > TILEY) { ey = TILEY; }
	if (ex > TILEX) { ex = TILEX; }

	_field->render(getBackDC(), _cam->getRC().left, _cam->getRC().top, _cam->getRC().left, _cam->getRC().top, WINSIZEX, WINSIZEY);

	_pl->render(getBackDC());
	//이 사이에 플레이어, 적군, 오브젝트 짬뽕시켜서 Z-order해준다음에 뿌려주자

	//대망의 오브젝트
	for (int y = sy; y < ey; ++y)
	{
		for (int x = sx; x < ex; ++x)
		{
			if (_tile[y][x].obj == WALL_TOP) _sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].objframeX, _tile[y][x].objframeY);
		}
	}

	//미니맵
	if (_mapOpen) { minimap(); }
}

void gungeonScene::minimap(void)
{
	HBRUSH bluebrush = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH withebrush = CreateSolidBrush(RGB(255, 255, 255));
	_black->render(_miniimg->getMemDC());
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (!_tile[y][x].show) { continue; }
			if (_tile[y][x].terrain != EMPTY)
			{
				FillRect(_miniimg->getMemDC(), &_tile[y][x].minirc, bluebrush);
			}
			if (_tile[y][x].wall != VOID_WALL)
			{
				FillRect(_miniimg->getMemDC(), &_tile[y][x].minirc, withebrush);
			}
		}
	}
	RECT mmaprc = RectMake(_cam->getRC().left + 100, _cam->getRC().top + 80, 800, 600);

	POINT mtemp;
	mtemp.x = (_cam->getRC().left + _pl->getCharacterData().x / (TILESIZE / _TILEMINISIZE)) - _minimappt.x + 100;
	mtemp.y = (_cam->getRC().top + _pl->getCharacterData().y / (TILESIZE / _TILEMINISIZE)) - _minimappt.y + 80;
	RECT ftemp = RectMakeCenter(mtemp.x, mtemp.y, _TILEMINISIZE * 16, _TILEMINISIZE * 12);

	_minimapcase->alphaRender(getBackDC(), mmaprc.left - 17, mmaprc.top - 60);
	_miniimg->alphaRender(getBackDC(), mmaprc.left, mmaprc.top, _minimappt.x, _minimappt.y, 800, 600);
	LineMake(getBackDC(), ftemp.left, ftemp.top, ftemp.right, ftemp.top);
	LineMake(getBackDC(), ftemp.right, ftemp.top, ftemp.right, ftemp.bottom);
	LineMake(getBackDC(), ftemp.right, ftemp.bottom, ftemp.left, ftemp.bottom);
	LineMake(getBackDC(), ftemp.left, ftemp.bottom, ftemp.left, ftemp.top);

	DeleteObject(bluebrush);
	DeleteObject(withebrush);
}