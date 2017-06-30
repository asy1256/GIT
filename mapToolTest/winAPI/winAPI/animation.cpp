#include "StdAfx.h"
#include "animation.h"

animation::animation(void)
	:_frameWidth(0),
	_frameHeight(0),
	_loop(false),
	_frameUpdateSec(0.0f),
	_elapsedSec(0.0f),
	_nowPlayIdx(0),
	_isPlay(false),
	_cbFunctionParameter(NULL),
	_cbFunction(NULL),
	_obj(NULL)
{
}

animation::~animation(void)
{
}

//초기화
HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	//가로 프레임 수 구한다
	_frameWidth = frameW;
	int frameNumWidth = totalW / _frameWidth;

	//세로 프레임 수 구한다
	_frameHeight = frameH;
	int frameNumHeight = totalH / _frameHeight;

	//총 프레임 갯수 구한다
	_frameNum = frameNumWidth * frameNumHeight;

	_frameList.clear();
	//프레임 위치 리스트 셋팅
	for (int i = 0; i < frameNumHeight; i++)
	{
		for (int j = 0; j < frameNumWidth; j++)
		{
			POINT framePos;
			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			//프레임 위치 리스트에 추가한다
			_frameList.push_back(framePos);
		}
	}

	//기본셋팅 디폴트로...
	setDefPlayFrame();
		
	return S_OK;
}

void animation::release(void)
{
}

//처음부터 끝까지 애니메이션... (디폴트 값)
void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	//뺑뻉이냐?
	_loop = loop;

	//플레이 리스트 끌리어..
	_playList.clear();

	//왔다리 갔다리냐?
	if (reverse)
	{
		//루프임?
		if (loop)
		{
			//갈때 프레임..
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			
			//올때 프레임
			for (int i = _frameNum - 2; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			//갈때 프레임..
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			
			//올때 프레임
			for (int i = _frameNum - 2; i >= 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		//갈때 프레임..
		for (int i = 0; i < _frameNum; i++)
		{
			_playList.push_back(i);
		}
	}
}

//원하는 인덱스 애니메이션...
void animation::setPlayFrame(int* arr, int arrLen, BOOL loop)
{
	//뺑뻉이냐?
	_loop = loop;

	//플레이 리스트 끌리어..
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(arr[i]);
	}
}

//구간 애니메이션...
void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	//뺑뻉이냐?
	_loop = loop;

	//플레이 리스트 끌리어..
	_playList.clear();

	//시작과 끝이 같으면.......
	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	//시작프레임이 종료프레임보다 큰 경우
	if (start > end)
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i < start; i++)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i >= end; i--)
				{
					_playList.push_back(i);
				}

				for (int i = end + 1; i <= start; i++)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i >= end; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	//스타트 < 엔드
	else
	{
		if (reverse)
		{
			if (_loop)
			{
				for (int i = start; i <= end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end - 1; i > start; i--)
				{
					_playList.push_back(i);
				}
			}
			else
			{
				for (int i = start; i <= end; i++)
				{
					_playList.push_back(i);
				}

				for (int i = end - 1; i >= start; i--)
				{
					_playList.push_back(i);
				}
			}
		}
		else
		{
			for (int i = start; i <= end; i++)
			{
				_playList.push_back(i);
			}
		}
	}
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	setDefPlayFrame(reverse, loop);
	_cbFunction = static_cast<CALLBACK_FUNCTION>(cbFunction);
}

void animation::setPlayFrame(int* arr, int arrLen, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	setPlayFrame(arr, arrLen, loop);
	_cbFunction = static_cast<CALLBACK_FUNCTION>(cbFunction);
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION cbFunction)
{
	setPlayFrame(start, end, reverse, loop);
	_cbFunction = static_cast<CALLBACK_FUNCTION>(cbFunction);
}

void animation::setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	setDefPlayFrame(reverse, loop);
	_cbFunctionParameter = static_cast<CALLBACK_FUNCTION_PARAMETER>(cbFunction);
	_obj = obj;
}

void animation::setPlayFrame(int* arr, int arrLen, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	setPlayFrame(arr, arrLen, loop);
	_cbFunctionParameter = static_cast<CALLBACK_FUNCTION_PARAMETER>(cbFunction);
	_obj = obj;
}

void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	setPlayFrame(start, end, reverse, loop);
	_cbFunctionParameter = static_cast<CALLBACK_FUNCTION_PARAMETER>(cbFunction);
	_obj = obj;
}

//초당 갱신 횟수임...
void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

//프레임 업데이트...
void animation::frameUpdate(float elapsedTime)
{
	if (_isPlay)
	{
		_elapsedSec += elapsedTime;

		//프레임 업데이트 시간이 되었으면....
		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIdx++;
			if (_nowPlayIdx == _playList.size())
			{
				//뺑뺑이냐?
				if (_loop)
				{
					_nowPlayIdx = 0;
				}
				else
				{
					if (_obj == NULL)
					{
						if (_cbFunction != NULL) _cbFunction();
					}
					else
					{
						if (_cbFunctionParameter != NULL) _cbFunctionParameter(_obj);
					}
					_nowPlayIdx--;
					_isPlay = false;
				}
			}
		}
	}
}

//애니메이션 시작...
void animation::start(void)
{
	_isPlay = true;
	_nowPlayIdx = 0;
}

//애니메이션 정지...
void animation::stop(void)
{
	_isPlay = false;
	_nowPlayIdx = 0;
}

//애니메이션 일시 정지...
void animation::pause(void)
{
	_isPlay = false;
}

//애니메이션 일시정지에서 다시 시작.....
void animation::resume(void)
{
	_isPlay = true;
}