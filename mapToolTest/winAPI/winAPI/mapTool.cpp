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

	if (KEYMANAGER->isOnceKeyDown(VK_BACK))
	{
		SCENEMANAGER->changeScene("titleScene");
	}
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
		_sample = IMAGEMANAGER->findImage("tile");
		_cimg = IMAGEMANAGER->findImage("bimg");
		_miniimg = IMAGEMANAGER->findImage("mimg");
		_book = IMAGEMANAGER->findImage("book");
		_alhpa = IMAGEMANAGER->findImage("minimap");
		_minimapcase = IMAGEMANAGER->findImage("minimapcase");
		_black = IMAGEMANAGER->findImage("black");
		_ok = IMAGEMANAGER->findImage("ok");
		_no = IMAGEMANAGER->findImage("no");
		_cover = IMAGEMANAGER->findImage("cover");
		_bigsamplecase = IMAGEMANAGER->findImage("bigsamplecase");
		_bosscase = IMAGEMANAGER->findImage("bosscase");
		_mapoption = IMAGEMANAGER->findImage("mapoption");
		_bigsampleimg = IMAGEMANAGER->findImage("bigsample");
		_enemysampleimg = IMAGEMANAGER->findImage("enemysample");
		_bosssampleimg = IMAGEMANAGER->findImage("bosssample");
		_roomsampleimg = IMAGEMANAGER->findImage("roomsample");
		_booktag = IMAGEMANAGER->findImage("booktag");
		_scrollbardown = IMAGEMANAGER->findImage("scrollbardown");
		_scrollbar = IMAGEMANAGER->findImage("scrollbar");
		_barrels = IMAGEMANAGER->findImage("barrels");
		_enemys = IMAGEMANAGER->findImage("enemys");
		_boss = IMAGEMANAGER->findImage("bossimg");
	}
	//�پ��� ���� �ʱ�ȭ
	{
		_campt = PointMake(WINSIZEX / 2, WINSIZEY / 2);
		_mouse = _ptadd = _basept = _minimappt = PointMake(0, 0);
		_sampleOpen = false;
		_mapOpen = false;
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
	HPEN pen = CreatePen(NULL, 1, RGB(255, 255, 255));
	SelectObject(getBackDC(), pen);

	_cam = new camera;
	_cam->init(&_campt, NULL, _cimg, false);

	//���ú� �±� �ʱ�ȭ
	for (int y = 40, i = 0; i < 6; ++i, y += 50)
	{
		_tagbutton[i].onmouse = _tagbutton[i].select = false;
		_tagbutton[i].rc = RectMake(_cam->getRC().right - _booktag->getFrameWidth() - _book->getWidth() + 10, _cam->getRC().top + y,
			_booktag->getFrameWidth(), _booktag->getFrameHeight());
	}
	_tagbutton[0].select = true;

	//��������̽� �ʱ�ȭ
	for (int sy = 94, y = 0; y < 5; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			if (y == 0) { _bigsample[y][x].type = SAMIAUTO; _bigsample[y][x].kind = (x == 0) ? S_WALL : W_WALL; }
			if (y == 1) { _bigsample[y][x].type = SAMIAUTO; _bigsample[y][x].kind = (x == 0) ? CARPT_A : CARPT_B; }
			if (y == 2) { _bigsample[y][x].type = AUTO; _bigsample[y][x].kind = (x == 0) ? O_TP : O_DOOR; }
			if (y == 3) { _bigsample[y][x].type = STICK; _bigsample[y][x].kind = (x == 0) ? L_TABLE : W_TABLE; }
			if (y == 4) { _bigsample[y][x].type = STICK; _bigsample[y][x].kind = (x == 0) ? W_BARREL : B_BARREL; }
			_bigsample[y][x].select = false;
			_bigsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				_bigsamplecase->getFrameWidth(), _bigsamplecase->getFrameHeight());
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
				_bigsamplecase->getFrameWidth(), _bigsamplecase->getFrameHeight());
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
				_bigsamplecase->getFrameWidth(), _bigsamplecase->getFrameHeight());
		}
	}

	//�����������̽� �ʱ�ȭ
	_bosssample.type = STICK;
	_bosssample.kind = KING_BULLET;
	_bosssample.select = false;
	_bosssample.rc = RectMake(_cam->getRC().right - 350 + 20, _cam->getRC().top + 94,
		_bosscase->getFrameWidth(), _bosscase->getFrameHeight());

	//���� �ɼ� �ʱ�ȭ
	for (int sy = 94, y = 0; y < 5; ++y, sy += 118)
	{
		if (y == 0) { _option[y].type = SAMIAUTO; _option[y].kind = T_ERASE; }
		if (y == 1) { _option[y].type = SAMIAUTO; _option[y].kind = OB_ERASE; }
		if (y == 2) { _option[y].type = AUTO; _option[y].kind = R_ERASE; }
		_option[y].select = false;
		_option[y].rc = RectMake(_cam->getRC().right - 350, _cam->getRC().top + sy,
			_bigsamplecase->getFrameWidth(), _bigsamplecase->getFrameHeight());
	}

	//Ÿ�� �ʱ�ȭ
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			_tile[y][x].obj = NONE;
			_tile[y][x].pass = true;
			_tile[y][x].show = false;
			_tile[y][x].wall = VOID_WALL;
			_tile[y][x].terrain = EMPTY;
			_tile[y][x].terrainX = _tile[y][x].objframeX = _tile[y][x].wallX = 5;
			_tile[y][x].terrainY = _tile[y][x].objframeY = _tile[y][x].wallY = 1;
			_tile[y][x].roomnum = _tile[y][x].sponSequence = 0;
			_tile[y][x].rc = RectMake(x * TILESIZE, y * TILESIZE, TILESIZE, TILESIZE);
			_tile[y][x].minirc = RectMake(x * _TILEMINISIZE, y * _TILEMINISIZE, _TILEMINISIZE, _TILEMINISIZE);
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
	//�̴ϸ�
	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		_mapOpen = (_mapOpen == false) ? true : false;
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
	else if (_mapOpen)
	{
		if (_wheelUp)
		{
			++_TILEMINISIZE;
			if (_TILEMINISIZE > 8) { _TILEMINISIZE = 8; }
		}
		if (_wheelDown)
		{
			--_TILEMINISIZE;
			if (_TILEMINISIZE < 4) { _TILEMINISIZE = 4; }
		}
		for (int y = 0; y < TILEY; ++y)
		{
			for (int x = 0; x < TILEX; ++x)
			{
				_tile[y][x].minirc = RectMake(x * _TILEMINISIZE, y * _TILEMINISIZE, _TILEMINISIZE, _TILEMINISIZE);
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
			else if (_mapOpen) { _start.x = _mouse.x; _start.y = _mouse.y; }
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
			else if (_mapOpen)
			{
				int width = (_mouse.x < _start.x) ? _start.x - _mouse.x : _mouse.x - _start.x;
				int height = (_mouse.y < _start.y) ? _start.y - _mouse.y : _mouse.y - _start.y;
				(_mouse.x < _start.x) ? _minimappt.x = _basept.x + width : _minimappt.x = _basept.x - width;
				(_mouse.y < _start.y) ? _minimappt.y = _basept.y + height : _minimappt.y = _basept.y - height;
				if (_minimappt.x < 0) { _minimappt.x = 0; }
				if (_minimappt.x > _miniimg->getWidth() - 800) { _minimappt.x = _miniimg->getWidth() - 800; }
				if (_minimappt.y < 0) { _minimappt.y = 0; }
				if (_minimappt.y > _miniimg->getHeight() - 600) { _minimappt.y = _miniimg->getHeight() - 600; }
			}
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
			if (_sampleOpen) {}
			else if (_mapOpen) { _basept = _minimappt; }
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
	//���� ������
	if (_page == 3)
	{
		if (PtInRect(&_bosssample.rc, _mouse))
		{
			_bosssample.select = true;
			_nowselecct = _bosssample.kind;
			_nowdraw = _bosssample.type;
		}
		else { _bosssample.select = false; }
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
				//���̺�
				if (y == 3)
				{
					HANDLE file, file2, file3;
					DWORD write;

					file = CreateFile("stageOne", GENERIC_WRITE, 0, NULL,
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

					WriteFile(file, _tile, sizeof(tagTile) * TILEX * TILEY, &write, NULL);

					file2 = CreateFile("spon", GENERIC_WRITE, 0, NULL,
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

					WriteFile(file2, &DATABASE->spon, sizeof(POINT), &write, NULL);

					file3 = CreateFile("roomscale", GENERIC_WRITE, 0, NULL,
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

					WriteFile(file3, &_makeroom, sizeof(int), &write, NULL);

					CloseHandle(file);
					CloseHandle(file2);
					CloseHandle(file3);
				}
				//�ε�
				if (y == 4)
				{
					HANDLE file, file2, file3;
					DWORD read;

					file = CreateFile("stageOne", GENERIC_READ, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					ReadFile(file, _tile, sizeof(tagTile) * TILEX * TILEY, &read, NULL);

					file2 = CreateFile("spon", GENERIC_READ, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					ReadFile(file2, &DATABASE->spon, sizeof(POINT), &read, NULL);

					file3 = CreateFile("roomscale", GENERIC_READ, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					ReadFile(file3, &_makeroom, sizeof(int), &read, NULL);

					CloseHandle(file);
					CloseHandle(file2);
					CloseHandle(file3);
				}
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
			if ((_tile[nowy][nowx].obj == NONE && _tile[nowy][nowx + 1].obj == NONE) &&
				(_tile[nowy][nowx].wall == VOID_WALL && _tile[nowy][nowx + 1].wall == VOID_WALL))
			{
				_tile[nowy][nowx].objframeX = 3;
				_tile[nowy][nowx].objframeY = 5;
				_tile[nowy][nowx + 1].objframeX = 4;
				_tile[nowy][nowx + 1].objframeY = 5;
				_tile[nowy][nowx].obj = TABLE_WIDTH;
				_tile[nowy][nowx + 1].obj = BLANK;
				_tile[nowy][nowx].pass = false;
				_tile[nowy][nowx + 1].pass = false;
			}
		}
		//���� ���̺�
		if (_nowselecct == L_TABLE)
		{
			if ((_tile[nowy][nowx].obj == NONE && _tile[nowy - 1][nowx].obj == NONE) &&
				_tile[nowy][nowx].wall == VOID_WALL && _tile[nowy - 1][nowx].wall == VOID_WALL)
			{
				_tile[nowy][nowx].objframeX = 6;
				_tile[nowy][nowx].objframeY = 6;
				_tile[nowy - 1][nowx].objframeX = 6;
				_tile[nowy - 1][nowx].objframeY = 5;
				_tile[nowy][nowx].obj = TABLE_LENGTH;
				_tile[nowy - 1][nowx].obj = BLANK;
				_tile[nowy][nowx].pass = false;
				_tile[nowy - 1][nowx].pass = false;
			}
		}
		//���� �� or ��ź��
		if (_nowselecct == W_BARREL || _nowselecct == B_BARREL)
		{
			if (_tile[nowy][nowx].obj == NONE && _tile[nowy][nowx].wall == VOID_WALL) { _tile[nowy][nowx].obj = (_nowselecct == W_BARREL) ? WOOD_BARREL : BOOM_BARREL; }
		}
		//�Ѿ� or �ݴ޸���Ʈ
		if (_nowselecct == K_BULLET || _nowselecct == B_BULLET)
		{
			if (_tile[nowy][nowx].obj == NONE && _tile[nowy][nowx].wall == VOID_WALL)
			{
				_tile[nowy][nowx].obj = (_nowselecct == K_BULLET) ? BULLET_KIN : BULLET_BANDANA;
				_tile[nowy][nowx].sponSequence = 1;
			}
		}
		//���� or �ǳ�
		if (_nowselecct == R_SHOTGUN || _nowselecct == GUNUT)
		{
			if (_tile[nowy][nowx].obj == NONE && _tile[nowy][nowx].wall == VOID_WALL)
			{
				_tile[nowy][nowx].obj = (_nowselecct == R_SHOTGUN) ? SHOTGUN_RED : GUN_NUT;
				_tile[nowy][nowx].sponSequence = 1;
			}
		}
		//����
		if (_nowselecct == KING_BULLET)
		{
			if (_tile[nowy][nowx].obj == NONE && _tile[nowy][nowx].wall == VOID_WALL)
			{
				_tile[nowy][nowx].obj = BULLET_KING;
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
					if (_tile[y][x].obj != NONE || _tile[y][x].wall != VOID_WALL)
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
						_tile[y][x].obj = WALL_TOP;
						continue;
					}
					if (y == startY && x == endX)//��������
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].objframeY = 0;
						_tile[y][x].obj = WALL_TOP;
						continue;
					}
					if (y == endY - 2 && x == startX)//���ʾƷ�
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].wallY = 2;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					if (y == endY - 2 && x == endX)//�����ʾƷ�
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].wallY = 2;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					//�纯
					if (y == startY)//��
					{
						_tile[y][x].objframeX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].objframeY = 0;
						_tile[y][x].obj = WALL_TOP;
					}
					else if (y == endY - 2)//�Ʒ�
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 8 : 5;
						_tile[y][x].wallY = 2;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == startX && y < endY - 2)//����
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 7 : 4;
						_tile[y][x].wallY = 1;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == endX && y < endY - 2)//������
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 9 : 6;
						_tile[y][x].wallY = 1;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY - 1) //�� ����
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].wallY = 3;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY) //�Ʒ� ����
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 4 : 3;
						_tile[y][x].wallY = 4;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else //�� ����
					{
						_tile[y][x].wallX = (_nowselecct == S_WALL) ? 5 : 5;
						_tile[y][x].wallY = 1;
						_tile[y][x].wall = (_nowselecct == S_WALL) ? STON_WALL : BOOK_WALL;
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
					//�� �𼭸�
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
						_tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 11 : 13;
						_tile[y][x].wallY = 3;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						continue;
					}
					if (y == startY && x == endX)//��������
					{
						_tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 12 : 14;
						_tile[y][x].wallY = 3;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						continue;
					}
					if (y == endY && x == startX)//���ʾƷ�
					{
						_tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 11 : 13;
						_tile[y][x].wallY = 4;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					if (y == endY && x == endX)//�����ʾƷ�
					{
						_tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 12 : 14;
						_tile[y][x].wallY = 4;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
						continue;
					}
					//�纯
					if (y == startY)//��
					{
						_tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 8 : 5;
						_tile[y][x].wallY = 2;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == endY)//�Ʒ�
					{
						_tile[y][x].objframeX = (_nowselecct == STON_ROOM) ? 8 : 5;
						_tile[y][x].objframeY = 0;
						_tile[y][x].obj = WALL_TOP;
					}
					else if (y == endY + 1)//���Ʒ�
					{
						_tile[y][x].wallX = 5;
						_tile[y][x].wallY = 1;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == startX && y < endY)//����
					{
						_tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 9 : 6;
						_tile[y][x].wallY = 1;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (x == endX && y < endY)//������
					{
						_tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 7 : 4;
						_tile[y][x].wallY = 1;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == startY + 1 && (x > startX && x < endX)) //�� ����
					{
						if (x == startX + 1) { _tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 4 : 5; }
						else if (x == endX - 1) { _tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 4 : 9; }
						else { _tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 4 : bwal; }
						_tile[y][x].wallY = 3;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
						_tile[y][x].pass = false;
					}
					else if (y == startY + 2 && (x > startX && x < endX)) //�Ʒ� ����
					{
						if (x == startX + 1) { _tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 4 : 5; }
						else if (x == endX - 1) { _tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 4 : 9; }
						else { _tile[y][x].wallX = (_nowselecct == STON_ROOM) ? 4 : bwal; }
						_tile[y][x].wallY = 4;
						_tile[y][x].wall = (_nowselecct == STON_ROOM) ? STON_WALL : BOOK_WALL;
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
			++_makeroom;
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
					_tile[y][x].wall = VOID_WALL;
					_tile[y][x].objframeX = _tile[y][x].wallX = 5;
					_tile[y][x].objframeY = _tile[y][x].wallY = 1;
					_tile[y][x].sponSequence = 0;
					_tile[y][x].pass = true;
				}
			}
		}
	}
	else if (_nowdraw == AUTO)
	{
		//�ڷ����� ���鶧
		if (_nowselecct == O_TP)
		{
			if (_tile[_start.y][_start.x].obj == NONE && _tile[_start.y][_start.x].wall == VOID_WALL)
			{
				for (int ty = 6, y = _start.y; y < _start.y + 2; ++y, ++ty)
				{
					for (int tx = 0, x = _start.x; x < _start.x + 2; ++x, ++tx)
					{
						if (y == _start.y && x == _start.x) { _tile[y][x].obj = TELEPOTER; }
						else { _tile[y][x].obj = BLANK; }
						_tile[y][x].objframeX = tx;
						_tile[y][x].objframeY = ty;
					}
				}
			}
		}
		//�� ��ġ�Ҷ�
		if (_nowselecct == O_DOOR)
		{
			if (_tile[_start.y][_start.x].roomnum != 0)
			{
				bool makeok = true;
				int sy, sx, ey, ex;
				sy = ey = _start.y; sx = ex = _start.x;
				if (_tile[sy - 1][sx].roomnum == 0 || _tile[sy + 2][sx].roomnum == 0) //���Ǵ� �Ʒ��� ������
				{
					bool UorD = false; //flase�� �Ʒ���
					UorD = (_tile[sy - 1][sx].roomnum == 0) ? true : false;

					if (_tile[sy - 1][sx].roomnum == 0)
					{
						while (_tile[sy - 1][sx].roomnum == 0) { if (sy - 1 == 0) { makeok = false; break; } --sy; }
						sy -= 2;
						++ey;
					}
					else if (_tile[sy + 2][sx].roomnum == 0)
					{
						while (_tile[ey + 2][sx].roomnum == 0) { if (ey + 3 >= TILEY) { makeok = false; break; } ++ey; }
						ey += 3;
					}
					if (!makeok) { return; }
					ex += 4;

					//����� ���ô�
					for (int y = sy; y < ey; ++y)
					{
						for (int x = sx; x < ex; ++x)
						{
							//��ο� ���ȣ ����
							if (UorD) { _tile[y][x].roomnum = _tile[ey - 1][sx].roomnum; }
							else { _tile[y][x].roomnum = _tile[sy][sx].roomnum; }
							//�� �𼭸�
							if (y == sy && x == sx)//������
							{
								_tile[y][x].objframeX = (_tile[y + 1][x].wall == STON_WALL) ? 9 : 6;
								_tile[y][x].objframeY = 0;
								_tile[y][x].obj = WALL_TOP;
								_tile[y][x].wall = VOID_WALL;
								_tile[y][x].pass = true;
								continue;
							}
							if (y == sy && x == ex - 1)//��������
							{
								_tile[y][x].objframeX = (_tile[y + 1][x].wall == STON_WALL) ? 7 : 4;
								_tile[y][x].objframeY = 0;
								_tile[y][x].obj = WALL_TOP;
								_tile[y][x].wall = VOID_WALL;
								_tile[y][x].pass = true;
								continue;
							}
							if (y == ey - 1 && x == sx)//���ʾƷ�
							{
								if (_tile[y][x].wall == STON_WALL)
								{
									_tile[y][x].wallX = 9;
									_tile[y][x].wallY = 2;
									_tile[y][x].wall = STON_WALL;
									_tile[y][x].pass = false;
								}
								else
								{
									_tile[y][x].wallX = 6;
									_tile[y][x].wallY = 2;
									_tile[y][x].wall = BOOK_WALL;
									_tile[y][x].pass = false;
									_tile[y + 1][x].wallX = 9;
									_tile[y + 1][x].wallY = 3;
									_tile[y + 1][x].wall = BOOK_WALL;
									_tile[y + 1][x].pass = false;
									_tile[y + 2][x].wallX = 9;
									_tile[y + 2][x].wallY = 4;
									_tile[y + 2][x].wall = BOOK_WALL;
									_tile[y + 2][x].pass = false;
								}
								continue;
							}
							if (y == ey - 1 && x == ex - 1)//�����ʾƷ�
							{
								if (_tile[y][x].wall == STON_WALL)
								{
									_tile[y][x].wallX = 7;
									_tile[y][x].wallY = 2;
									_tile[y][x].wall = STON_WALL;
									_tile[y][x].pass = false;
								}
								else
								{
									_tile[y][x].wallX = 4;
									_tile[y][x].wallY = 2;
									_tile[y][x].wall = BOOK_WALL;
									_tile[y][x].pass = false;
									_tile[y + 1][x].wallX = 5;
									_tile[y + 1][x].wallY = 3;
									_tile[y + 1][x].wall = BOOK_WALL;
									_tile[y + 1][x].pass = false;
									_tile[y + 2][x].wallX = 5;
									_tile[y + 2][x].wallY = 4;
									_tile[y + 2][x].wall = BOOK_WALL;
									_tile[y + 2][x].pass = false;
								}
								continue;
							}
							//�纯
							if (y == sy) //��
							{
								_tile[y][x].terrainX = 3;
								_tile[y][x].terrainY = (_tile[y + 1][x].wall == STON_WALL) ? RND->getInt(2) : 2;
								_tile[y][x].terrain = (_tile[y + 1][x].wall == STON_WALL) ? STON_FLOOR : WOOD_FLOOR;
								_tile[y][x].obj = NONE;
								_tile[y][x].wall = VOID_WALL;
								_tile[y][x].pass = true;
								_tile[y + 1][x].terrainX = 3;
								_tile[y + 1][x].terrainY = RND->getInt(2);;
								_tile[y + 1][x].terrain = STON_FLOOR;
								_tile[y + 1][x].objframeX = (x == sx + 1) ? 7 : 8;
								_tile[y + 1][x].objframeY = 5;
								_tile[y + 1][x].obj = (x == sx + 1) ? DOOR_LENGTH : BLANK;
								_tile[y + 1][x].wall = VOID_WALL;
								_tile[y + 1][x].pass = true;
								_tile[y + 2][x].terrainX = 3;
								_tile[y + 2][x].terrainY = RND->getInt(2);;
								_tile[y + 2][x].terrain = STON_FLOOR;
								_tile[y + 2][x].objframeX = (x == sx + 1) ? 7 : 8;
								_tile[y + 2][x].objframeY = 6;
								_tile[y + 2][x].obj = BLANK;
								_tile[y + 2][x].wall = VOID_WALL;
								_tile[y + 2][x].pass = true;
							}
							else if (y == ey - 1) //�Ʒ�
							{
								_tile[y][x].terrainX = 3;
								_tile[y][x].terrainY = RND->getInt(2);;
								_tile[y][x].terrain = STON_FLOOR;
								_tile[y][x].objframeX = (x == sx + 1) ? 7 : 8;
								_tile[y][x].objframeY = 5;
								_tile[y][x].obj = (x == sx + 1) ? DOOR_LENGTH : BLANK;
								_tile[y][x].wall = VOID_WALL;
								_tile[y][x].pass = true;
								_tile[y + 1][x].terrainX = 3;
								_tile[y + 1][x].terrainY = RND->getInt(2);;
								_tile[y + 1][x].terrain = STON_FLOOR;
								_tile[y + 1][x].objframeX = (x == sx + 1) ? 7 : 8;
								_tile[y + 1][x].objframeY = 6;
								_tile[y + 1][x].obj = BLANK;
								_tile[y + 1][x].wall = VOID_WALL;
								_tile[y + 1][x].pass = true;
								_tile[y + 2][x].terrainX = 3;
								_tile[y + 2][x].terrainY = (_tile[y + 2][x].wall == STON_WALL) ? RND->getInt(2) : 2;
								_tile[y + 2][x].terrain = (_tile[y + 2][x].wall == STON_WALL) ? STON_FLOOR : WOOD_FLOOR;
								_tile[y + 2][x].wall = VOID_WALL;
								_tile[y + 2][x].obj = NONE;
								_tile[y + 2][x].pass = true;
							}
							else if (x == sx)//����
							{
								_tile[y][x].wallX = (_tile[ey - 1][ex - 1].wall == STON_WALL) ? 9 : 6;
								_tile[y][x].wallY = 1;
								_tile[y][x].wall = (_tile[ey - 1][ex - 1].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
								_tile[y][x].pass = false;
							}
							else if (x == ex - 1)//������
							{
								_tile[y][x].wallX = (_tile[ey - 1][ex - 1].wall == STON_WALL) ? 7 : 4;
								_tile[y][x].wallY = 1;
								_tile[y][x].wall = (_tile[ey - 1][ex - 1].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
								_tile[y][x].pass = false;
							}
							else
							{
								_tile[y][x].terrainX = 3;
								_tile[y][x].terrainY = RND->getInt(2);;
								_tile[y][x].terrain = STON_FLOOR;
								_tile[y][x].pass = true;
							}
						}
					}

					if (UorD)
					{
						_tile[sy + 1][sx + 1].roomnum = _tile[sy - 1][sx].roomnum;
						_tile[ey - 1][sx + 1].roomnum = _tile[ey][sx].roomnum;
					}
					else
					{
						_tile[sy + 1][sx + 1].roomnum = _tile[sy][sx].roomnum;
						_tile[ey - 1][sx + 1].roomnum = _tile[ey][sx].roomnum;
					}					
				}
				if (_tile[sy][sx - 1].roomnum == 0 || _tile[sy][sx + 1].roomnum == 0) //�����̳� ���������� ������
				{
					bool LorR = false; //flase �� ������
					LorR = (_tile[sy][sx - 1].roomnum == 0) ? true : false;

					if (_tile[sy][sx - 1].roomnum == 0)
					{
						while (_tile[sy][sx - 1].roomnum == 0) { if (sx - 1 <= 0) { makeok = false; break; } --sx; }
						sx -= 1;
						++ex;
					}
					else if (_tile[sy][sx + 1].roomnum == 0)
					{
						while (_tile[sy][ex + 1].roomnum == 0) { if (ex + 1 >= TILEX) { makeok = false; break; } ++ex; }
						ex += 2;
					}
					if (!makeok) { return; }
					ey += 5;					

					//�׷����ô�
					for (int y = sy; y < ey; ++y)
					{
						for (int x = sx; x < ex; ++x)
						{
							//��ο� ���ȣ �ֱ�
							if (LorR) { _tile[y][x].roomnum = _tile[sy][ex - 1].roomnum; }
							else { _tile[y][x].roomnum = _tile[sy][sx].roomnum; }
							//�� �𼭸�
							if (y == sy && x == sx)//������
							{
								_tile[y][x].wallX = (_tile[y][x].wall == STON_WALL) ? 7 : 4;
								_tile[y][x].wallY = 2;
								_tile[y][x].wall = (_tile[y][x].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
								continue;
							}
							if (y == sy && x == ex - 1)//��������
							{
								_tile[y][x].wallX = (_tile[y][x].wall == STON_WALL) ? 9 : 6;
								_tile[y][x].wallY = 2;
								_tile[y][x].wall = (_tile[y][x].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
								continue;
							}
							if (y == ey - 1 && x == sx)//���ʾƷ�
							{
								_tile[y][x].objframeX = (_tile[y][x].wall == STON_WALL) ? 7 : 4;
								_tile[y][x].objframeY = 0;
								_tile[y][x].obj = WALL_TOP;
								_tile[y][x].pass = true;
								continue;
							}
							if (y == ey - 1 && x == ex - 1)//�����ʾƷ�
							{
								_tile[y][x].objframeX = (_tile[y][x].wall == STON_WALL) ? 9 : 6;
								_tile[y][x].objframeY = 0;
								_tile[y][x].obj = WALL_TOP;
								_tile[y][x].pass = true;
								continue;
							}
							//�纯
							if (y == sy)//��
							{
								_tile[y][x].wallX = (_tile[y][ex - 1].wall == STON_WALL) ? 8 : 5;
								_tile[y][x].wallY = 2;
								_tile[y][x].wall = (_tile[y][ex - 1].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
								_tile[y][x].pass = false;
							}
							else if (y == sy + 1)//�� ����
							{
								if (x == sx)
								{
									_tile[y][x].wallX = (_tile[y][x].wall == STON_WALL) ? 4 : 3;
									_tile[y][x].wallY = 3;
									_tile[y][x].wall = (_tile[y][x].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
									_tile[y][x].pass = false;
								}
								else
								{
									_tile[y][x].wallX = (_tile[y][ex - 1].wall == STON_WALL) ? 4 : 3;
									_tile[y][x].wallY = 3;
									_tile[y][x].wall = (_tile[y][ex - 1].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
									_tile[y][x].pass = false;
								}
							}
							else if (y == sy + 2)//�Ʒ� ����
							{
								if (x == sx)
								{
									_tile[y][x].wallX = (_tile[y][x].wall == STON_WALL) ? 4 : 3;
									_tile[y][x].wallY = 4;
									_tile[y][x].wall = (_tile[y][x].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
									_tile[y][x].pass = false;
								}
								else
								{
									_tile[y][x].wallX = (_tile[y][ex - 1].wall == STON_WALL) ? 4 : 3;
									_tile[y][x].wallY = 4;
									_tile[y][x].wall = (_tile[y][ex - 1].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
									_tile[y][x].pass = false;
								}
							}
							else if (y == ey - 1)//�Ʒ�
							{
								_tile[y][x].objframeX = (_tile[y][ex - 1].wall == STON_WALL) ? 8 : 5;
								_tile[y][x].objframeY = 0;
								_tile[y][x].obj = WALL_TOP;
								_tile[y][x].pass = true;
								_tile[y + 1][x].wallX = 5;
								_tile[y + 1][x].wallY = 1;
								_tile[y + 1][x].wall = (_tile[y][ex - 1].wall == STON_WALL) ? STON_WALL : BOOK_WALL;
								_tile[y + 1][x].pass = false;
							}
							else if (x == sx)//����
							{
								_tile[y][x].terrainX = 3;
								_tile[y][x].terrainY = (_tile[y][x].wall == STON_WALL) ? RND->getInt(2) : 2;
								_tile[y][x].terrain = (_tile[y][x].wall == STON_WALL) ? STON_FLOOR : WOOD_FLOOR;
								_tile[y][x].wall = VOID_WALL;
								_tile[y][x].pass = true;
							}
							else if (x == ex - 1)//������
							{
								_tile[y][x].terrainX = 3;
								_tile[y][x].terrainY = (_tile[y][x].wall == STON_WALL) ? RND->getInt(2) : 2;
								_tile[y][x].terrain = (_tile[y][x].wall == STON_WALL) ? STON_FLOOR : WOOD_FLOOR;
								_tile[y][x].wall = VOID_WALL;
								_tile[y][x].pass = true;
							}
							else
							{
								_tile[y][x].terrainX = 3;
								_tile[y][x].terrainY = RND->getInt(2);;
								_tile[y][x].terrain = STON_FLOOR;
								_tile[y][x].pass = true;
							}
							//��¦
							if (y == sy + 1 && (x == sx + 1 || x == ex - 2))
							{
								_tile[y][x].objframeX = 10;
								_tile[y][x].objframeY = 4;
								_tile[y][x].obj = BLANK;
								_tile[y + 1][x].objframeX = 10;
								_tile[y + 1][x].objframeY = 5;
								_tile[y + 1][x].obj = BLANK;
								_tile[y + 2][x].objframeX = 10;
								_tile[y + 2][x].objframeY = 6;
								_tile[y + 2][x].obj = DOOR_WIDTH;
							}
						}
					}

					if (LorR)
					{
						_tile[sy + 3][sx + 1].roomnum = _tile[sy + 3][sx - 1].roomnum;
						_tile[sy + 3][ex - 2].roomnum = _tile[sy + 3][ex].roomnum;
					}
					else
					{
						_tile[sy + 3][sx + 1].roomnum = _tile[sy + 3][sx].roomnum;
						_tile[sy + 3][ex - 2].roomnum = _tile[sy + 3][ex].roomnum;
					}
				}
			}
		}
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
						_tile[y][x].show = true;
						//�� �𼭸�
						if (y == _start.y && x == _start.x)//������
						{
							_tile[y][x].wallX = 11;
							_tile[y][x].wallY = 3;
							_tile[y][x].wall = STON_WALL;
							continue;
						}
						if (y == _start.y && x == _start.x + 21)//��������
						{
							_tile[y][x].wallX = 12;
							_tile[y][x].wallY = 3;
							_tile[y][x].wall = STON_WALL;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x)//���ʾƷ�
						{
							_tile[y][x].wallX = 11;
							_tile[y][x].wallY = 4;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x + 21)//�����ʾƷ�
						{
							_tile[y][x].wallX = 12;
							_tile[y][x].wallY = 4;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						//�纯
						if (y == _start.y)//��
						{
							_tile[y][x].wallX = 8;
							_tile[y][x].wallY = 2;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 23)//�Ʒ�
						{
							_tile[y][x].objframeX = 8;
							_tile[y][x].objframeY = 0;
							_tile[y][x].obj = WALL_TOP;
						}
						else if (y == _start.y + 24)//���Ʒ�
						{
							_tile[y][x].wallX = 5;
							_tile[y][x].wallY = 1;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x && y < _start.y + 23)//����
						{
							_tile[y][x].wallX = 9;
							_tile[y][x].wallY = 1;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x + 21 && y < _start.y + 23)//������
						{
							_tile[y][x].wallX = 7;
							_tile[y][x].wallY = 1;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 1 && (x > _start.x && x < _start.x + 21)) //�� ����
						{
							_tile[y][x].wallX = 4;
							_tile[y][x].wallY = 3;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 2 && (x > _start.x && x < _start.x + 21)) //�Ʒ� ����
						{
							_tile[y][x].wallX = 4;
							_tile[y][x].wallY = 4;
							_tile[y][x].wall = STON_WALL;
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
				DATABASE->spon.x = _start.y + 12;
				DATABASE->spon.y = _start.x + 10;
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
							_tile[y][x].wallX = 11;
							_tile[y][x].wallY = 3;
							_tile[y][x].wall = STON_WALL;
							continue;
						}
						if (y == _start.y && x == _start.x + 21)//��������
						{
							_tile[y][x].wallX = 12;
							_tile[y][x].wallY = 3;
							_tile[y][x].wall = STON_WALL;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x)//���ʾƷ�
						{
							_tile[y][x].wallX = 11;
							_tile[y][x].wallY = 4;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						if (y == _start.y + 23 && x == _start.x + 21)//�����ʾƷ�
						{
							_tile[y][x].wallX = 12;
							_tile[y][x].wallY = 4;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
							continue;
						}
						//�纯
						if (y == _start.y)//��
						{
							_tile[y][x].wallX = 8;
							_tile[y][x].wallY = 2;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 23)//�Ʒ�
						{
							_tile[y][x].objframeX = 8;
							_tile[y][x].objframeY = 0;
							_tile[y][x].obj = WALL_TOP;
						}
						else if (y == _start.y + 24)//���Ʒ�
						{
							_tile[y][x].wallX = 5;
							_tile[y][x].wallY = 1;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x && y < _start.y + 23)//����
						{
							_tile[y][x].wallX = 9;
							_tile[y][x].wallY = 1;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (x == _start.x + 21 && y < _start.y + 23)//������
						{
							_tile[y][x].wallX = 7;
							_tile[y][x].wallY = 1;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 1 && (x > _start.x && x < _start.x + 21)) //�� ����
						{
							_tile[y][x].wallX = 4;
							_tile[y][x].wallY = 3;
							_tile[y][x].wall = STON_WALL;
							_tile[y][x].pass = false;
						}
						else if (y == _start.y + 2 && (x > _start.x && x < _start.x + 21)) //�Ʒ� ����
						{
							_tile[y][x].wallX = 4;
							_tile[y][x].wallY = 4;
							_tile[y][x].wall = STON_WALL;
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
						_tile[sy + 1][sx + 5].obj = DECO;
						_tile[sy + 1][sx + 5].objframeX = 10;
						_tile[sy + 1][sx + 5].objframeY = 0;
						_tile[sy + 2][sx + 5].obj = DECO;
						_tile[sy + 2][sx + 5].objframeX = 10;
						_tile[sy + 2][sx + 5].objframeY = 1;
						_tile[sy + 3][sx + 5].obj = DECO;
						_tile[sy + 3][sx + 5].objframeX = 10;
						_tile[sy + 3][sx + 5].objframeY = 2;
						_tile[sy + 4][sx + 5].obj = DECO;
						_tile[sy + 4][sx + 5].objframeX = 10;
						_tile[sy + 4][sx + 5].objframeY = 3;
						//������ ���
						_tile[sy + 1][sx + 6].obj = DECO;
						_tile[sy + 1][sx + 6].objframeX = 11;
						_tile[sy + 1][sx + 6].objframeY = 0;
						_tile[sy + 2][sx + 6].obj = DECO;
						_tile[sy + 2][sx + 6].objframeX = 11;
						_tile[sy + 2][sx + 6].objframeY = 1;
						_tile[sy + 3][sx + 6].obj = DECO;
						_tile[sy + 3][sx + 6].objframeX = 11;
						_tile[sy + 3][sx + 6].objframeY = 2;
						//������ ������
						_tile[sy + 1][sx + 7].obj = DECO;
						_tile[sy + 1][sx + 7].objframeX = 12;
						_tile[sy + 1][sx + 7].objframeY = 0;
						_tile[sy + 2][sx + 7].obj = DECO;
						_tile[sy + 2][sx + 7].objframeX = 12;
						_tile[sy + 2][sx + 7].objframeY = 1;
						_tile[sy + 3][sx + 7].obj = DECO;
						_tile[sy + 3][sx + 7].objframeX = 12;
						_tile[sy + 3][sx + 7].objframeY = 2;
					}
					//������
					{
						_tile[sy + 4][sx + 1].obj = DECO;
						_tile[sy + 4][sx + 1].objframeX = 5;
						_tile[sy + 4][sx + 1].objframeY = 5;
						_tile[sy + 3][sx + 4].obj = DECO;
						_tile[sy + 3][sx + 4].objframeX = 5;
						_tile[sy + 3][sx + 4].objframeY = 5;
					}
					//����
					{
						_tile[sy + 5][sx + 1].obj = DECO;
						_tile[sy + 5][sx + 1].objframeX = 16;
						_tile[sy + 5][sx + 1].objframeY = 0;
						_tile[sy + 5][sx + 2].obj = DECO;
						_tile[sy + 5][sx + 2].objframeX = 17;
						_tile[sy + 5][sx + 2].objframeY = 0;
						//�������
						_tile[sy + 3][sx + 2].obj = DECO;
						_tile[sy + 3][sx + 2].objframeX = 16;
						_tile[sy + 3][sx + 2].objframeY = 1;
						_tile[sy + 4][sx + 2].obj = DECO;
						_tile[sy + 4][sx + 2].objframeX = 16;
						_tile[sy + 4][sx + 2].objframeY = 2;
						_tile[sy + 3][sx + 3].obj = DECO;
						_tile[sy + 3][sx + 3].objframeX = 17;
						_tile[sy + 3][sx + 3].objframeY = 1;
						_tile[sy + 4][sx + 3].obj = DECO;
						_tile[sy + 4][sx + 3].objframeX = 17;
						_tile[sy + 4][sx + 3].objframeY = 2;
					}
					//�ѹٱ���
					{
						_tile[sy + 2][sx + 8].obj = DECO;
						_tile[sy + 2][sx + 8].objframeX = 18;
						_tile[sy + 2][sx + 8].objframeY = 0;
						_tile[sy + 3][sx + 8].obj = DECO;
						_tile[sy + 3][sx + 8].objframeX = 18;
						_tile[sy + 3][sx + 8].objframeY = 1;
					}
					//����
					{
						_tile[sy + 1][sx + 1].obj = DECO;
						_tile[sy + 1][sx + 1].objframeX = 19;
						_tile[sy + 1][sx + 1].objframeY = 0;
						_tile[sy + 2][sx + 1].obj = DECO;
						_tile[sy + 2][sx + 1].objframeX = 19;
						_tile[sy + 2][sx + 1].objframeY = 1;
					}
					//Ź��
					{
						//����
						for (int oy = 7, y = _start.y + 8; y < _start.y + 11; ++y, ++oy)
						{
							for (int ox = 7, x = _start.x + 1; x < _start.x + 14; ++x, ++ox)
							{
								_tile[y][x].obj = DECO;
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
								_tile[y][x].obj = DECO;
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
		//�� ���
		if (_nowselecct == R_ERASE)
		{
			if (_tile[_start.y][_start.x].roomnum == 0) { return; }
			int sy, sx, ey, ex;
			sy = ey = _start.y;
			sx = ex = _start.x;
			while (_tile[sy][_start.x].roomnum != 0) { --sy; }
			while (_tile[ey][_start.x].roomnum != 0) { ++ey; }
			while (_tile[_start.y][sx].roomnum != 0) { --sx; }
			while (_tile[_start.y][ex].roomnum != 0) { ++ex; }
			for (int y = sy; y < ey; ++y)
			{
				for (int x = sx; x < ex; ++x)
				{
					_tile[y][x].pass = true;
					_tile[y][x].obj = NONE;
					_tile[y][x].terrain = EMPTY;
					_tile[y][x].wall = VOID_WALL;
					_tile[y][x].terrainX = _tile[y][x].objframeX = _tile[y][x].wallX = 5;
					_tile[y][x].terrainY = _tile[y][x].objframeY = _tile[y][x].wallY = 1;
					_tile[y][x].roomnum = _tile[y][x].sponSequence = 0;
				}
			}
			--_makeroom;
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
		_tagbutton[i].rc = RectMake(_cam->getRC().right - _booktag->getFrameWidth() - _book->getWidth() + 10, _cam->getRC().top + y,
			_booktag->getFrameWidth(), _booktag->getFrameHeight());
	}
	//����� ���̽� ��ġ ����
	for (int sy = 94, y = 0; y < 5; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_bigsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy - _currentscrool,
				_bigsamplecase->getFrameWidth(), _bigsamplecase->getFrameHeight());
		}
	}
	//���ʹ� ���� ���̽� ��ġ ����
	for (int sy = 94, y = 0; y < 2; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_enemysample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				_bigsamplecase->getFrameWidth(), _bigsamplecase->getFrameHeight());
		}
	}
	//�� ���� ���̽� ��ġ ����
	for (int sy = 94, y = 0; y < 2; ++y, sy += 240)
	{
		for (int sx = 0, x = 0; x < 2; ++x, sx += 160)
		{
			_roomsample[y][x].rc = RectMake(_cam->getRC().right - 350 + sx, _cam->getRC().top + sy,
				_bigsamplecase->getFrameWidth(), _bigsamplecase->getFrameHeight());
		}
	}
	//���� �ɼ� ��ġ ����
	for (int sy = 94, y = 0; y < 5; ++y, sy += 118)
	{
		_option[y].rc = RectMake(_cam->getRC().right - 350, _cam->getRC().top + sy,
			_mapoption->getFrameWidth(), _mapoption->getFrameHeight());
	}
	//���� ���� ��ġ ����
	_bosssample.rc = RectMake(_cam->getRC().right - 350 + 20, _cam->getRC().top + 94,
		_bosscase->getFrameWidth(), _bosscase->getFrameHeight());
	//��ũ�ѹ� ����
	_scroolpos = (_currentscrool / _scroolmax) * 512;
}

void mapTool::draw(void)
{
	char str[8];
	int sy = _cam->getRC().top / TILESIZE;
	int sx = _cam->getRC().left / TILESIZE;
	int ey = _cam->getRC().bottom / TILESIZE + 1;
	int ex = _cam->getRC().right / TILESIZE + 1;

	if (ey > TILEY) { ey = TILEY; }
	if (ex > TILEX) { ex = TILEX; }

	//�� ���ñ�� �� �ٴ��� �׸��ϴ�.
	for (int y = sy; y < ey; ++y)
	{
		for (int x = sx; x < ex; ++x)
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
	//���� �׷��ݽô� ����
	for (int y = sy; y < ey; ++y)
	{
		for (int x = sx; x < ex; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; }
			if (_tile[y][x].wall == VOID_WALL) { continue; }
			_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].wallX, _tile[y][x].wallY);
		}
	}
	//������Ʈ�� �׸��ϴ�.
	for (int y = sy; y < ey; ++y)
	{
		for (int x = sx; x < ex; ++x)
		{
			sprintf(str, "%d", _tile[y][x].sponSequence);
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_tile[y][x].rc, &_cam->getRC())) { continue; } //Ŭ���� ���� �ȿ� ������ �Ѿ
			if (_tile[y][x].obj == NONE) { continue; } //������Ʈ�� �ȱ�� �־ �Ѿ
			if (_tile[y][x].obj == WOOD_BARREL) { _barrels->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 0, 0); continue; } //�������̸� �׸��� �Ѿ
			else if (_tile[y][x].obj == BOOM_BARREL) { _barrels->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, 1, 0); continue; } //��ź���̸� �׸��� �Ѿ
			else if (_tile[y][x].obj == BULLET_KIN) //�Ѿ��̸� �׸��� �Ѿ
			{
				_enemys->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64, 0, 0);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, str, strlen(str)); continue;
			}
			else if (_tile[y][x].obj == BULLET_BANDANA) //�ݴ޸���Ʈ�� �׸��� �Ѿ
			{
				_enemys->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64, 1, 0);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 32, str, strlen(str)); continue;
			}
			else if (_tile[y][x].obj == SHOTGUN_RED) //�����̸� �׸��� �Ѿ
			{
				_enemys->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64, 0, 1);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 48, str, strlen(str)); continue;
			}
			else if (_tile[y][x].obj == GUN_NUT) //�ǳ��̸� �׸��� �Ѿ
			{
				_enemys->frameRender(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - 64, 1, 1);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - 48, str, strlen(str)); continue;
			}
			else if (_tile[y][x].obj == BULLET_KING) //������ �׸��� �Ѿ
			{
				_boss->render(getBackDC(), _tile[y][x].rc.left - 32, _tile[y][x].rc.top - TILESIZE);
				TextOut(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top - TILESIZE, str, strlen(str)); continue;
			}
			_sample->frameRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top, _tile[y][x].objframeX, _tile[y][x].objframeY);
		}
	}
	//���� �� ���콺�� ������ Ȥ�� ��ġ�� Ÿ��
	int x = _mouse.x / TILESIZE;
	int y = _mouse.y / TILESIZE;
	if (_tile[y][x].obj == NONE)
	{
		_ok->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
	}
	else
	{
		_no->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
	}

	//�巡���� �κ� �׷�����
	selectdraw();
	//�̴ϸ�
	if (_mapOpen) { minidraw(); }

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
					if (_bigsample[y][x].select) { _bigsamplecase->frameRender(getBackDC(), _bigsample[y][x].rc.left, _bigsample[y][x].rc.top, 1, 0); }
					else { _bigsamplecase->frameRender(getBackDC(), _bigsample[y][x].rc.left, _bigsample[y][x].rc.top, 0, 0); }
					_bigsampleimg->frameRender(getBackDC(), _bigsample[y][x].rc.left + 10, _bigsample[y][x].rc.top + 30, x, y);
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
					if (_enemysample[y][x].select) { _bigsamplecase->frameRender(getBackDC(), _enemysample[y][x].rc.left, _enemysample[y][x].rc.top, 1, 0); }
					else { _bigsamplecase->frameRender(getBackDC(), _enemysample[y][x].rc.left, _enemysample[y][x].rc.top, 0, 0); }
					_enemysampleimg->frameRender(getBackDC(), _enemysample[y][x].rc.left + 10, _enemysample[y][x].rc.top + 30, x, y);
				}
			}
		}
		//����
		if (_page == 3)
		{
			if (_bosssample.select) { _bosscase->frameRender(getBackDC(), _bosssample.rc.left, _bosssample.rc.top, 1, 0); }
			else { _bosscase->frameRender(getBackDC(), _bosssample.rc.left, _bosssample.rc.top, 0, 0); }
			_boss->render(getBackDC(), _bosssample.rc.left + 60, _bosssample.rc.top + 40);
		}
		//��
		if (_page == 4)
		{
			for (int y = 0; y < 2; ++y)
			{
				for (int x = 0; x < 2; ++x)
				{
					if (_roomsample[y][x].select) { _bigsamplecase->frameRender(getBackDC(), _roomsample[y][x].rc.left, _roomsample[y][x].rc.top, 1, 0); }
					else { _bigsamplecase->frameRender(getBackDC(), _roomsample[y][x].rc.left, _roomsample[y][x].rc.top, 0, 0); }
					_roomsampleimg->frameRender(getBackDC(), _roomsample[y][x].rc.left + 10, _roomsample[y][x].rc.top + 30, x, y);
				}
			}
		}
		//�ɼ�
		if (_page == 5)
		{
			for (int y = 0; y < 5; ++y)
			{
				if (_option[y].select) { _mapoption->frameRender(getBackDC(), _option[y].rc.left, _option[y].rc.top, 1, y); }
				else { _mapoption->frameRender(getBackDC(), _option[y].rc.left, _option[y].rc.top, 0, y); }
			}
		}

		//���ú� Ŀ��
		_cover->render(getBackDC(), _cam->getRC().right - _book->getWidth(), _cam->getRC().top);
		//��ũ�ѹ� �� �κ�
		_scrollbardown->render(getBackDC(), _cam->getRC().right - 34, _cam->getRC().top + 94);
		_scrollbar->render(getBackDC(), _cam->getRC().right - 34, (_cam->getRC().top + 94) + _scroolpos);
		//�±�
		for (int y = 1, i = 0; i < 6; ++i, y += 2)
		{
			if (_tagbutton[i].select)
			{
				if (_tagbutton[i].onmouse) { _booktag->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 1, y); }
				else { _booktag->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 0, y); }
			}
			else
			{
				if (_tagbutton[i].onmouse) { _booktag->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 1, i * 2); }
				else { _booktag->frameRender(getBackDC(), _tagbutton[i].rc.left, _tagbutton[i].rc.top, 0, i * 2); }
			}
		}
	}
}

void mapTool::minidraw(void)
{
	HBRUSH bluebrush = CreateSolidBrush(RGB(0, 0, 255));
	HBRUSH withebrush = CreateSolidBrush(RGB(255, 255, 255));
	_black->render(_miniimg->getMemDC());
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
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
	mtemp.x = (_cam->getRC().left + _campt.x / (TILESIZE / _TILEMINISIZE)) - _minimappt.x + 100;
	mtemp.y = (_cam->getRC().top + _campt.y / (TILESIZE / _TILEMINISIZE)) - _minimappt.y + 80;
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

void mapTool::selectdraw(void)
{
	if (_nowdraw == SAMIAUTO)
	{
		for (int y = _dragrc.top / TILESIZE; y < _dragrc.bottom / TILESIZE + 1; ++y)
		{
			for (int x = _dragrc.left / TILESIZE; x < _dragrc.right / TILESIZE + 1; ++x)
			{
				if (!IntersectRect(&RectMake(0, 0, 0, 0), &_cam->getRC(), &_tile[y][x].rc)) { continue; }
				if (_tile[y][x].obj != NONE)
				{
					_no->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
					continue;
				}
				if (_tile[y][x].obj == NONE)
				{
					_ok->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
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
				for (int x = _start.x; x < _start.x + 22; ++x)
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
						_no->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
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
						_ok->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
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
						_no->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
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
						_ok->alphaRender(getBackDC(), _tile[y][x].rc.left, _tile[y][x].rc.top);
					}
				}
			}
		}
	}
}