#include "StdAfx.h"
#include "gameStudy.h"


gameStudy::gameStudy(void)
{
}

gameStudy::~gameStudy(void)
{
}

HRESULT gameStudy::init(void)
{
	gameNode::init(true);

	IMAGEMANAGER->addImage("mapImage", "backImage.bmp", WINSIZEX, WINSIZEY);

	//�� ����
	SCENEMANAGER->addScene("mapTool", new mapTool);
	SCENEMANAGER->addScene("loadingScene", new loadingScene);
	SCENEMANAGER->addScene("titleScene", new titleScene);
	SCENEMANAGER->addScene("gungeonScene", new gungeonScene);
	
	//���� ���� _sceneOne���� ����
	SCENEMANAGER->changeScene("mapTool");
	
	return S_OK;
}

void gameStudy::release(void)
{
	gameNode::release();
}

void gameStudy::update(void)
{
	gameNode::update();
	SCENEMANAGER->update();
}

void gameStudy::render(void)
{
	IMAGEMANAGER->findImage("mapImage")->render(getMemDC());
	SCENEMANAGER->render();
	TIMEMANAGER->render(getMemDC());
	this->getBackBuffer()->render(getHDC(), 0, 0);
}