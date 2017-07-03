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
			_bigsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy - _currentscrool,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}
	//스크롤바 갱신
	_scroolpos = (_currentscrool / _scroolmax) * 512;
}

void mapTool::render(void)
{
	draw();
	_cam->render();
}

void mapTool::setup(void)
{
	//필요한 이미지들 추가
	{
		_sample = IMAGEMANAGER->addFrameImage("tile", "sampletile.bmp", 640, 640, 10, 10, true);
		_img = IMAGEMANAGER->addImage("bimg", "backImage.bmp", TILEWIDTH, TILEHEIGHT);
		_book = IMAGEMANAGER->addImage("book", "samplebook.bmp", 400, 768, true);
		_alhpa = IMAGEMANAGER->addImage("minimap", "minimap.bmp", 256, 256, true);
		IMAGEMANAGER->addImage("ok", "tileok.bmp", 64, 64, true);
		IMAGEMANAGER->addImage("no", "tileno.bmp", 64, 64, true);
		IMAGEMANAGER->addImage("cover", "bookcover.bmp", 400, 768, true);
		IMAGEMANAGER->addFrameImage("bigsample", "bigsample.bmp", 240, 800, 2, 5, true);
		IMAGEMANAGER->addFrameImage("bigsamplecase", "bigsamplecase.bmp", 280, 220, 2, 1, true);
		IMAGEMANAGER->addFrameImage("booktag", "booktag.bmp", 100, 300, 2, 10, true);
		IMAGEMANAGER->addImage("scrollbardown", "scrollbarplace.bmp", 14, 580, true);
		IMAGEMANAGER->addImage("scrollbar", "scrollbar.bmp", 14, 68, true);
		IMAGEMANAGER->addFrameImage("barrels", "barrels.bmp", 128, 96, 2, 1, true);
	}
	//다양한 변수 초기화
	{
		_campt = PointMake(WINSIZEX / 2, WINSIZEY / 2);
		_mouse = _ptadd = PointMake(0, 0);
		_sampleOpen = false;
		_dragrc = RectMake(0, 0, 0, 0);
		_page = 0;
		_scroolmax = 600;
		_currentscrool = 0;
		_scroolpos = (_currentscrool / _scroolmax) * 512;
		_nowdraw = D_NONE;
		_nowselecct = S_NONE;
	}

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
			if (y == 0) { _bigsample[y][x].type = SAMIAUTO; _bigsample[y][x].kind = (x == 0) ? S_WALL : W_WALL; }
			if (y == 1) { _bigsample[y][x].type = SAMIAUTO; _bigsample[y][x].kind = (x == 0) ? CARPT_A : CARPT_B; }
			if (y == 2) { _bigsample[y][x].type = AUTO; _bigsample[y][x].kind = (x == 0) ? TELEPORTER : DOOR; }
			if (y == 3) { _bigsample[y][x].type = STICK; _bigsample[y][x].kind = (x == 0) ? L_TABLE : W_TABLE; }
			if (y == 4) { _bigsample[y][x].type = STICK; _bigsample[y][x].kind = (x == 0) ? W_BARREL : B_BARREL; }
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
	//스크롤바 이동
	if (_sampleOpen)
	{
		if (_wheelUp)
		{
			if (_page == 1)
			{
				_currentscrool -= 20;
				if (_currentscrool <= 0) { _currentscrool = 0; }
			}
		}
		if (_wheelDown)
		{
			if (_page == 1)
			{
				_currentscrool += 20;
				if (_currentscrool >= _scroolmax) { _currentscrool = _scroolmax; }
			}
		}
		_wheelUp = false;
		_wheelDown = false;
	}
	else
	{

	}
	//타일에 그림그리기
	{
		//샘플북 포커싱 해주기
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
				if (_nowdraw == SAMIAUTO) //반자동 그리기 렉트시작점
				{
					_start.x = _mouse.x;
					_start.y = _mouse.y;
				}
				if (_nowdraw == AUTO) { tiledraw(); }
			}
		}
		else if (KEYMANAGER->isStayKeyDown(VK_LBUTTON)) //원하는 만큼의 구역 지정
		{
			if (_sampleOpen) {}
			else
			{
				if (_nowdraw == STICK) { tiledraw(); }
				if (_nowdraw == SAMIAUTO) //반자동 그리기 렉트시작점
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
				}
			}
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) //지정범위 타일 드로우
		{
			//if (_nowtag != SAMIAUTO) return;
			if (_sampleOpen) {}
			else { if (_nowdraw == SAMIAUTO) { tiledraw(); } }
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
					_nowselecct = _bigsample[y][x].kind;
					_nowdraw = _bigsample[y][x].type;
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
		int nowx = _mouse.x / TILESIZE;
		int nowy = _mouse.y / TILESIZE;
		//가로 테이블
		if (_nowselecct == W_TABLE)
		{
			if (_tile[nowy][nowx].obj == NONE && _tile[nowy][nowx + 1].obj == NONE)
			{
				_tile[nowy][nowx].objframeX = 3;
				_tile[nowy][nowx].objframeY = 5;
				_tile[nowy][nowx + 1].objframeX = 4;
				_tile[nowy][nowx + 1].objframeY = 5;
				_tile[nowy][nowx].obj = TABLE_WIDTH;
				_tile[nowy][nowx + 1].obj = BLANK;
			}
		}
		//세로 테이블
		if (_nowselecct == L_TABLE)
		{
			if (_tile[nowy][nowx].obj == NONE && _tile[nowy - 1][nowx].obj == NONE)
			{
				_tile[nowy][nowx].objframeX = 6;
				_tile[nowy][nowx].objframeY = 6;
				_tile[nowy - 1][nowx].objframeX = 6;
				_tile[nowy - 1][nowx].objframeY = 5;
				_tile[nowy][nowx].obj = TABLE_LENGTH;
				_tile[nowy - 1][nowx].obj = BLANK;
			}
		}
		//나무 통
		if (_nowselecct == W_BARREL)
		{
			if (_tile[nowy][nowx].obj == NONE && (_tile[nowy][nowx].terrain != BOOK_WALL && _tile[nowy][nowx].terrain != STON_WALL)) { _tile[nowy][nowx].obj = WOOD_BARREL; }
		}
		//폭탄 통
		if (_nowselecct == B_BARREL)
		{
			if (_tile[nowy][nowx].obj == NONE) { _tile[nowy][nowx].obj = BOOM_BARREL; }
		}
	}
	else if (_nowdraw == SAMIAUTO) //반자동 그리기
	{
		int startX = _dragrc.left / TILESIZE;
		int startY = _dragrc.top / TILESIZE;
		int endX = _dragrc.right / TILESIZE;
		int endY = _dragrc.bottom / TILESIZE;
		//돌벽 혹은 나무벽그릴때
		if (_nowselecct == S_WALL || _nowselecct == W_WALL)
		{
			//작으면 넘어가자
			if (endX - startX < 2 || endY - startY < 3)
			{
				_dragrc = RectMake(0, 0, 0, 0);
				return;
			}
			//그리는 영역에 오브젝트가 깔려있을때
			for (int y = startY; y < endY + 1; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					if (!_tile[y][x].pass || _tile[y][x].obj != NONE)
					{
						_dragrc = RectMake(0, 0, 0, 0);
						return;
					}
				}
			}
			//그려봅시다.
			for (int y = startY; y < endY + 1; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					//각 모서리
					if (y == startY && x == startX)//왼쪽위
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].terrainY = 0;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].obj = BLANK;
						continue;
					}
					if (y == startY && x == endX)//오른쪽위
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].terrainY = 0;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].obj = BLANK;
						continue;
					}
					if (y == endY - 2 && x == startX)//왼쪽아래
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].terrainY = 2;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					if (y == endY - 2 && x == endX)//오른쪽아래
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].terrainY = 2;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					//사변
					if (y == startY)//위
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].terrainY = 0;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].obj = BLANK;
					}
					else if (y == endY - 2)//아래
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].terrainY = 2;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == startX && y < endY - 2)//왼쪽
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].terrainY = 1;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == endX && y < endY - 2)//오른쪽
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].terrainY = 1;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY - 1) //윗 벽돌
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].terrainY = 3;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY) //아래 벽돌
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].terrainY = 4;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else //벽 안쪽
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 5 : 5;
						_tile[y][x].terrainY = 1;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
				}
			}
		}
		//카페트 그릴때
		if (_nowselecct == CARPT_A || _nowselecct == CARPT_B)
		{
			//작으면 넘어가자
			if (endX - startX < 2 || endY - startY < 2)
			{
				_dragrc = RectMake(0, 0, 0, 0);
				return;
			}
			//그려봅시다.
			for (int y = startY; y < endY + 1; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					if (y == startY && x == startX)//왼쪽위
					{
						_tile[y][x].terrainX = 0;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 0 : 3;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					if (y == startY && x == endX)//오른쪽위
					{
						_tile[y][x].terrainX = 2;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 0 : 3;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					if (y == endY && x == startX)//왼쪽아래
					{
						_tile[y][x].terrainX = 0;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 2 : 5;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					if (y == endY && x == endX)//오른쪽아래
					{
						_tile[y][x].terrainX = 2;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 2 : 5;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					//사변
					if (y == startY)//위
					{
						_tile[y][x].terrainX = 1;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 0 : 3;
						_tile[y][x].terrain = CARPET;
					}
					else if (y == endY)//아래
					{
						_tile[y][x].terrainX = 1;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 2 : 5;
						_tile[y][x].terrain = CARPET;
					}
					else if (x == startX && y < endY)//왼쪽
					{
						_tile[y][x].terrainX = 0;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 1 : 4;
						_tile[y][x].terrain = CARPET;
					}
					else if (x == endX && y < endY)//오른쪽
					{
						_tile[y][x].terrainX = 2;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 1 : 4;
						_tile[y][x].terrain = CARPET;
					}
					else
					{
						_tile[y][x].terrainX = 1;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 1 : 4;
						_tile[y][x].terrain = CARPET;
					}
				}
			}
		}
	}
	else if (_nowdraw == AUTO)
	{

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
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; } //클리핑 영역 안에 없으면 넘어가
			if (_tile[y][x].obj == NONE) { continue; } //오브젝트가 안깔려 있어도 넘어가
			if ((_tile[y][x].terrain == STON_WALL || _tile[y][x].terrain == BOOK_WALL) && _tile[y][x].pass) { continue; } //벽이 세워져 있으면 넘어가
			if (_tile[y][x].obj == WOOD_BARREL) { IMAGEMANAGER->findImage("barrels")->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 0, 0); continue; } //나무통이면 그리고 넘어가
			else if (_tile[y][x].obj == BOOM_BARREL) { IMAGEMANAGER->findImage("barrels")->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 1, 0); continue; } //폭탄통이면 그리고 넘어가
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
					IMAGEMANAGER->findImage("bigsample")->frameRender(getBackDC(), _bigsample[y][x].rc.left + 10, _bigsample[y][x].rc.top + 30, x, y);
				}
			}
		}

		//샘플북 커버
		IMAGEMANAGER->findImage("cover")->render(getBackDC(), _cam->getRC().right - _book->getWidth(), _cam->getRC().top);
		//스크롤바 밑 부분
		IMAGEMANAGER->findImage("scrollbardown")->render(getBackDC(), _cam->getRC().right - 34, _cam->getRC().top + 94);
		IMAGEMANAGER->findImage("scrollbar")->render(getBackDC(), _cam->getRC().right - 34, (_cam->getRC().top + 94) + _scroolpos);
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
			if (!_tile[y][x].pass || _tile[y][x].obj != NONE)
			{
				IMAGEMANAGER->findImage("no")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
				continue;
			}
			if (_tile[y][x].obj == NONE)
			{
				IMAGEMANAGER->findImage("ok")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
			}
		}
	}
}