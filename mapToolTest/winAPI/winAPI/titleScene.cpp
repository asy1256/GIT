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

	_bimg = IMAGEMANAGER->findImage("title");

	for (int y = WINSIZEY - 200, i = 0; i < 3; ++i, y += 50)
	{
		_button[i] = RectMake(50, y, 100, 30);
	}
<<<<<<< HEAD

=======
	
>>>>>>> 435d19df0aabdf37fbd42fb6d81ff6b5a9aece44
	return S_OK;
}

void titleScene::release(void)
{
	gameNode::release();
}

void titleScene::update(void)
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < 3; ++i)
		{
			if (PtInRect(&_button[i], _ptMouse))
			{
				if (i == 0) { SCENEMANAGER->changeScene("mapTool"); }
				if (i == 1) { SCENEMANAGER->changeScene("gungeonScene"); }
				if (i == 2) { PostMessage(_hWnd, WM_DESTROY, 0, 0); }
			}
		}
	}

}

void titleScene::render(void)
{
	_bimg->render(getMemDC());
	char str[16];

	sprintf(str, "맵툴");
	TextOut(getMemDC(), _button[0].left + 5, _button[0].top + 10, str, strlen(str));
	sprintf(str, "게임시작");
	TextOut(getMemDC(), _button[1].left + 5, _button[1].top + 10, str, strlen(str));
	sprintf(str, "나가기");
	TextOut(getMemDC(), _button[2].left + 5, _button[2].top + 10, str, strlen(str));
}