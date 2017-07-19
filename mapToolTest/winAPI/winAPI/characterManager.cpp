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

	_ebullet = new bulletE;
	_ebullet->init("ebullet", 600);

	_pl = new player;
	_pl->init(x, y);

	_anything.push_back(_pl);

	return S_OK;
}

void characterManager::release(void)
{
	gameNode::release();
	_pl->release();
	SAFE_DELETE(_pl);
}

void characterManager::update(void)
{
	gameNode::update();
	_pl->update();

	_ebullet->update();

	vector<gameNode*>::iterator iter;
	iter = _anything.begin();
}

void characterManager::render(HDC hdc)
{
	_pl->render(hdc);
}