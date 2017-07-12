#include "stdafx.h"
#include "loadingScene.h"

loadingScene::loadingScene()
{
}

loadingScene::~loadingScene()
{
}

HRESULT loadingScene::init(void)
{
	gameNode::init();

	_loading = new loading;
	_loading->init();



	return S_OK;
}

void loadingScene::release(void)
{
	gameNode::release();

	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	gameNode::update();

	_loading->update();

	if (!_loading->loadNext())
	{
		SCENEMANAGER->changeScene("titleScene");
	}
}

void loadingScene::render(void)
{
	_loading->render();
}