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

	IMAGEMANAGER->addImage("bimg", "backImage.bmp", TILEWIDTH, TILEHEIGHT);
	IMAGEMANAGER->addImage("field", "backImage.bmp", TILEWIDTH, TILEHEIGHT);
	_loading->loadImage("title", "titleimg.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	_loading->loadFrameImage("tile", "sampletile.bmp", 1280, 2560, 20, 40, true, RGB(255, 0, 255));
	_loading->loadImage("mimg", "backImage.bmp", 8 * TILEX, 8 * TILEY, true, RGB(255, 0, 255));
	_loading->loadImage("book", "samplebook.bmp", 400, 768, true, RGB(255, 0, 255));
	_loading->loadImage("minimapcase", "minimapcase.bmp", 838, 734, true, RGB(255, 0, 255));
	_loading->loadImage("black", "blackImage.bmp", 8 * TILEX, 8 * TILEY, true, RGB(255, 0, 255));
	_loading->loadImage("ok", "tileok.bmp", 64, 64, true, RGB(255, 0, 255));
	_loading->loadImage("no", "tileno.bmp", 64, 64, true, RGB(255, 0, 255));
	_loading->loadImage("cover", "bookcover.bmp", 400, 768, true, RGB(255, 0, 255));
	_loading->loadFrameImage("bigsamplecase", "bigsamplecase.bmp", 280, 220, 2, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("mapoption", "mapoption.bmp", 600, 540, 2, 5, true, RGB(255, 0, 255));
	_loading->loadFrameImage("bigsample", "bigsample.bmp", 240, 800, 2, 5, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemysample", "enemysample.bmp", 240, 320, 2, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("roomsample", "roomsample.bmp", 240, 320, 2, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("booktag", "booktag.bmp", 100, 360, 2, 12, true, RGB(255, 0, 255));
	_loading->loadImage("scrollbardown", "scrollbarplace.bmp", 14, 580, true, RGB(255, 0, 255));
	_loading->loadImage("scrollbar", "scrollbar.bmp", 14, 68, true, RGB(255, 0, 255));
	_loading->loadFrameImage("barrels", "barrels.bmp", 128, 96, 2, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemys", "enemys.bmp", 192, 256, 2, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("Player", "Player.bmp", 1008, 640, 16, 8, true, RGB(255, 0, 255));

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

		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			SCENEMANAGER->changeScene("titleScene");
		}
		SCENEMANAGER->changeScene("titleScene");
	}
}

void loadingScene::render(void)
{
	_loading->render();
}