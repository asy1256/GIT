#pragma once

#include "singletonbase.h"
#include "timer.h"

//====================================================
//####### 2017.05.01 헤더파일 image class #############
//====================================================
class timeManager : public singletonBase <timeManager>
{
private:
	timer* _timer;

public:
	//초기화
	HRESULT init(void);

	//릴리즈
	void release(void);

	//프레임 적용
	void update(float lock = 0.0f);

	//해당 HDC에 정보 출력한다
	void render(HDC hdc);

	//전체 시간 가져온다
	inline float getWorldTime(void) const {return _timer->getWorldTime();}

	//프레임 경과시간
	inline float getElapsedTime(void) const {return _timer->getElapsedTime();}

	timeManager(void);
	~timeManager(void);
};

