#include "StdAfx.h"
#include "timeManager.h"

timeManager::timeManager(void)
{
}

timeManager::~timeManager(void)
{
}

//초기화
HRESULT timeManager::init(void)
{
	//타이머 생성 및 초기화
	_timer = new timer;
	_timer->init();

	return S_OK;
}

//릴리즈
void timeManager::release(void)
{
	SAFE_DELETE(_timer);
}

//프레임 적용
void timeManager::update(float lock)
{
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

//해당 HDC에 정보 출력한다
void timeManager::render(HDC hdc)
{
	char str[256];
	std::string strFrame;

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

#ifdef _DEBUG
	if (_timer != NULL)
	{
		//프레임 출력
		sprintf_s(str, "framePerSec : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	
		//월드 타임 출력
		sprintf_s(str, "worldTime : %f", _timer->getWorldTime());
		TextOut(hdc, 0, 20, str, strlen(str));

		//갱신 Tick 값 출력
		sprintf_s(str, "elapsedTime : %f", _timer->getElapsedTime());
		TextOut(hdc, 0, 40, str, strlen(str));
	}
#else
	if (_timer != NULL)
	{
		//프레임 출력
		sprintf_s(str, "framePerSec : %d", _timer->getFrameRate());
		TextOut(hdc, 0, 0, str, strlen(str));
	}
#endif
}
