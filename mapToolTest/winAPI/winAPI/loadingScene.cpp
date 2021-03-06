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
	_loading->loadFrameImage("bosscase", "bosssamplecase.bmp", 480, 210, 2, 1, true, RGB(255, 0, 255));
	_loading->loadImage("bossimg", "boss.bmp", 128, 128, true, RGB(255, 0, 255));
	_loading->loadImage("bosssample", "bosssample.bmp", 220, 150, true, RGB(255, 0, 255));
	_loading->loadFrameImage("mapoption", "mapoption.bmp", 600, 540, 2, 5, true, RGB(255, 0, 255));
	_loading->loadFrameImage("bigsample", "bigsample.bmp", 240, 800, 2, 5, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemysample", "enemysample.bmp", 240, 320, 2, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("roomsample", "roomsample.bmp", 240, 320, 2, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("booktag", "booktag.bmp", 100, 360, 2, 12, true, RGB(255, 0, 255));
	_loading->loadImage("scrollbardown", "scrollbarplace.bmp", 14, 580, true, RGB(255, 0, 255));
	_loading->loadImage("scrollbar", "scrollbar.bmp", 14, 68, true, RGB(255, 0, 255));
	_loading->loadFrameImage("barrels", "barrels.bmp", 128, 96, 2, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("enemys", "enemys.bmp", 192, 256, 2, 2, true, RGB(255, 0, 255));

	//인게임에서 사용할 캐릭터들과 오브젝트 및 총기 이미지 등등
	_loading->loadFrameImage("marin", "Player.bmp", 1008, 640, 16, 8, true, RGB(255, 0, 255));
	_loading->loadFrameImage("kin", "bullet_kin.bmp", 301, 256, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("bandana", "bullet_bandana.bmp", 301, 256, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("king", "bullet_king.bmp", 2178, 924, 11, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("red", "shotgun_red.bmp", 301, 344, 7, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("gunnut", "gunnut.bmp", 3024, 3528, 12, 12, true, RGB(255, 0, 255));
	_loading->loadFrameImage("mPistol", "m_pistol.bmp", 128, 40, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("kPistol", "k_pistol.bmp", 200, 64, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("bPistol", "b_pistol.bmp", 102, 64, 3, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("rShotgun", "shotgun.bmp", 630, 72, 7, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("wTable", "w_table.bmp", 1992, 612, 12, 4, true, RGB(255, 0, 255));
	_loading->loadFrameImage("hTable", "h_table.bmp", 1944, 820, 12, 4, true , RGB(255,0,255));
	_loading->loadFrameImage("wDoor", "w_door.bmp", 432, 384, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("hDoor", "h_door.bmp", 512, 318, 4, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("sheel", "sheel.bmp", 56, 40, 2, 2, true, RGB(255, 0, 255));
	_loading->loadFrameImage("ui", "ui.bmp", 336, 42, 7, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("telpo", "telepoter.bmp", 640, 896, 5, 7, true, RGB(255, 0, 255));
	_loading->loadFrameImage("spawn", "spawn.bmp", 2115, 246, 15, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("blankfx", "blankshot.bmp", 4620, 420, 11, 1, true, RGB(255, 0, 255));
	_loading->loadFrameImage("bossbattle", "bossbattle.bmp", 6144, 768, 6, 1, true, RGB(255, 0, 255));
	_loading->loadImage("pBullet", "playerbullet.bmp", 18, 18, true, RGB(255,0,255));
	_loading->loadImage("eBullet", "enemybullet.bmp", 36, 36, true, RGB(255, 0, 255));
	_loading->loadImage("weapon", "weapon.bmp", 172, 152, true, RGB(255, 0, 255));
	_loading->loadImage("reloading", "reloading.bmp", 96, 21, true, RGB(255, 0, 255));
	_loading->loadImage("reloadingbar", "reloadingbar.bmp", 3, 21, true, RGB(255, 0, 255));
	_loading->loadImage("icon", "icons.bmp", 16, 16, true, RGB(255, 0, 255));

	_loading->loadSound("gunfire", "pistolshoot.mp3", false, false);
	_loading->loadSound("gunreload", "pistolreload.mp3", false, false);
	_loading->loadSound("turntable", "tablepush.mp3", false, false);
	_loading->loadSound("dodge", "dodge.mp3", false, false);
	_loading->loadSound("sumon", "sumon.mp3", false, false);
	_loading->loadSound("blankshoot", "blankshoot.mp3", false, false);
	_loading->loadSound("telactivated", "telactivated.mp3", false, false);
	_loading->loadSound("teleport", "teleport.mp3", false, false);
	_loading->loadSound("edie", "edie.mp3", false, false);
	_loading->loadSound("nutfire", "nutfire.mp3", false, false);
	_loading->loadSound("dooropen", "dooropen.mp3", false, false);
	_loading->loadSound("circleshot", "circleshot.mp3", false, false);
	_loading->loadSound("turnshot", "turnshot.mp3", false, false);


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