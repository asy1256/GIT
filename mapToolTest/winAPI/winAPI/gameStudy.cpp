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

	//숙제
	//NS-Tower 구현하기

	IMAGEMANAGER->addImage("mapImage", "backImage.bmp", WINSIZEX, WINSIZEY);

	//씬 생성
	SCENEMANAGER->addScene("mapTool", new mapTool);
	
	//현재 씬을 _sceneOne으로 설정
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