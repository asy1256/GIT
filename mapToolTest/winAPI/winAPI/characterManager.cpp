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
				_Tile[y][x].obj == SHOTGUN_RED /*|| _Tile[y][x].obj == GUN_NUT*/)
			{
				if (_Tile[y][x].obj == BULLET_KIN)
				{
					_temp = new bulletK;
					bulletK* mon = (bulletK*)_temp;
					mon->init(_Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].obj, _Tile[y][x].roomnum);
					_vCharacter.push_back(mon);
				}
				if (_Tile[y][x].obj == BULLET_BANDANA) { _temp = new bulletB; }
				if (_Tile[y][x].obj == SHOTGUN_RED) { _temp = new shotgunR; }
				/*if (_Tile[y][x].obj == GUN_NUT) {}*/
			}
		}
	}

	_ebullet = new bulletE;
	_ebullet->init("ebullet", 600);

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
	
	for (int i = 0; i < _vCharacter.size(); ++i)
	{
		if (_vCharacter[i]->getCharacterData().obj == BULLET_KIN)
		{
			bulletK* mon = (bulletK*)_vCharacter[i];
			mon->update(_pl->getCharacterData().idX, _pl->getCharacterData().idY);
		}
		else { _vCharacter[i]->update(); }
	}

	_ebullet->update();
}

void characterManager::render(HDC hdc)
{
	for (int i = 0; i < _vCharacter.size(); ++i)
	{
		_vCharacter[i]->render(hdc);
	}
}