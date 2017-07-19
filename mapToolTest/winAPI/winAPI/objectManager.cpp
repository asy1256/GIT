#include "stdafx.h"
#include "objectManager.h"

objectManager::objectManager()
{
}

objectManager::~objectManager()
{
}

HRESULT objectManager::init(void)
{
	gameNode::init();
	setup();

	return S_OK;
}

void objectManager::release(void)
{
	gameNode::release();

	for (int i = 0; i < _vObject.size(); ++i)
	{
		SAFE_DELETE(_vObject[i]);
	}
}

void objectManager::update(void)
{
	gameNode::update();

}

void objectManager::render(HDC hdc)
{
	for (int i = 0; i < _vObject.size(); ++i)
	{
		_vObject[i]->render(hdc);
	}
}

void objectManager::setup(void)
{
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (_Tile[y][x].obj == DOOR_WIDTH || _Tile[y][x].obj == DOOR_LENGTH)
			{
				_temp = new door;
				_temp->init(_Tile[y][x].rc.left, _Tile[y][x].rc.top, _Tile[y][x].obj, _Tile[y][x].roomnum);
				_vObject.push_back(_temp);
			}
		}
	}
}