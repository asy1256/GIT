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
	//샘플북 버튼 위치 갱신
	for (int y = 40, i = 0; i < 5; ++i, y += 50)
	{
		_tagbutton[i].rc = RectMake(_cam->getRC().right - IMAGEMANAGER->findImage("booktag")->getFrameWidth() - _book->getWidth() + 10, _cam->getRC().top + y,
			IMAGEMANAGER->findImage("booktag")->getFrameWidth(), IMAGEMANAGER->findImage("booktag")->getFrameHeight());
	}
	//빅샘플 케이스 위치 갱신
	for (int sy = 94, y = 0; y < 5; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_bigsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}
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
	IMAGEMANAGER->addImage("cover", "bookcover.bmp", 400, 768, true);
	IMAGEMANAGER->addFrameImage("bigsample", "bigsample.bmp", 600, 320, 5, 2, true);
	IMAGEMANAGER->addFrameImage("bigsamplecase", "bigsamplecase.bmp", 280, 220, 2, 1, true);
	IMAGEMANAGER->addFrameImage("booktag", "booktag.bmp", 100, 300, 2, 10, true);
	_campt = PointMake(WINSIZEX / 2, WINSIZEY / 2);
	_mouse = _ptadd = PointMake(0, 0);
	_sampleOpen = false;
	_dragrc = RectMake(0, 0, 0, 0);
	_page = 0;
	_nowdraw = D_NONE;
	_nowselecct = S_NONE;

	_cam = new camera;
	_cam->init(&_campt, _img);

	//샘플북 태그 초기화
	for (int y = 40, i = 0; i < 5; ++i, y += 50)
	{
		_tagbutton[i].onmouse = _tagbutton[i].select = false;
		_tagbutton[i].rc = RectMake(_cam->getRC().right - IMAGEMANAGER->findImage("booktag")->getFrameWidth() - _book->getWidth() + 10, _cam->getRC().top + y,
			IMAGEMANAGER->findImage("booktag")->getFrameWidth(), IMAGEMANAGER->findImage("booktag")->getFrameHeight());
	}
	_tagbutton[0].select = true;

	//빅샘플케이스 초기화
	for (int sy = 94, y = 0; y < 5; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_bigsample[y][x].kind = S_NONE;
			_bigsample[y][x].select = false;
			_bigsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}


	//타일 초기화
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			_tile[y][x].obj = NONE;
			_tile[y][x].pass = true;
			_tile[y][x].terrain = EMPTY;
			_tile[y][x].terrainX = 3;
			_tile[y][x].terrainY = RND->getInt(2);
			_tile[y][x].objframeX = 5;
			_tile[y][x].objframeY = 1;
			_tile[y][x].roomnum = _tile[y][x].sponSequence = 0;
			_tile[y][x].rc = RectMake(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE);
		}
	}
}

void mapTool::keycontrol(void)
{
	//카메라 이동
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
	//타일에 그림그리기
	{
		//태그 움직이기
		for (int i = 0; i < 5; ++i)
		{
			if (PtInRect(&_tagbutton[i].rc, _mouse))
			{
				_tagbutton[i].onmouse = true;
			}
			else { _tagbutton[i].onmouse = false; }
		}

		if (KEYMANAGER->isOnceKeyDown('E')) //타일 샘플북 오픈
		{
			_sampleOpen = (_sampleOpen == false) ? true : false;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) //한번 클릭
		{
			if (_sampleOpen) { pageselect(); tileselect(); }
			else
			{
				_start.x = _mouse.x;
				_start.y = _mouse.y;
			}
		}
		else if (KEYMANAGER->isStayKeyDown(VK_LBUTTON)) //원하는 만큼의 구역 지정
		{
			if (_sampleOpen) {}
			else
			{
				//tiledraw();
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
			}
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) //지정범위 타일 드로우
		{
			//if (_nowtag != SAMIAUTO) return;
			if (_sampleOpen) {}
			else { tiledraw(); }
		}
	}
}

void mapTool::tileselect(void)
{
	if (_page == 1)
	{
		int sy = 0, sx = 0;
		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				if (PtInRect(&_bigsample[y][x].rc, _mouse))
				{
					_bigsample[y][x].select = true;
					sy = y; sx = x;
				}
			}
		}

		for (int y = 0; y < 5; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				if (sy == y && sx == x) { continue; }
				_bigsample[y][x].select = false;
			}
		}
	}
}

void mapTool::pageselect(void)
{
	for (int i = 0; i < 5; ++i)
	{
		if (PtInRect(&_tagbutton[i].rc, _mouse))
		{
			_page = i;
			_tagbutton[i].select = true;
		}
	}
	for (int i = 0; i < 5; ++i)
	{
		if (i != _page) { _tagbutton[i].select = false; }
	}
}

void mapTool::tiledraw(void)
{
	if (_nowdraw == STICK) //수동 그리기
	{

	}
	else if (_nowdraw == SAMIAUTO) //반자동 그리기
	{
		
	}
	int startX = _dragrc.left / TILESIZE;
	int startY = _dragrc.top / TILESIZE;
	int endX = _dragrc.right / TILESIZE;
	int endY = _dragrc.bottom / TILESIZE;

	////작으면 넘어가자
	//if (endX - startX < 2 || endY - startY < 3)
	//{
	//	_dragrc = RectMake(0, 0, 0, 0);
	//	return;
	//}

	//그리는 영역에 오브젝트가 깔려있다면 ㅈㅈ
	for (int y = startY; y < endY + 1; ++y)
	{
		for (int x = startX; x < endX + 1; ++x)
		{
			if (!_tile[y][x].pass || _tile[y][x].terrain != EMPTY)
			{
				_dragrc = RectMake(0, 0, 0, 0);
				return;
			}
		}
	}
	//돌벽 or 나무벽 기준 정보 저장
	for (int y = startY; y < endY + 1; ++y)
	{
		for (int x = startX; x < endX + 1; ++x)
		{
			//각 모서리
			if (y == startY && x == startX)//왼쪽위
			{
				_tile[y][x].terrainX = 7;
				_tile[y][x].terrainY = 0;
				_tile[y][x].terrain = STON_FLOOR;
				continue;
			}
			if (y == startY && x == endX)//오른쪽위
			{
				_tile[y][x].terrainX = 9;
				_tile[y][x].terrainY = 0;
				_tile[y][x].terrain = STON_WALL;
				continue;
			}
			if (y == endY - 2 && x == startX)//왼쪽아래
			{
				_tile[y][x].terrainX = 7;
				_tile[y][x].terrainY = 2;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
				continue;
			}
			if (y == endY - 2 && x == endX)//오른쪽아래
			{
				_tile[y][x].terrainX = 9;
				_tile[y][x].terrainY = 2;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
				continue;
			}
			//사변
			if (y == startY)//위
			{
				_tile[y][x].terrainX = 8;
				_tile[y][x].terrainY = 0;
				_tile[y][x].terrain = STON_WALL;
			}
			else if (y == endY - 2)//아래
			{
				_tile[y][x].terrainX = 8;
				_tile[y][x].terrainY = 2;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
			}
			else if (x == startX && y < endY - 2)//왼쪽
			{
				_tile[y][x].terrainX = 7;
				_tile[y][x].terrainY = 1;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
			}
			else if (x == endX && y < endY - 2)//오른쪽
			{
				_tile[y][x].terrainX = 9;
				_tile[y][x].terrainY = 1;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
			}
			else if (y == endY - 1)
			{
				_tile[y][x].terrainX = 4;
				_tile[y][x].terrainY = 3;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
			}
			else if (y == endY)
			{
				_tile[y][x].terrainX = 4;
				_tile[y][x].terrainY = 4;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
			}
			else
			{
				_tile[y][x].terrainX = 5;
				_tile[y][x].terrainY = 1;
				_tile[y][x].terrain = STON_WALL;
				_tile[y][x].pass = false;
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
	//거 뭐시기냐 그 바닥을 그립니다.
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; }
			if (_tile[y][x].terrain == EMPTY)
			{
				_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].terrainX, _tile[y][x].terrainY);
			}
			else
			{
				_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].terrainX, _tile[y][x].terrainY);
			}
		}
	}

	//오브젝트를 그립니다.
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; }
			if (_tile[y][x].obj == NONE) { continue; }
			_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].objframeX, _tile[y][x].objframeY);
		}
	}
	//현재 내 마우스가 지정한 혹은 위치한 타일
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

	//드래그한 부분 그려주자
	selectdraw();
	//미니맵
	_alhpa->alphaRender(getBackDC(), _cam->getRC().right - _alhpa->getWidth(), _cam->getRC().top);

	//샘플북
	if (_sampleOpen)
	{
		_book->render(getBackDC(), _cam->getRC().right - _book->getWidth(), _cam->getRC().top);

		if (_page == 1)
		{
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 2; ++x)
				{
					if (_bigsample[y][x].select) { IMAGEMANAGER->findImage("bigsamplecase")->frameRender(getBackDC(), _bigsample[y][x].rc.left, _bigsample[y][x].rc.top, 1, 0); }
					else { IMAGEMANAGER->findImage("bigsamplecase")->frameRender(getBackDC(), _bigsample[y][x].rc.left, _bigsample[y][x].rc.top, 0, 0); }
					IMAGEMANAGER->findImage("bigsample")->frameRender(getBackDC(), _bigsample[y][x].rc.left + 10, _bigsample[y][x].rc.top + 30, y, x);
				}
			}
		}


		IMAGEMANAGER->findImage("cover")->render(getBackDC(), _cam->getRC().right - _book->getWidth(), _cam->getRC().top);
		//태그
		for (int y = 1, i = 0; i < 5; ++i, y += 2)
		{
			if (_tagbutton[i].select)
			{
				if (_tagbutton[i].onmouse) { IMAGEMANAGER->findImage("booktag")->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 1, y); }
				else { IMAGEMANAGER->findImage("booktag")->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 0, y); }
			}
			else
			{
				if (_tagbutton[i].onmouse) { IMAGEMANAGER->findImage("booktag")->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 1, i * 2); }
				else { IMAGEMANAGER->findImage("booktag")->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 0, i * 2); }
			}
		}
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