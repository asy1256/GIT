#include "stdafx.h"
#include "titleScene.h"

titleScene::titleScene()
{
}

titleScene::~titleScene()
{
}

HRESULT titleScene::init(void)
{
	gameNode::init();

	return S_OK;
}

void titleScene::release(void)
{
	gameNode::release();
}

void titleScene::update(void)
{
	gameNode::update();
}

void titleScene::render(void)
{

}