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

	_cm->release();
	SAFE_DELETE(_cm);

	_om->release();
	SAFE_DELETE(_om);

	_cam->release();
	SAFE_DELETE(_cam);
}

void gungeonScene::update(void)
{
	gameNode::update();
	SOUNDMANAGER->update();
	mousemove();
	keycontrol();
	frameup();
	if (!DATABASE->bossincount)
	{
		_cm->update();
		_om->update();
	}
	objectColision();
	_cam->update();
	if (_mapOpen) { iconup(); }

	for (int y = _cam->getRC().bottom - 30, i = 0; i < 12; ++i, y -= _sheel->getFrameHeight())
	{
		_sheelrc[i] = RectMakeCenter(_cam->getRC().right - _sheel->getFrameWidth() / 2, y, _sheel->getFrameWidth(), _sheel->getFrameHeight());
	}
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
		HANDLE file, file2, file3;
		DWORD read;

		file = CreateFile("stageOne", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		ReadFile(file, _Tile, sizeof(tagTile) * TILEX * TILEY, &read, NULL);

		file2 = CreateFile("spon", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		ReadFile(file2, &DATABASE->spon, sizeof(POINT), &read, NULL);

		file3 = CreateFile("roomscale", GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		ReadFile(file3, &DATABASE->roomcount, sizeof(int), &read, NULL);

		CloseHandle(file);
		CloseHandle(file2);
		CloseHandle(file3);
	}
	//변수 초기화
	{
		_campt.x = DATABASE->spon.x * TILESIZE;
		_campt.y = DATABASE->spon.y * TILESIZE;

		_ptadd.x = _campt.x - WINSIZEX / 2;
		if (_campt.y > WINSIZEY / 2) { _ptadd.y = _campt.y - WINSIZEY / 2; }
		else { _ptadd.y = 0; }

		_basept = _minimappt = PointMake(0, 0);

		_framecount = _frameX = 0;

		_mapOpen = false;

		_cimg = IMAGEMANAGER->findImage("bimg");
		_field = IMAGEMANAGER->findImage("field");
		_miniimg = IMAGEMANAGER->findImage("mimg");
		_sample = IMAGEMANAGER->findImage("tile");
		_black = IMAGEMANAGER->findImage("black");
		_minimapcase = IMAGEMANAGER->findImage("minimapcase");
		_weaponcase = IMAGEMANAGER->findImage("weapon");
		_sheel = IMAGEMANAGER->findImage("sheel");
		_ui = IMAGEMANAGER->findImage("ui");
		_telepot = IMAGEMANAGER->findImage("telpo");
		_icon = IMAGEMANAGER->findImage("icon");
		_incount = IMAGEMANAGER->findImage("bossbattle");
		_blankFx = IMAGEMANAGER->findImage("blankfx");
	}
	//첫번째 통로 개방 및 기타 전투를 위한 준비
	{
		for (int y = 0; y < TILEY; ++y)
		{
			for (int x = 0; x < TILEX; ++x)
			{
				if (_Tile[y][x].roomnum != 1) { continue; }
				_Tile[y][x].show = true;
			}
		}

		bool rmc = false;
		bool rmf = false;
		int sn = 1;
		for (int i = 0; i < DATABASE->roomcount; ++i)
		{
			if (i == 0) { rmc = true; }
			else { rmc = false; }
			rmf = false;
			sn = 1;
			DATABASE->roomclear.push_back(rmc);
			DATABASE->roomfight.push_back(rmf);
			DATABASE->roomsequnce.push_back(sn);
		}
	}

	_om = new objectManager;
	_om->init();

	_cm = new characterManager;
	_cm->init(_campt.x, _campt.y);

	_cm->getPlayer()->setObject(_om);

	_cam = new camera;
	_cam->init(&_campt, _cm->getPlayer(), _cimg, true);

	_cm->getBullet()->setCharacterManager(_cm);
	_cm->getBullet()->setObjectManager(_om);
	_cm->getBullet()->setPlayer(_cm->getPlayer());

	telepoter* tp;

	//물질이동기 생성
	for (int i = 0; i < _om->getObjectvector().size(); ++i)
	{
		if (_om->getObjectvector()[i]->getObjectData().type == TELEPOTER)
		{
			tp = (telepoter*)_om->getObjectvector()[i];
			tagTel tempTel;
			tempTel.x = tp->getObjectData().x;
			tempTel.y = tp->getObjectData().y;
			tempTel.rc = RectMake(tempTel.x, tempTel.y, _telepot->getFrameWidth(), _telepot->getFrameHeight());
			tempTel.frameX = &tp->getObjectData().frameX;
			tempTel.frameY = &tp->getObjectData().frameY;
			tempTel.turnon = &tp->getTpData().turnon;
			tempTel.activated = &tp->getTpData().activated;
			RECT temprc = RectMake((_cam->getRC().left + 100) + ((tempTel.x / TILESIZE) * _TILEMINISIZE),
				(_cam->getRC().top + 80) + ((tempTel.y / TILESIZE) * _TILEMINISIZE),
				_icon->getWidth(), _icon->getHeight());
			_telicon.push_back(temprc);
			_telimg.push_back(tempTel);
		}
	}

	//현재총 렉트 그리자
	for (int y = _cam->getRC().bottom - 30, i = 0; i < 12; ++i, y -= _sheel->getFrameHeight())
	{
		_sheelrc[i] = RectMakeCenter(_cam->getRC().right - _sheel->getFrameWidth() / 2, y, _sheel->getFrameWidth(), _sheel->getFrameHeight());
	}

	//바닥
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (_Tile[y][x].terrain == EMPTY)
			{
				_sample->frameRender(_field->getMemDC(), _Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].terrainX, _Tile[y][x].terrainY);
			}
			else
			{
				_sample->frameRender(_field->getMemDC(), _Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].terrainX, _Tile[y][x].terrainY);
			}
		}
	}
	//벽
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (_Tile[y][x].wall == VOID_WALL) { continue; }
			_sample->frameRender(_field->getMemDC(), _Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].wallX, _Tile[y][x].wallY);
		}
	}
	//데코
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (_Tile[y][x].obj == NONE) { continue; }
			if (_Tile[y][x].obj == DECO || _Tile[y][x].obj == SHOP_MASTER)
			{
				_sample->frameRender(_field->getMemDC(), _Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].objframeX, _Tile[y][x].objframeY);
			}
		}
	}
}

void gungeonScene::keycontrol(void)
{
	//미니맵
	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		_mapOpen = (_mapOpen == false) ? true : false;
		_minimapdraw = true;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//텔레포터 타기
		for (int i = 0; i < _telicon.size(); ++i)
		{
			POINT tempPt = _mouse;
			int wid = (_cm->getPlayer()->getCharacterData().x - _mouse.x) / 2;
			int hei = (_cm->getPlayer()->getCharacterData().y - _mouse.y) / 2;
			tempPt.x -= wid;
			tempPt.y -= hei;
			if (PtInRect(&_telicon[i], tempPt))
			{
				SOUNDMANAGER->play("teleport");
				DATABASE->past.x = _cm->getPlayer()->getCharacterData().x;
				DATABASE->past.y = _cm->getPlayer()->getCharacterData().y;
				DATABASE->moveTo.x = _telimg[i].x + TILESIZE;
				DATABASE->moveTo.y = _telimg[i].y + TILESIZE;
				*_telimg[i].frameY = 5;
				*_telimg[i].activated = true;
				_cm->getPlayer()->getPlayerData().teleporting = true;
			}
		}
	}
}

void gungeonScene::frameup(void)
{
	if (DATABASE->bang)
	{
		++_framecount;
		if (_framecount >= 7)
		{
			_framecount = 0;
			++_frameX;
			if (_frameX >= 11)
			{
				_frameX = 0;
				DATABASE->bang = false;
			}
		}
	}
	if (DATABASE->bossincount)
	{
		++_framecount;
		if (_framecount >= 25)
		{
			_framecount = 0;
			++_frameX;
			if (_frameX >= 6)
			{
				_frameX = 0;
				DATABASE->bossincount = false;
			}
		}
	}
}

void gungeonScene::iconup(void)
{
	for (int i = 0; i < _telicon.size(); ++i)
	{
		_telicon[i] = RectMake((_cam->getRC().left + 100) + ((_telimg[i].x / TILESIZE) * _TILEMINISIZE),
			(_cam->getRC().top + 80) + ((_telimg[i].y / TILESIZE) * _TILEMINISIZE),
			_icon->getWidth(), _icon->getHeight());
	}
}

void gungeonScene::mousemove(void)
{
	_mouse = _ptMouse;
	POINT temp = _mouse;

	_mouse.x = temp.x + _ptadd.x;
	_mouse.y = temp.y + _ptadd.y;
}

void gungeonScene::draw(void)
{
	char str[64];
	int sy = _cam->getRC().top / TILESIZE;
	int sx = _cam->getRC().left / TILESIZE;
	int ey = _cam->getRC().bottom / TILESIZE + 1;
	int ex = _cam->getRC().right / TILESIZE + 1;
	if (ey > TILEY) { ey = TILEY; }
	if (ex > TILEX) { ex = TILEX; }

	//기본 배경
	_field->render(getBackDC(), _cam->getRC().left, _cam->getRC().top, _cam->getRC().left, _cam->getRC().top, WINSIZEX, WINSIZEY);
	//물질이동기
	for (int i = 0; i < _telimg.size(); ++i)
	{
		if (!IntersectRect(&RectMake(0, 0, 0, 0), &_telimg[i].rc, &_cam->getRC())) { continue; }
		_telepot->frameRender(getBackDC(), _telimg[i].x, _telimg[i].y, *_telimg[i].frameX, *_telimg[i].frameY);
	}
	//공포탄 발싸!
	if (DATABASE->bang)
	{
		RECT temp = RectMakeCenter(_cm->getPlayer()->getCharacterData().x, _cm->getPlayer()->getCharacterData().y, _blankFx->getFrameWidth(), _blankFx->getFrameHeight());
		_blankFx->frameRender(getBackDC(), temp.left, temp.top, _frameX, 0);
	}

	Z_order();
	//총알 발싸!
	_cm->getBullet()->render(getBackDC());

	//벽대가리
	for (int y = sy; y < ey; ++y)
	{
		for (int x = sx; x < ex; ++x)
		{
			if (!IntersectRect(&RectMake(0, 0, 0, 0), &_Tile[y][x].rc, &_cam->getRC())) { continue; }
			if (_Tile[y][x].obj == WALL_TOP)
			{
				_sample->frameRender(getBackDC(), _Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].objframeX, _Tile[y][x].objframeY);
			}
		}
	}

	//화면에 뿌려줄 UI 관련 부분
	_weaponcase->render(getBackDC(), _cam->getRC().right - _sheel->getFrameWidth() - _weaponcase->getWidth(),
		_cam->getRC().bottom - _sheel->getFrameHeight() / 2 - _weaponcase->getHeight());

	_cm->getPlayer()->getCharacterData().hp;
	
	for (int i = 0; i < 12; ++i)
	{
		if (i == 0) { _sheel->frameRender(getBackDC(), _sheelrc[i].left, _sheelrc[i].top, 1, 1); }
		else if (i == 11) { _sheel->frameRender(getBackDC(), _sheelrc[i].left, _sheelrc[i].top, 0, 1); }
		else
		{
			if (i <= _cm->getPlayer()->getammo()) { _sheel->frameRender(getBackDC(), _sheelrc[i].left, _sheelrc[i].top, 1, 0); }
			else { _sheel->frameRender(getBackDC(), _sheelrc[i].left, _sheelrc[i].top, 0, 0); }
		}
	}
	//체력 과 장갑
	{
		if (_cm->getPlayer()->getCharacterData().hp == 6)
		{
			for (int x = 0, i = 0; i < 3; ++i, x += _ui->getFrameWidth())
			{
				_ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 0, 0);
			}
		}
		else if (_cm->getPlayer()->getCharacterData().hp == 5)
		{
			for (int x = 0, i = 0; i < 3; ++i, x += _ui->getFrameWidth())
			{
				if (i == 2) { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 1, 0); }
				else { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 0, 0); }
			}
		}
		else if (_cm->getPlayer()->getCharacterData().hp == 4)
		{
			for (int x = 0, i = 0; i < 3; ++i, x += _ui->getFrameWidth())
			{
				if (i == 2) { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 2, 0); }
				else { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 0, 0); }
			}
		}
		else if (_cm->getPlayer()->getCharacterData().hp == 3)
		{
			for (int x = 0, i = 0; i < 3; ++i, x += _ui->getFrameWidth())
			{
				if (i == 2) { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 2, 0); }
				else if (i == 1) { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 1, 0); }
				else { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 0, 0); }
			}
		}
		else if (_cm->getPlayer()->getCharacterData().hp == 2)
		{
			for (int x = 0, i = 0; i < 3; ++i, x += _ui->getFrameWidth())
			{
				if (i == 0) { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 0, 0); }
				else { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 2, 0); }
			}
		}
		else if (_cm->getPlayer()->getCharacterData().hp == 1)
		{
			for (int x = 0, i = 0; i < 3; ++i, x += _ui->getFrameWidth())
			{
				if (i == 0) { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 1, 0); }
				else { _ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 2, 0); }
			}
		}
		else
		{
			for (int x = 0, i = 0; i < 3; ++i, x += _ui->getFrameWidth())
			{
				_ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top, 2, 0);
			}
		}
	}
	//공포탄
	for (int x = 0, i = 0; i < _cm->getPlayer()->getPlayerData().blankshot; ++i, x += _ui->getFrameWidth())
	{
		_ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top + _ui->getFrameHeight(), 4, 0);
	}
	//열쇠랑 돈
	for (int x = 0, fx = 5, i = 0; i < 2; ++i, ++fx, x += _ui->getFrameWidth() + _ui->getFrameWidth() / 2)
	{
		_ui->frameRender(getBackDC(), _cam->getRC().left + x, _cam->getRC().top + _ui->getFrameHeight() * 2, fx, 0);
		if (i == 0)
		{
			sprintf(str, "%d", _cm->getPlayer()->getPlayerData().key);
			TextOut(getBackDC(), _cam->getRC().left + x + _ui->getFrameWidth(), _cam->getRC().top + _ui->getFrameHeight() * 2, str, strlen(str));
		}
		else
		{
			sprintf(str, "%d", _cm->getPlayer()->getPlayerData().money);
			TextOut(getBackDC(), _cam->getRC().left + x + _ui->getFrameWidth(), _cam->getRC().top + _ui->getFrameHeight() * 2, str, strlen(str));
		}
	}

	//테스트
	/*sprintf(str, "X좌표 : %f Y좌표 : %f", _cm->getPlayer()->getCharacterData().x, _cm->getPlayer()->getCharacterData().y);
	TextOut(getBackDC(), _cam->getRC().right - 250, _cam->getRC().top, str, strlen(str));
	sprintf(str, "각도 디그리값 : %d", RADIAN_TO_DEGREE(_cm->getPlayer()->getCharacterData().angle));
	TextOut(getBackDC(), _cam->getRC().right - 250, _cam->getRC().top + 20, str, strlen(str));
	sprintf(str, "mX좌표 : %d mY좌표 : %d", (int)_mouse.x, (int)_mouse.y);
	TextOut(getBackDC(), _cam->getRC().right - 250, _cam->getRC().top + 40, str, strlen(str));*/

	//미니맵
	if (_mapOpen)
	{
		minimap();
		for (int i = 0; i < _telicon.size(); ++i)
		{
			if (*_telimg[i].turnon)
			{
				_icon->render(getBackDC(), _telicon[i].left, _telicon[i].top);
				/*sprintf(str, "iX좌표 : %d iY좌표 : %d", _telicon[i].left, _telicon[i].top);
				TextOut(getBackDC(), _cam->getRC().right - 250, _cam->getRC().top + 60, str, strlen(str));*/
			}
		}
	} 
	if (DATABASE->bossincount) { _incount->frameRender(getBackDC(), _cam->getRC().left, _cam->getRC().top, _frameX, 0); }
}

void gungeonScene::Z_order(void)
{
	vector<gameNode*> _vOutput;

	for (int i = 0; i < _om->getObjectvector().size(); ++i)
	{
		_vOutput.push_back(_om->getObjectvector()[i]);
	}
	for (int i = 0; i < _cm->getCharacterVector().size(); ++i)
	{
		_vOutput.push_back(_cm->getCharacterVector()[i]);
	}

	sort(_vOutput.begin(), _vOutput.end(), compare());

	RECT temp = RectMake(0, 0, 0, 0);
	for (int i = 0; i < _vOutput.size(); ++i)
	{
		if (!IntersectRect(&temp, &_vOutput[i]->getdRC(), &_cam->getRC())) { continue; }
		_vOutput[i]->render(getBackDC());
	}
	_vOutput.clear();
}

void gungeonScene::minimap(void)
{
	if (_minimapdraw)
	{
		HBRUSH bluebrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH withebrush = CreateSolidBrush(RGB(255, 255, 255));

		_black->render(_miniimg->getMemDC());

		for (int y = 0; y < TILEY; ++y)
		{
			for (int x = 0; x < TILEX; ++x)
			{
				if (!_Tile[y][x].show) { continue; }
				if (_Tile[y][x].wall != VOID_WALL)
				{
					FillRect(_miniimg->getMemDC(), &_Tile[y][x].minirc, withebrush);
				}
				if (_Tile[y][x].terrain != EMPTY)
				{
					FillRect(_miniimg->getMemDC(), &_Tile[y][x].minirc, bluebrush);
				}
			}
		}

		DeleteObject(bluebrush);
		DeleteObject(withebrush);
		_minimapdraw = false;
	}
	
	RECT mmaprc = RectMake(_cam->getRC().left + 100, _cam->getRC().top + 80, 800, 600);

	POINT mtemp;
	mtemp.x = (_cam->getRC().left + _cm->getPlayer()->getCharacterData().x / (TILESIZE / _TILEMINISIZE)) - _minimappt.x + 100;
	mtemp.y = (_cam->getRC().top + _cm->getPlayer()->getCharacterData().y / (TILESIZE / _TILEMINISIZE)) - _minimappt.y + 80;
	RECT ftemp = RectMakeCenter(mtemp.x, mtemp.y, _TILEMINISIZE * 16, _TILEMINISIZE * 12);

	_minimapcase->alphaRender(getBackDC(), mmaprc.left - 17, mmaprc.top - 60);
	_miniimg->alphaRender(getBackDC(), mmaprc.left, mmaprc.top, _minimappt.x, _minimappt.y, 800, 600);
	LineMake(getBackDC(), ftemp.left, ftemp.top, ftemp.right, ftemp.top);
	LineMake(getBackDC(), ftemp.right, ftemp.top, ftemp.right, ftemp.bottom);
	LineMake(getBackDC(), ftemp.right, ftemp.bottom, ftemp.left, ftemp.bottom);
	LineMake(getBackDC(), ftemp.left, ftemp.bottom, ftemp.left, ftemp.top);
}