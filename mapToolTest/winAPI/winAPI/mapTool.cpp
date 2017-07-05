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
	bookup();
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
		_sample = IMAGEMANAGER->addFrameImage("tile", "sampletile.bmp", 1280, 2560, 20, 40, true);
		_img = IMAGEMANAGER->addImage("bimg", "backImage.bmp", TILEWIDTH, TILEHEIGHT);
		_book = IMAGEMANAGER->addImage("book", "samplebook.bmp", 400, 768, true);
		_alhpa = IMAGEMANAGER->addImage("minimap", "minimap.bmp", 256, 256, true);
		IMAGEMANAGER->addImage("ok", "tileok.bmp", 64, 64, true);
		IMAGEMANAGER->addImage("no", "tileno.bmp", 64, 64, true);
		IMAGEMANAGER->addImage("cover", "bookcover.bmp", 400, 768, true);
		IMAGEMANAGER->addFrameImage("bigsamplecase", "bigsamplecase.bmp", 280, 220, 2, 1, true);
		IMAGEMANAGER->addFrameImage("mapoption", "mapoption.bmp", 600, 540, 2, 5, true);
		IMAGEMANAGER->addFrameImage("bigsample", "bigsample.bmp", 240, 800, 2, 5, true);
		IMAGEMANAGER->addFrameImage("enemysample", "enemysample.bmp", 240, 320, 2, 2, true);
		IMAGEMANAGER->addFrameImage("roomsample", "roomsample.bmp", 240, 320, 2, 2, true);
		IMAGEMANAGER->addFrameImage("booktag", "booktag.bmp", 100, 360, 2, 12, true);
		IMAGEMANAGER->addImage("scrollbardown", "scrollbarplace.bmp", 14, 580, true);
		IMAGEMANAGER->addImage("scrollbar", "scrollbar.bmp", 14, 68, true);
		IMAGEMANAGER->addFrameImage("barrels", "barrels.bmp", 128, 96, 2, 1, true);
		IMAGEMANAGER->addFrameImage("enemys", "enemys.bmp", 192, 256, 2, 2, true);
	}
	//�پ��� ���� �ʱ�ȭ
	{
		_campt = PointMake(WINSIZEX / 2, WINSIZEY / 2);
		_mouse = _ptadd = PointMake(0, 0);
		_sampleOpen = false;
		_dragrc = RectMake(0, 0, 0, 0);
		_page = 0;
		_makeroom = 1;
		_scroolmax = 600;
		_currentscrool = 0;
		_scroolpos = (_currentscrool / _scroolmax) * 512;
		_nowdraw = D_NONE;
		_nowselecct = S_NONE;
	}

	SetTextColor(getBackDC(), RGB(255, 255, 255));
	SetBkMode(getBackDC(), TRANSPARENT);

	_cam = new camera;
	_cam->init(&_campt, _img);

	//���ú� �±� �ʱ�ȭ
	for (int y = 40, i = 0; i < 6; ++i, y += 50)
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

	//���ʹ̻������̽� �ʱ�ȭ
	for (int sy = 94, y = 0; y < 2; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			if (y == 0) { _enemysample[y][x].type = STICK; _enemysample[y][x].kind = (x == 0) ? K_BULLET : B_BULLET; }
			if (y == 1) { _enemysample[y][x].type = STICK; _enemysample[y][x].kind = (x == 0) ? R_SHOTGUN : GUNUT; }
			_enemysample[y][x].select = false;
			_enemysample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}

	//��������̽� �ʱ�ȭ
	for (int sy = 94, y = 0; y < 2; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			if (y == 0) { _roomsample[y][x].type = AUTO; _roomsample[y][x].kind = (x == 0) ? START_ROOM : SHOP_ROOM; }
			if (y == 1) { _roomsample[y][x].type = SAMIAUTO; _roomsample[y][x].kind = (x == 0) ? STON_ROOM : WOOD_ROOM; }
			_roomsample[y][x].select = false;
			_roomsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}

	//���� �ɼ� �ʱ�ȭ
	for (int sy = 94, y = 0; y < 5; ++y, sy += 118)
	{
		if (y == 0) { _option[y].type = SAMIAUTO; _option[y].kind = T_ERASE; }
		if (y == 1) { _option[y].type = SAMIAUTO; _option[y].kind = OB_ERASE; }
		_option[y].select = false;
		_option[y].rc = RectMake(_cam->getRC().right - 350, _cam->getRC().top + sy,
			IMAGEMANAGER->findImage("mapoption")->getFrameWidth(), IMAGEMANAGER->findImage("mapoption")->getFrameHeight());
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
	//���� �׸���Ÿ���� �ڵ��ϋ� ��ŸƮ ��ġ �������
	if (_nowdraw == AUTO)
	{
		_start.x = _mouse.x / TILESIZE;
		_start.y = _mouse.y / TILESIZE;
	}
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
	//���콺 �� �׼�
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
		int x = _mouse.x / TILESIZE;
		int y = _mouse.y / TILESIZE;

		if (_tile[y][x].obj == BULLET_KIN || _tile[y][x].obj == BULLET_BANDANA || _tile[y][x].obj == SHOTGUN_RED || _tile[y][x].obj == GUN_NUT)
		{
			if (_wheelUp) { ++_tile[y][x].sponSequence; }
			if (_wheelDown)
			{
				--_tile[y][x].sponSequence;
				if (_tile[y][x].sponSequence <= 1) { _tile[y][x].sponSequence = 1; }
			}
		}
		_wheelUp = false;
		_wheelDown = false;
	}
	//Ÿ�Ͽ� �׸��׸���
	{
		//���ú� ��Ŀ�� ���ֱ�
		for (int i = 0; i < 6; ++i)
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
	//������Ʈ ������
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
	//���� ������
	if (_page == 2)
	{
		int sy = 0, sx = 0;
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				if (PtInRect(&_enemysample[y][x].rc, _mouse))
				{
					_enemysample[y][x].select = true;
					_nowselecct = _enemysample[y][x].kind;
					_nowdraw = _enemysample[y][x].type;
					sy = y; sx = x;
				}
			}
		}

		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				if (sy == y && sx == x) { continue; }
				_enemysample[y][x].select = false;
			}
		}
	}
	//�� ������
	if (_page == 4)
	{
		int sy = 0, sx = 0;
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				if (PtInRect(&_roomsample[y][x].rc, _mouse))
				{
					_roomsample[y][x].select = true;
					_nowselecct = _roomsample[y][x].kind;
					_nowdraw = _roomsample[y][x].type;
					sy = y; sx = x;
				}
			}
		}

		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 2; ++x)
			{
				if (sy == y && sx == x) { continue; }
				_roomsample[y][x].select = false;
			}
		}
	}
	//�ɼ� ������
	if (_page == 5)
	{
		int sy = 0;
		for (int y = 0; y < 5; ++y)
		{
			if (PtInRect(&_option[y].rc, _mouse))
			{
				_option[y].select = true;
				_nowselecct = _option[y].kind;
				_nowdraw = _option[y].type;
				sy = y;
			}
		}
		for (int y = 0; y < 5; ++y)
		{
			if (sy == y) { continue; }
			_option[y].select = false;
		}
	}
}

void mapTool::pageselect(void)
{
	for (int i = 0; i < 6; ++i)
	{
		if (PtInRect(&_tagbutton[i].rc, _mouse))
		{
			_page = i;
			_tagbutton[i].select = true;
		}
	}
	for (int i = 0; i < 6; ++i)
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
		//���� �� or ��ź��
		if (_nowselecct == W_BARREL || _nowselecct == B_BARREL)
		{
			if (_tile[nowy][nowx].obj == NONE) { _tile[nowy][nowx].obj = (_nowselecct == W_BARREL) ? WOOD_BARREL : BOOM_BARREL; }
		}
		//�Ѿ� or �ݴ޸���Ʈ
		if (_nowselecct == K_BULLET || _nowselecct == B_BULLET)
		{
			if (_tile[nowy][nowx].obj == NONE)
			{
				_tile[nowy][nowx].obj = (_nowselecct == K_BULLET) ? BULLET_KIN : BULLET_BANDANA;
				_tile[nowy][nowx].sponSequence = 1;
			}
		}
		//���� or �ǳ�
		if (_nowselecct == R_SHOTGUN || _nowselecct == GUNUT)
		{
			if (_tile[nowy][nowx].obj == NONE)
			{
				_tile[nowy][nowx].obj = (_nowselecct == R_SHOTGUN) ? SHOTGUN_RED : GUN_NUT;
				_tile[nowy][nowx].sponSequence = 1;
			}
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
					if (_tile[y][x].obj != NONE)
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
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].objframeY = 0;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						continue;
					}
					if (y == startY && x == endX)//��������
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].objframeY = 0;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						continue;
					}
					if (y == endY - 2 && x == startX)//���ʾƷ�
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].objframeY = 2;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					if (y == endY - 2 && x == endX)//�����ʾƷ�
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].objframeY = 2;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					//�纯
					if (y == startY)//��
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].objframeY = 0;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
					}
					else if (y == endY - 2)//�Ʒ�
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].objframeY = 2;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == startX && y < endY - 2)//����
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].objframeY = 1;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == endX && y < endY - 2)//������
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].objframeY = 1;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY - 1) //�� ����
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].objframeY = 3;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY) //�Ʒ� ����
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].objframeY = 4;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else //�� ����
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 5 : 5;
						_tile[y][x].objframeY = 1;
						_tile[y][x].obj = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
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
		//�� �׸���
		if (_nowselecct == STON_ROOM || _nowselecct == WOOD_ROOM)
		{
			for (int y = startY; y < endY + 2; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					int bwal = RND->getFromIntTo(6, 8);
					_tile[y][x].roomnum = _makeroom;
					//�� �𼭸�
					if (y == startY && x == startX)//������
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 11 : 13;
						_tile[y][x].objframeY = 3;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						continue;
					}
					if (y == startY && x == endX)//��������
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 12 : 14;
						_tile[y][x].objframeY = 3;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						continue;
					}
					if (y == endY && x == startX)//���ʾƷ�
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 11 : 13;
						_tile[y][x].objframeY = 4;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					if (y == endY && x == endX)//�����ʾƷ�
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 12 : 14;
						_tile[y][x].objframeY = 4;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					//�纯
					if (y == startY)//��
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 8 : 5;
						_tile[y][x].objframeY = 2;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY)//�Ʒ�
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 8 : 5;
						_tile[y][x].objframeY = 0;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
					}
					else if (y == endY + 1)//���Ʒ�
					{
						_tile[y][x].objframeX = 5;
						_tile[y][x].objframeY = 1;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == startX && y < endY)//����
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 9 : 6;
						_tile[y][x].objframeY = 1;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == endX && y < endY)//������
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 7 : 4;
						_tile[y][x].objframeY = 1;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == startY + 1 && (x > startX && x < endX)) //�� ����
					{
						if (x == startX + 1) { _tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 4 : 5; }
						else if (x == endX - 1) { _tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 4 : 9; }
						else { _tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 4 : bwal; }
						_tile[y][x].objframeY = 3;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == startY + 2 && (x > startX && x < endX)) //�Ʒ� ����
					{
						if (x == startX + 1) { _tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 4 : 5; }
						else if (x == endX - 1) { _tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 4 : 9; }
						else { _tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 4 : bwal; }
						_tile[y][x].objframeY = 4;
						_tile[y][x].obj = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					//������ ���
					else if (x > startX && x < endX)
					{
						_tile[y][x].terrain = (_nowselecct == STON_ROOM) ? STON_FLOOR : WOOD_FLOOR;
						_tile[y][x].terrainX = 3;
						_tile[y][x].terrainY = (_nowselecct == STON_ROOM) ? RND->getInt(2) : 2;
					}
				}
			}
		}
		//�ٴ� ���ﶧ
		if (_nowselecct == T_ERASE)
		{
			for (int y = startY; y < endY + 1; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					_tile[y][x].terrain = EMPTY;
					_tile[y][x].terrainX = 5;
					_tile[y][x].terrainY = 1;
					_tile[y][x].pass = true;
				}
			}
		}
		//������Ʈ ���ﶧ
		if (_nowselecct == OB_ERASE)
		{
			for (int y = startY; y < endY + 1; ++y)
			{
				for (int x = startX; x < endX + 1; ++x)
				{
					_tile[y][x].obj = NONE;
					_tile[y][x].objframeX = 5;
					_tile[y][x].objframeY = 1;
					_tile[y][x].sponSequence = 0;
					_tile[y][x].pass = true;
				}
			}
		}
	}
	else if (_nowdraw == AUTO)
	{
		//���۹� ���鋚
		if (_nowselecct == START_ROOM)
		{
			bool escape = false;
			//�ϴ� ���� üũ�ؼ� �׸��� �ִ��� ������ �Ǵ�
			for (int y = _start.y; y < _start.y + 25; ++y)
			{
				for (int x = _start.x; x < _start.x + 22; ++x)
				{
					if (_tile[y][x].roomnum != 0) { break; }
				}
				if (escape) { break; }
			}

			if (!escape)
			{
				for (int ty = 20, y = _start.y; y < _start.y + 25; ++y)
				{
					for (int tx = 0, x = _start.x; x < _start.x + 22; ++x)
					{
						_tile[y][x].roomnum = _makeroom;
						//�� �𼭸�
						if (y == _start.y && x == _start.x)//������
						{
							_tile[y][x].objframeX = 11;
							_tile[y][x].objframeY = 3;
							_tile[y][x].obj = STON_WALL;
							continue;
						}
						if (y == _start.y && x == _start.x + 21)//��������
						{
							_tile[y][x].objframeX = 12;
							_tile[y][x].objframeY = 3;
							_tile[y][x].obj = STON_WALL;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x)//���ʾƷ�
						{
							_tile[y][x].objframeX = 11;
							_tile[y][x].objframeY = 4;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x + 21)//�����ʾƷ�
						{
							_tile[y][x].objframeX = 12;
							_tile[y][x].objframeY = 4;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						//�纯
						if (y == _start.y)//��
						{
							_tile[y][x].objframeX = 8;
							_tile[y][x].objframeY = 2;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 23)//�Ʒ�
						{
							_tile[y][x].objframeX = 8;
							_tile[y][x].objframeY = 0;
							_tile[y][x].obj = STON_WALL;
						}
						else if (y == _start.y + 24)//���Ʒ�
						{
							_tile[y][x].objframeX = 5;
							_tile[y][x].objframeY = 1;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x && y < _start.y + 23)//����
						{
							_tile[y][x].objframeX = 9;
							_tile[y][x].objframeY = 1;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x + 21 && y < _start.y + 23)//������
						{
							_tile[y][x].objframeX = 7;
							_tile[y][x].objframeY = 1;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 1 && (x > _start.x && x < _start.x + 21)) //�� ����
						{
							_tile[y][x].objframeX = 4;
							_tile[y][x].objframeY = 3;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 2 && (x > _start.x && x < _start.x + 21)) //�Ʒ� ����
						{
							_tile[y][x].objframeX = 4;
							_tile[y][x].objframeY = 4;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						//������ ���
						else if (x > _start.x && x < _start.x + 21)
						{
							_tile[y][x].terrain = STON_FLOOR;
							_tile[y][x].terrainX = tx;
							_tile[y][x].terrainY = ty;
							++tx;
						}
					}
					if (y > _start.y + 2 && y < _start.y + 24) { ++ty; }
				}
				++_makeroom;
			}
		}
		//������ ���鶧
		if (_nowselecct == SHOP_ROOM)
		{
			bool escape = false;
			//�ϴ� ���� üũ�ؼ� �׸��� �ִ��� ������ �Ǵ�
			for (int y = _start.y; y < _start.y + 25; ++y)
			{
				for (int x = _start.x; x < _start.x + 22; ++x)
				{
					if (_tile[y][x].roomnum != 0) { break; }
				}
				if (escape) { break; }
			}

			if (!escape)
			{
				//�ڵ�����
				for (int y = _start.y; y < _start.y + 25; ++y)
				{
					for (int x = _start.x; x < _start.x + 22; ++x)
					{
						_tile[y][x].roomnum = _makeroom;
						//�� �𼭸�
						if (y == _start.y && x == _start.x)//������
						{
							_tile[y][x].objframeX = 11;
							_tile[y][x].objframeY = 3;
							_tile[y][x].obj = STON_WALL;
							continue;
						}
						if (y == _start.y && x == _start.x + 21)//��������
						{
							_tile[y][x].objframeX = 12;
							_tile[y][x].objframeY = 3;
							_tile[y][x].obj = STON_WALL;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x)//���ʾƷ�
						{
							_tile[y][x].objframeX = 11;
							_tile[y][x].objframeY = 4;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x + 21)//�����ʾƷ�
						{
							_tile[y][x].objframeX = 12;
							_tile[y][x].objframeY = 4;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						//�纯
						if (y == _start.y)//��
						{
							_tile[y][x].objframeX = 8;
							_tile[y][x].objframeY = 2;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 23)//�Ʒ�
						{
							_tile[y][x].objframeX = 8;
							_tile[y][x].objframeY = 0;
							_tile[y][x].obj = STON_WALL;
						}
						else if (y == _start.y + 24)//���Ʒ�
						{
							_tile[y][x].objframeX = 5;
							_tile[y][x].objframeY = 1;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x && y < _start.y + 23)//����
						{
							_tile[y][x].objframeX = 9;
							_tile[y][x].objframeY = 1;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x + 21 && y < _start.y + 23)//������
						{
							_tile[y][x].objframeX = 7;
							_tile[y][x].objframeY = 1;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 1 && (x > _start.x && x < _start.x + 21)) //�� ����
						{
							_tile[y][x].objframeX = 4;
							_tile[y][x].objframeY = 3;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 2 && (x > _start.x && x < _start.x + 21)) //�Ʒ� ����
						{
							_tile[y][x].objframeX = 4;
							_tile[y][x].objframeY = 4;
							_tile[y][x].obj = STON_WALL;
							_tile[y][x].pass = false;
						}
						//������ ���
						else if (x > _start.x && x < _start.x + 21)
						{
							_tile[y][x].terrain = WOOD_FLOOR;
							_tile[y][x].terrainX = 3; 
							_tile[y][x].terrainY = 2;
						}
					}
				}
				//��������
				{
					int sy = _start.y;
					int sx = _start.x;
					//������
					{
						_tile[sy + 1][sx + 5].obj = BLANK;
						_tile[sy + 1][sx + 5].objframeX = 10;
						_tile[sy + 1][sx + 5].objframeY = 0;
						_tile[sy + 2][sx + 5].obj = BLANK;
						_tile[sy + 2][sx + 5].objframeX = 10;
						_tile[sy + 2][sx + 5].objframeY = 1;
						_tile[sy + 3][sx + 5].obj = BLANK;
						_tile[sy + 3][sx + 5].objframeX = 10;
						_tile[sy + 3][sx + 5].objframeY = 2;
						_tile[sy + 4][sx + 5].obj = BLANK;
						_tile[sy + 4][sx + 5].objframeX = 10;
						_tile[sy + 4][sx + 5].objframeY = 3;
						//������ ���
						_tile[sy + 1][sx + 6].obj = BLANK;
						_tile[sy + 1][sx + 6].objframeX = 11;
						_tile[sy + 1][sx + 6].objframeY = 0;
						_tile[sy + 2][sx + 6].obj = BLANK;
						_tile[sy + 2][sx + 6].objframeX = 11;
						_tile[sy + 2][sx + 6].objframeY = 1;
						_tile[sy + 3][sx + 6].obj = BLANK;
						_tile[sy + 3][sx + 6].objframeX = 11;
						_tile[sy + 3][sx + 6].objframeY = 2;
						//������ ������
						_tile[sy + 1][sx + 7].obj = BLANK;
						_tile[sy + 1][sx + 7].objframeX = 12;
						_tile[sy + 1][sx + 7].objframeY = 0;
						_tile[sy + 2][sx + 7].obj = BLANK;
						_tile[sy + 2][sx + 7].objframeX = 12;
						_tile[sy + 2][sx + 7].objframeY = 1;
						_tile[sy + 3][sx + 7].obj = BLANK;
						_tile[sy + 3][sx + 7].objframeX = 12;
						_tile[sy + 3][sx + 7].objframeY = 2;
					}
					//������
					{
						_tile[sy + 4][sx + 1].obj = BLANK;
						_tile[sy + 4][sx + 1].objframeX = 5;
						_tile[sy + 4][sx + 1].objframeY = 5;
						_tile[sy + 3][sx + 4].obj = BLANK;
						_tile[sy + 3][sx + 4].objframeX = 5;
						_tile[sy + 3][sx + 4].objframeY = 5;
					}
					//����
					{
						_tile[sy + 5][sx + 1].obj = BLANK;
						_tile[sy + 5][sx + 1].objframeX = 16;
						_tile[sy + 5][sx + 1].objframeY = 0;
						_tile[sy + 5][sx + 2].obj = BLANK;
						_tile[sy + 5][sx + 2].objframeX = 17;
						_tile[sy + 5][sx + 2].objframeY = 0;
						//�������
						_tile[sy + 3][sx + 2].obj = BLANK;
						_tile[sy + 3][sx + 2].objframeX = 16;
						_tile[sy + 3][sx + 2].objframeY = 1;
						_tile[sy + 4][sx + 2].obj = BLANK;
						_tile[sy + 4][sx + 2].objframeX = 16;
						_tile[sy + 4][sx + 2].objframeY = 2;
						_tile[sy + 3][sx + 3].obj = BLANK;
						_tile[sy + 3][sx + 3].objframeX = 17;
						_tile[sy + 3][sx + 3].objframeY = 1;
						_tile[sy + 4][sx + 3].obj = BLANK;
						_tile[sy + 4][sx + 3].objframeX = 17;
						_tile[sy + 4][sx + 3].objframeY = 2;
					}
					//�ѹٱ���
					{
						_tile[sy + 2][sx + 8].obj = BLANK;
						_tile[sy + 2][sx + 8].objframeX = 18;
						_tile[sy + 2][sx + 8].objframeY = 0;
						_tile[sy + 3][sx + 8].obj = BLANK;
						_tile[sy + 3][sx + 8].objframeX = 18;
						_tile[sy + 3][sx + 8].objframeY = 1;
					}
					//����
					{
						_tile[sy + 1][sx + 1].obj = BLANK;
						_tile[sy + 1][sx + 1].objframeX = 19;
						_tile[sy + 1][sx + 1].objframeY = 0;
						_tile[sy + 2][sx + 1].obj = BLANK;
						_tile[sy + 2][sx + 1].objframeX = 19;
						_tile[sy + 2][sx + 1].objframeY = 1;
					}
					//Ź��
					{
						//����
						for (int oy = 7, y = _start.y + 8; y < _start.y + 12; ++y, ++oy)
						{
							for (int ox = 7, x = _start.x + 1; x < _start.x + 14; ++x, ++ox)
							{
								_tile[y][x].obj = BLANK;
								if (y == _start.y + 8 && x == _start.x + 7) { _tile[y][x].obj = SHOP_MASTER; }
								_tile[y][x].objframeX = ox;
								_tile[y][x].objframeY = oy;
								_tile[y][x].pass = false;
							}
						}
						//����
						for (int oy = 2, y = _start.y + 3; y < _start.y + 9; ++y, ++oy)
						{
							for (int ox = 18, x = _start.x + 12; x < _start.x + 14; ++x, ++ox)
							{
								_tile[y][x].obj = BLANK;
								_tile[y][x].objframeX = ox;
								_tile[y][x].objframeY = oy;
								_tile[y][x].pass = false;
							}
						}
					}
				}
				++_makeroom;
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

void mapTool::bookup(void)
{
	//���ú� ��ư ��ġ ����
	for (int y = 40, i = 0; i < 6; ++i, y += 50)
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
	//���ʹ� ���� ���̽� ��ġ ����
	for (int sy = 94, y = 0; y < 2; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_enemysample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}
	//�� ���� ���̽� ��ġ ����
	for (int sy = 94, y = 0; y < 2; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_roomsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				IMAGEMANAGER->findImage("bigsamplecase")->getFrameWidth(), IMAGEMANAGER->findImage("bigsamplecase")->getFrameHeight());
		}
	}
	//���� �ɼ� ��ġ ����
	for (int sy = 94, y = 0; y < 5; ++y, sy += 118)
	{
		_option[y].rc = RectMake(_cam->getRC().right - 350, _cam->getRC().top + sy,
			IMAGEMANAGER->findImage("mapoption")->getFrameWidth(), IMAGEMANAGER->findImage("mapoption")->getFrameHeight());
	}
	//��ũ�ѹ� ����
	_scroolpos = (_currentscrool / _scroolmax) * 512;
}

void mapTool::draw(void)
{
	char str[8];
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
			sprintf(str, "%d", _tile[y][x].sponSequence);
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; } //Ŭ���� ���� �ȿ� ������ �Ѿ
			if (_tile[y][x].obj == NONE) { continue; } //������Ʈ�� �ȱ�� �־ �Ѿ
			if (_tile[y][x].obj == WOOD_BARREL) { IMAGEMANAGER->findImage("barrels")->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 0, 0); continue; } //�������̸� �׸��� �Ѿ
			else if (_tile[y][x].obj == BOOM_BARREL) { IMAGEMANAGER->findImage("barrels")->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 1, 0); continue; } //��ź���̸� �׸��� �Ѿ
			else if (_tile[y][x].obj == BULLET_KIN) //�Ѿ��̸� �׸��� �Ѿ
			{
				IMAGEMANAGER->findImage("enemys")->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64, 0, 0);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, str, strlen(str)); continue;
			}
			else if (_tile[y][x].obj == BULLET_BANDANA) //�ݴ޸���Ʈ�� �׸��� �Ѿ
			{
				IMAGEMANAGER->findImage("enemys")->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64 , 1, 0);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, str, strlen(str)); continue;
			} 
			else if (_tile[y][x].obj == SHOTGUN_RED) //�����̸� �׸��� �Ѿ
			{
				IMAGEMANAGER->findImage("enemys")->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64, 0, 1);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 48, str, strlen(str)); continue;
			}
			else if (_tile[y][x].obj == GUN_NUT) //�ǳ��̸� �׸��� �Ѿ
			{
				IMAGEMANAGER->findImage("enemys")->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64, 1, 1);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 48, str, strlen(str)); continue;
			}
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

		//������Ʈ ��
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
		//���� ��
		if (_page == 2)
		{
			for (int y = 0; y < 2; ++y)
			{
				for (int x = 0; x < 2; ++x)
				{
					if (_enemysample[y][x].select) { IMAGEMANAGER->findImage("bigsamplecase")->frameRender(getBackDC(), _enemysample[y][x].rc.left, _enemysample[y][x].rc.top, 1, 0); }
					else { IMAGEMANAGER->findImage("bigsamplecase")->frameRender(getBackDC(), _enemysample[y][x].rc.left, _enemysample[y][x].rc.top, 0, 0); }
					IMAGEMANAGER->findImage("enemysample")->frameRender(getBackDC(), _enemysample[y][x].rc.left + 10, _enemysample[y][x].rc.top + 30, x, y);
				}
			}
		}
		//��
		if (_page == 4)
		{
			for (int y = 0; y < 2; ++y)
			{
				for (int x = 0; x < 2; ++x)
				{
					if (_roomsample[y][x].select) { IMAGEMANAGER->findImage("bigsamplecase")->frameRender(getBackDC(), _roomsample[y][x].rc.left, _roomsample[y][x].rc.top, 1, 0); }
					else { IMAGEMANAGER->findImage("bigsamplecase")->frameRender(getBackDC(), _roomsample[y][x].rc.left, _roomsample[y][x].rc.top, 0, 0); }
					IMAGEMANAGER->findImage("roomsample")->frameRender(getBackDC(), _roomsample[y][x].rc.left + 10, _roomsample[y][x].rc.top + 30, x, y);
				}
			}
		}
		//�ɼ�
		if (_page == 5)
		{
			for (int y = 0; y < 5; ++y)
			{
				if (_option[y].select) { IMAGEMANAGER->findImage("mapoption")->frameRender(getBackDC(), _option[y].rc.left, _option[y].rc.top, 1, y); }
				else { IMAGEMANAGER->findImage("mapoption")->frameRender(getBackDC(), _option[y].rc.left, _option[y].rc.top, 0, y); }
			}
		}

		//���ú� Ŀ��
		IMAGEMANAGER->findImage("cover")->render(getBackDC(), _cam->getRC().right - _book->getWidth(), _cam->getRC().top);
		//��ũ�ѹ� �� �κ�
		IMAGEMANAGER->findImage("scrollbardown")->render(getBackDC(), _cam->getRC().right - 34, _cam->getRC().top + 94);
		IMAGEMANAGER->findImage("scrollbar")->render(getBackDC(), _cam->getRC().right - 34, (_cam->getRC().top + 94) + _scroolpos);
		//�±�
		for (int y = 1, i = 0; i < 6; ++i, y += 2)
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
	if (_nowdraw == SAMIAUTO)
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
	if (_nowdraw == AUTO && !_sampleOpen)
	{
		//���۹� �׸� ���� üũ
		if (_nowselecct == START_ROOM)
		{
			bool escape = false;
			//�ϴ� ���� üũ�ؼ� �׸��� �ִ��� ������ �Ǵ�
			for (int y = _start.y; y < _start.y + 25; ++y)
			{
				for (int x = _start.x; x < _start.x + 23; ++x)
				{
					if (_tile[y][x].roomnum != 0) { escape = true; break; }
				}
				if (escape) { break; }
			}
			//�� ���׷�
			if (escape)
			{
				for (int y = _start.y; y < _start.y + 25; ++y)
				{
					for (int x = _start.x; x < _start.x + 22; ++x)
					{
						if (!IntersectRect(&RectMake(0, 0, 0, 0), &_cam->getRC(), &_tile[y][x].rc)) { continue; }
						IMAGEMANAGER->findImage("no")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
					}
				}
			}
			//�� �׸��� �־�
			else
			{
				for (int y = _start.y; y < _start.y + 25; ++y)
				{
					for (int x = _start.x; x < _start.x + 22; ++x)
					{
						if (!IntersectRect(&RectMake(0, 0, 0, 0), &_cam->getRC(), &_tile[y][x].rc)) { continue; }
						IMAGEMANAGER->findImage("ok")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
					}
				}
			}
		}
		//������ �׸� ���� üũ
		if (_nowselecct == SHOP_ROOM)
		{
			bool escape = false;
			//�ϴ� ���� üũ�ؼ� �׸��� �ִ��� ������ �Ǵ�
			for (int y = _start.y; y < _start.y + 25; ++y)
			{
				for (int x = _start.x; x < _start.x + 23; ++x)
				{
					if (_tile[y][x].roomnum != 0) { escape = true; break; }
				}
				if (escape) { break; }
			}
			//�� ���׷�
			if (escape)
			{
				for (int y = _start.y; y < _start.y + 25; ++y)
				{
					for (int x = _start.x; x < _start.x + 22; ++x)
					{
						if (!IntersectRect(&RectMake(0, 0, 0, 0), &_cam->getRC(), &_tile[y][x].rc)) { continue; }
						IMAGEMANAGER->findImage("no")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
					}
				}
			}
			//�� �׸��� �־�
			else
			{
				for (int y = _start.y; y < _start.y + 25; ++y)
				{
					for (int x = _start.x; x < _start.x + 22; ++x)
					{
						if (!IntersectRect(&RectMake(0, 0, 0, 0), &_cam->getRC(), &_tile[y][x].rc)) { continue; }
						IMAGEMANAGER->findImage("ok")->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
					}
				}
			}
		}
	}
}