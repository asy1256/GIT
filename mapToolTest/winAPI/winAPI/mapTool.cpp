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
	//���ú� ��ư ��ġ ����
	for (int y = 40, i = 0; i < 5; ++i, y += 50)
	{
		_tagbutton[i].rc = RectMake(_cam->getRC().right - IMAGEMANAGER->findImage("booktag")->getFrameWidth() - _book->getWidth() + 10, _cam->getRC().top + y,
			IMAGEMANAGER->findImage("booktag")->getFrameWidth(), IMAGEMANAGER->findImage("booktag")->getFrameHeight());
	}
	//����� ���̽� ��ġ ����
	for (int sy = 94, y = 0; y < 5; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_bigsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy - _currentscrool,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}
	//��ũ�ѹ� ����
	_scroolpos = (_currentscrool / _scroolmax) * 512;
}

void mapTool::render(void)
{
	draw();
	_cam->render();
}

void mapTool::setup(void)
{
	//�ʿ��� �̹����� �߰�
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
	//�پ��� ���� �ʱ�ȭ
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

	//���ú� �±� �ʱ�ȭ
	for (int y = 40, i = 0; i < 5; ++i, y += 50)
	{
		_tagbutton[i].onmouse = _tagbutton[i].select = false;
		_tagbutton[i].rc = RectMake(_cam->getRC().right - IMAGEMANAGER->findImage("booktag")->getFrameWidth() - _book->getWidth() + 10, _cam->getRC().top + y,
			IMAGEMANAGER->findImage("booktag")->getFrameWidth(), IMAGEMANAGER->findImage("booktag")->getFrameHeight());
	}
	_tagbutton[0].select = true;

	//��������̽� �ʱ�ȭ
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

	//Ÿ�� �ʱ�ȭ
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
	//��ũ�ѹ� �̵�
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
	//Ÿ�Ͽ� �׸��׸���
	{
		//���ú� ��Ŀ�� ���ֱ�
		for (int i = 0; i < 5; ++i)
		{
			if (PtInRect(&_tagbutton[i].rc, _mouse))
			{
				_tagbutton[i].onmouse = true;
			}
			else { _tagbutton[i].onmouse = false; }
		}

		if (KEYMANAGER->isOnceKeyDown('E')) //Ÿ�� ���ú� ����
		{
			_sampleOpen = (_sampleOpen == false) ? true : false;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) //�ѹ� Ŭ��
		{
			if (_sampleOpen) { pageselect(); tileselect(); }
			else
			{
				if (_nowdraw == SAMIAUTO) //���ڵ� �׸��� ��Ʈ������
				{
					_start.x = _mouse.x;
					_start.y = _mouse.y;
				}
				if (_nowdraw == AUTO) { tiledraw(); }
			}
		}
		else if (KEYMANAGER->isStayKeyDown(VK_LBUTTON)) //���ϴ� ��ŭ�� ���� ����
		{
			if (_sampleOpen) {}
			else
			{
				if (_nowdraw == STICK) { tiledraw(); }
				if (_nowdraw == SAMIAUTO) //���ڵ� �׸��� ��Ʈ������
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
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON)) //�������� Ÿ�� ��ο�
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
	if (_nowdraw == STICK) //���� �׸���
	{
		int nowx = _mouse.x / TILESIZE;
		int nowy = _mouse.y / TILESIZE;
		//���� ���̺�
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
		//���� ���̺�
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
		//���� ��
		if (_nowselecct == W_BARREL)
		{
			if (_tile[nowy][nowx].obj == NONE && (_tile[nowy][nowx].terrain != BOOK_WALL && _tile[nowy][nowx].terrain != STON_WALL)) { _tile[nowy][nowx].obj = WOOD_BARREL; }
		}
		//��ź ��
		if (_nowselecct == B_BARREL)
		{
			if (_tile[nowy][nowx].obj == NONE) { _tile[nowy][nowx].obj = BOOM_BARREL; }
		}
	}
	else if (_nowdraw == SAMIAUTO) //���ڵ� �׸���
	{
		int startX = _dragrc.left / TILESIZE;
		int startY = _dragrc.top / TILESIZE;
		int endX = _dragrc.right / TILESIZE;
		int endY = _dragrc.bottom / TILESIZE;
		//���� Ȥ�� �������׸���
		if (_nowselecct == S_WALL || _nowselecct == W_WALL)
		{
			//������ �Ѿ��
			if (endX - startX < 2 || endY - startY < 3)
			{
				_dragrc = RectMake(0, 0, 0, 0);
				return;
			}
			//�׸��� ������ ������Ʈ�� ���������
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
			//�׷����ô�.
			for (int y = startY; y < endY + 1; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					//�� �𼭸�
					if (y == startY && x == startX)//������
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].terrainY = 0;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].obj = BLANK;
						continue;
					}
					if (y == startY && x == endX)//��������
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].terrainY = 0;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].obj = BLANK;
						continue;
					}
					if (y == endY - 2 && x == startX)//���ʾƷ�
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].terrainY = 2;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					if (y == endY - 2 && x == endX)//�����ʾƷ�
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].terrainY = 2;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					//�纯
					if (y == startY)//��
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].terrainY = 0;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].obj = BLANK;
					}
					else if (y == endY - 2)//�Ʒ�
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].terrainY = 2;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == startX && y < endY - 2)//����
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].terrainY = 1;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == endX && y < endY - 2)//������
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].terrainY = 1;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY - 1) //�� ����
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].terrainY = 3;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY) //�Ʒ� ����
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].terrainY = 4;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else //�� ����
					{
						_tile[y][x].terrainX = (_nowselecct == S_WALL) ? 5 : 5;
						_tile[y][x].terrainY = 1;
						_tile[y][x].terrain = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
				}
			}
		}
		//ī��Ʈ �׸���
		if (_nowselecct == CARPT_A || _nowselecct == CARPT_B)
		{
			//������ �Ѿ��
			if (endX - startX < 2 || endY - startY < 2)
			{
				_dragrc = RectMake(0, 0, 0, 0);
				return;
			}
			//�׷����ô�.
			for (int y = startY; y < endY + 1; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					if (y == startY && x == startX)//������
					{
						_tile[y][x].terrainX = 0;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 0 : 3;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					if (y == startY && x == endX)//��������
					{
						_tile[y][x].terrainX = 2;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 0 : 3;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					if (y == endY && x == startX)//���ʾƷ�
					{
						_tile[y][x].terrainX = 0;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 2 : 5;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					if (y == endY && x == endX)//�����ʾƷ�
					{
						_tile[y][x].terrainX = 2;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 2 : 5;
						_tile[y][x].terrain = CARPET;
						continue;
					}
					//�纯
					if (y == startY)//��
					{
						_tile[y][x].terrainX = 1;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 0 : 3;
						_tile[y][x].terrain = CARPET;
					}
					else if (y == endY)//�Ʒ�
					{
						_tile[y][x].terrainX = 1;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 2 : 5;
						_tile[y][x].terrain = CARPET;
					}
					else if (x == startX && y < endY)//����
					{
						_tile[y][x].terrainX = 0;
						_tile[y][x].terrainY = (_nowselecct == CARPT_A) ? 1 : 4;
						_tile[y][x].terrain = CARPET;
					}
					else if (x == endX && y < endY)//������
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
	//�� ���ñ�� �� �ٴ��� �׸��ϴ�.
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

	//������Ʈ�� �׸��ϴ�.
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; } //Ŭ���� ���� �ȿ� ������ �Ѿ
			if (_tile[y][x].obj == NONE) { continue; } //������Ʈ�� �ȱ�� �־ �Ѿ
			if ((_tile[y][x].terrain == STON_WALL || _tile[y][x].terrain == BOOK_WALL) && _tile[y][x].pass) { continue; } //���� ������ ������ �Ѿ
			if (_tile[y][x].obj == WOOD_BARREL) { IMAGEMANAGER->findImage("barrels")->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 0, 0); continue; } //�������̸� �׸��� �Ѿ
			else if (_tile[y][x].obj == BOOM_BARREL) { IMAGEMANAGER->findImage("barrels")->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 1, 0); continue; } //��ź���̸� �׸��� �Ѿ
			_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].objframeX, _tile[y][x].objframeY);
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
	//�̴ϸ�
	_alhpa->alphaRender(getBackDC(), _cam->getRC().right - _alhpa->getWidth(), _cam->getRC().top);

	//���ú�
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

		//���ú� Ŀ��
		IMAGEMANAGER->findImage("cover")->render(getBackDC(), _cam->getRC().right - _book->getWidth(), _cam->getRC().top);
		//��ũ�ѹ� �� �κ�
		IMAGEMANAGER->findImage("scrollbardown")->render(getBackDC(), _cam->getRC().right - 34, _cam->getRC().top + 94);
		IMAGEMANAGER->findImage("scrollbar")->render(getBackDC(), _cam->getRC().right - 34, (_cam->getRC().top + 94) + _scroolpos);
		//�±�
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