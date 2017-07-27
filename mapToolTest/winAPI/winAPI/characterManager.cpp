#include "stdafx.h"
#include "characterManager.h"

characterManager::characterManager()
{
}

characterManager::~characterManager()
{
}

HRESULT characterManager::init(float x, float y)
{
	gameNode::init();

	_pl = new player;
	_pl->init(x, y);

	_vCharacter.push_back(_pl);

	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (_Tile[y][x].obj == BULLET_KIN || _Tile[y][x].obj == BULLET_BANDANA ||
				_Tile[y][x].obj == SHOTGUN_RED || _Tile[y][x].obj == GUN_NUT || _Tile[y][x].obj == BULLET_KING)
			{
				if (_Tile[y][x].obj == BULLET_KIN)
				{
					_temp = new bulletK;
					bulletK* mon = (bulletK*)_temp;
					mon->init(_Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].obj, _Tile[y][x].roomnum, _Tile[y][x].sponSequence,
						&_pl->getCharacterData().idX, &_pl->getCharacterData().idY, &_pl->_bfire);
					_vCharacter.push_back(mon);
				}
				if (_Tile[y][x].obj == BULLET_BANDANA)
				{
					_temp = new bulletB;
					bulletB* mon = (bulletB*)_temp;
					mon->init(_Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].obj, _Tile[y][x].roomnum, _Tile[y][x].sponSequence,
						&_pl->getCharacterData().idX, &_pl->getCharacterData().idY, &_pl->_bfire);
					_vCharacter.push_back(mon);
				}
				if (_Tile[y][x].obj == SHOTGUN_RED)
				{
					_temp = new shotgunR;
					shotgunR* mon = (shotgunR*)_temp;
					mon->init(_Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].obj, _Tile[y][x].roomnum, _Tile[y][x].sponSequence,
						&_pl->getCharacterData().idX, &_pl->getCharacterData().idY, &_pl->_bfire);
					_vCharacter.push_back(mon);
				}
				if (_Tile[y][x].obj == GUN_NUT)
				{
					_temp = new gunnut;
					gunnut* mon = (gunnut*)_temp;
					mon->init(_Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].obj, _Tile[y][x].roomnum, _Tile[y][x].sponSequence,
						&_pl->getCharacterData().idX, &_pl->getCharacterData().idY, &_pl->_bfire);
					_vCharacter.push_back(mon);
				}
				if (_Tile[y][x].obj == BULLET_KING)
				{
					_temp = new bulletKing;
					bulletKing* mon = (bulletKing*)_temp;
					mon->init(_Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].obj, _Tile[y][x].roomnum, _Tile[y][x].sponSequence,
						&_pl->getCharacterData().idX, &_pl->getCharacterData().idY, &_pl->_bfire);
					_vCharacter.push_back(mon);
				}
			}
		}
	}

	_ebullet = new bulletE;
	_ebullet->init(600, &_pl->_move);

	return S_OK;
}

void characterManager::release(void)
{
	gameNode::release();

	for (int i = 0; i < _vCharacter.size(); ++i)
	{
		_vCharacter[i]->release();
		SAFE_DELETE(_vCharacter[i]);
	}
}

void characterManager::update(void)
{
	gameNode::update();
	
	enemyspawn();

	//이제 업데이트 부분
	for (int i = 0; i < _vCharacter.size(); ++i)
	{
		if ((_vCharacter[i]->getCharacterData().sqence == HOLD && _vCharacter[i]->getCharacterData().obj != NONE) ||
			!_vCharacter[i]->getCharacterData().life) { continue; }
		//업데이트 하고
		_vCharacter[i]->update();
		if (_vCharacter[i]->getCharacterData().obj != NONE)
		{
			if (_vCharacter[i]->getCharacterData().obj == SHOTGUN_RED)
			{
				if (_vCharacter[i]->getCharacterData().fire)
				{
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - PI / 4, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 5, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - PI / 8, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 5, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + PI / 8, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + PI / 4, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_vCharacter[i]->getCharacterData().fire = false;
				}
			}
			else if (_vCharacter[i]->getCharacterData().obj == GUN_NUT)
			{
				if (_vCharacter[i]->getCharacterData().fire)
				{
					//반원 발사
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.8f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.7f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.6f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.5f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 5, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.4f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.3f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.2f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle - 0.1f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.1f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.2f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.3f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 5, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.4f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.5f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.6f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.7f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_ebullet->fire(_vCharacter[i]->getCharacterData().x + 10, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle + 0.8f, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					//화살표 발사는 생각좀해보고
					_vCharacter[i]->getCharacterData().fire = false;
				}
			}
			else
			{
				if (_vCharacter[i]->getCharacterData().fire)
				{
					_ebullet->fire(_vCharacter[i]->getCharacterData().x, _vCharacter[i]->getCharacterData().y,
						_vCharacter[i]->getCharacterData().angle, 3.5f, _vCharacter[i]->getCharacterData().fdirec, 2);
					_vCharacter[i]->getCharacterData().fire = false;
				}
			}
		}
	}
	//콩포탄 조진다
	if (_pl->getBlankshot() && DATABASE->blank)
	{
		DATABASE->blank = false;
		_ebullet->getBullet().clear();
	}
	//플레이어 총쏘기
	if (_pl->getCharacterData().gframeX == 0 && _pl->getCharacterData().gframecount == 1)
	{
		_ebullet->fire(_pl->getCharacterData().x, _pl->getCharacterData().y, _pl->getCharacterData().angle, 10.0f,
			_pl->_direc, 1);
	}

	_ebullet->update();
}

void characterManager::render(HDC hdc)
{
	_ebullet->render(hdc);
	for (int i = 0; i < _vCharacter.size(); ++i)
	{
		_vCharacter[i]->render(hdc);
	}
}

void characterManager::enemyspawn(void)
{
	vector<character*> vtemp;
	bool clear = true;
	bool nextspawn = true;
	int fightroom = DATABASE->nowroom - 1;
	if (fightroom < 0) { return; }
	//몬스터 스폰시켜주고
	for (int i = 0; i < _vCharacter.size(); ++i)
	{
		//현재 싸우고 있는 방에 있는 애가 아니면 컨티뉴
		if (DATABASE->nowroom != _vCharacter[i]->getCharacterData().roomnum) { continue; }
		//지금 방이 클리어 되어있으면 컨티뉴
		if (DATABASE->roomclear[fightroom]) { continue; }
		vtemp.push_back(_vCharacter[i]);

		if (_vCharacter[i]->getCharacterData().obj == BULLET_KIN || _vCharacter[i]->getCharacterData().obj == BULLET_BANDANA ||
			_vCharacter[i]->getCharacterData().obj == SHOTGUN_RED || _vCharacter[i]->getCharacterData().obj == GUN_NUT ||
			_vCharacter[i]->getCharacterData().obj == BULLET_KING)
		{
			//현재 스폰순서가 아니면 컨티뉴
			if (DATABASE->roomsequnce[fightroom] != _vCharacter[i]->getCharacterData().spawnnum) { continue; }
			//스폰하지 않았다면 스폰시켜주자
			if (!_vCharacter[i]->getCharacterData().spawning)
			{
				_vCharacter[i]->getCharacterData().spawning = true;
				_vCharacter[i]->getCharacterData().sqence = SPAWN;
			}
		}
	}
	//방안에있는 몬스터 다 뒤졌는지 확인해 보고 다음애들뽑아야 하는지 체크하자
	for (int i = 0; i < vtemp.size(); ++i)
	{
		if (vtemp[i]->getCharacterData().life) { clear = false; }

		if (vtemp[i]->getCharacterData().spawnnum == DATABASE->roomsequnce[fightroom] && vtemp[i]->getCharacterData().life) { nextspawn = false; }
	}

	if (clear) { DATABASE->roomclear[fightroom] = true; }
	if (nextspawn) { ++DATABASE->roomsequnce[fightroom]; }
}