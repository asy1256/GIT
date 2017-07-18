#include "StdAfx.h"
#include "loading.h"

///////////////////// loadItem ///////////////////////

loadItem::loadItem(void)
{
}

loadItem::~loadItem(void)
{
}

HRESULT loadItem::initForImage(string keyName, int width, int height)
{
	_kind = LOADING_KIND_ADDIMAGE_00;

	ZeroMemory(&_imageResource, sizeof(tagImageResource));

	_imageResource.keyName = keyName;
	_imageResource.width = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::initForImage(string keyName, const char* fileName, 
	int width, int height, bool trans, COLORREF transColor)
{
	_kind = LOADING_KIND_ADDIMAGE_01;

	ZeroMemory(&_imageResource, sizeof(tagImageResource));

	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = trans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::initForImage(string keyName, const char* fileName, 
	float x, float y, int width, int height, bool trans, COLORREF transColor)
{
	_kind = LOADING_KIND_ADDIMAGE_02;

	ZeroMemory(&_imageResource, sizeof(tagImageResource));

	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = trans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::initForFrameImage(string keyName, const char* fileName, 
	int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	_kind = LOADING_KIND_ADDFRAMEIMAGE_00;

	ZeroMemory(&_imageResource, sizeof(tagImageResource));

	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = trans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::initForFrameImage(string keyName, const char* fileName, 
	float x, float y, int width, int height, int frameX, int frameY, 
	bool trans, COLORREF transColor)
{
	_kind = LOADING_KIND_ADDFRAMEIMAGE_01;

	ZeroMemory(&_imageResource, sizeof(tagImageResource));

	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = trans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::initForSound(string keyName, string soundName, bool background, bool loop)
{
	_kind = LOADING_KIND_ADDSOUND;

	ZeroMemory(&_soundResource, sizeof(tagsoundResource));

	_soundResource.keyName = keyName;
	_soundResource.soundName = soundName;
	_soundResource.background = background;
	_soundResource.loop = loop;

	return S_OK;
}

void loadItem::release(void)
{
}

///////////////////// loadItem ///////////////////////

///////////////////// loading ///////////////////////

loading::loading(void)
	:_current(0)
{
}

loading::~loading(void)
{
}

HRESULT loading::init(void)
{
	gameNode::init();

<<<<<<< HEAD
	_background = IMAGEMANAGER->addImage("background", "loadimg.bmp", WINSIZEX, WINSIZEY);
=======
	_background = IMAGEMANAGER->addImage("loadimg", "loadimg.bmp", WINSIZEX, WINSIZEY);
>>>>>>> 435d19df0aabdf37fbd42fb6d81ff6b5a9aece44
	IMAGEMANAGER->addImage("loadingBarUp", "loadingBarUp.bmp", 500, 10);
	IMAGEMANAGER->addImage("loadingBarDown", "loadingBarDown.bmp", 500, 10);

	_loadingBar = new progressBar;
	_loadingBar->init("loadingBarUp", "loadingBarDown", 256, 580);
	_loadingBar->setGuage(0, 0);

	return S_OK;
}

void loading::release(void)
{
	gameNode::release();

	_loadingBar->release();
	SAFE_DELETE(_loadingBar);

	for (int i = 0; i < _vLoadItem.size(); i++)
	{
		_vLoadItem[i]->release();
		SAFE_DELETE(_vLoadItem[i]);
	}
}

void loading::update(void)
{
	gameNode::update();
	_loadingBar->update();
}

void loading::render(void)
{
	_background->render(getMemDC());

	_loadingBar->render();
}

void loading::loadImage(string keyName, int width, int height)
{
	loadItem* item = new loadItem;
	item->initForImage(keyName, width, height);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string keyName, const char* fileName, 
	int width, int height, bool trans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->initForImage(keyName, fileName, width, height, trans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string keyName, const char* fileName, 
	float x, float y, int width, int height, bool trans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->initForImage(keyName, fileName, x, y, width, height, trans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string keyName, const char* fileName, 
	int width, int height, int frameX, int frameY, bool trans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->initForFrameImage(keyName, fileName, width, height, frameX, frameY, trans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string keyName, const char* fileName, 
	float x, float y, int width, int height, int frameX, int frameY, 
	bool trans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->initForFrameImage(keyName, fileName, x, y, width, height, frameX, frameY, trans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadSound(string keyName, string soundName, bool background, bool loop)
{
	loadItem* item = new loadItem;
	item->initForSound(keyName, soundName, background, loop);
	_vLoadItem.push_back(item);
}

BOOL loading::loadNext(void)
{
	//아이템 갯수가 벡터 사이즈이상 넘어가면 리턴 펄쓰..
	if (_current >= _vLoadItem.size())
	{
		return false;
	}

 	loadItem* item = _vLoadItem[_current];
	switch (item->getLoadingKind())
	{
		case LOADING_KIND_ADDIMAGE_00:
		{
			tagImageResource ir = item->getImageResource();
			IMAGEMANAGER->addImage(ir.keyName, ir.width, ir.height);
		}
		break;
		case LOADING_KIND_ADDIMAGE_01:
		{
			tagImageResource ir = item->getImageResource();
			IMAGEMANAGER->addImage(ir.keyName, ir.fileName, ir.width, ir.height, ir.trans, ir.transColor);
		}
		break;
		case LOADING_KIND_ADDIMAGE_02:
		{
			tagImageResource ir = item->getImageResource();
			IMAGEMANAGER->addImage(ir.keyName, ir.fileName, ir.x, ir.y, ir.width, ir.height, ir.trans, ir.transColor);
		}
		break;
		case LOADING_KIND_ADDFRAMEIMAGE_00:
		{
			tagImageResource ir = item->getImageResource();
			IMAGEMANAGER->addFrameImage(ir.keyName, ir.fileName, ir.width, ir.height, ir.frameX, ir.frameY, ir.trans, ir.transColor);
		}
		break;
		case LOADING_KIND_ADDFRAMEIMAGE_01:
		{
			tagImageResource ir = item->getImageResource();
			IMAGEMANAGER->addFrameImage(ir.keyName, ir.fileName, ir.x, ir.y, ir.width, ir.height, ir.frameX, ir.frameY, ir.trans, ir.transColor);
		}
		break;
		case LOADING_KIND_ADDSOUND:
		{
			tagsoundResource ir = item->getSoundResource();
			SOUNDMANAGER->addSound(ir.keyName, ir.soundName, ir.background, ir.loop);
		}
		break;
	}

	//로딩바 이미지 변경해준다
	_loadingBar->setGuage(_current, _vLoadItem.size());

	//로딩 갯수 카운팅
	_current++;

	return true;
}





///////////////////// loading ///////////////////////