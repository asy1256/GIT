#pragma once

#include <vector>

//=====================================================
//########## 2017.03.20 헤더파일 animation #############
//=====================================================

typedef void (*CALLBACK_FUNCTION)(void);
typedef void (*CALLBACK_FUNCTION_PARAMETER)(void*);

class animation
{
public:
	typedef std::vector<POINT> vFrameList; //프레임 리스트
	typedef std::vector<int> vPlayList;	//프레임재생 리스트
	
private:
	vFrameList _frameList;
	vPlayList _playList;

	int _frameWidth;		//프레임 가로크기
	int _frameHeight;		//프레임 세로크기

	int _frameNum;			//프레임 갯수

	bool _loop;				//반복?
	float _frameUpdateSec;	//프레임 업데이트 시간
	float _elapsedSec;		//프레임 시간에 대한 주기

	DWORD _nowPlayIdx;		//현재 애니메이션 인덱스
	bool _isPlay;			//재생중?

	CALLBACK_FUNCTION _cbFunction;
	CALLBACK_FUNCTION_PARAMETER _cbFunctionParameter;

	void* _obj;

public:
	//초기화
	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release(void);

	//처음부터 끝까지 애니메이션... (디폴트 값)
	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);

	//원하는 인덱스 애니메이션...
	void setPlayFrame(int* arr, int arrLen, BOOL loop = FALSE);

	//구간 애니메이션...
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);

	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int* arr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction);

	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);
	void setPlayFrame(int* arr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	//초당 갱신 횟수임...
	void setFPS(int framePerSec);

	//프레임 업데이트...
	void frameUpdate(float elapsedTime);

	void start(void);		//애니 시작
	void stop(void);		//애니 정지
	void pause(void);		//애니 일시 정지 
	void resume(void);		//애니 다시 시작

	//플레이냐?
	inline BOOL isPlay(void) {return _isPlay;}

	//프레임 위치 얻어온다
	inline POINT getFramePos(void) {return _frameList[_playList[_nowPlayIdx]];}

	//프레임 가로 크기 얻어온다
	inline int getFrameWidth(void) {return _frameWidth;}

	//프레임 세로 크기 얻어온다
	inline int getFrameHeight(void) {return _frameHeight;}


	animation(void);
	~animation(void);
};

