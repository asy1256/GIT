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

//�ʱ�ȭ
HRESULT animation::init(int totalW, int totalH, int frameW, int frameH)
{
	//���� ������ �� ���Ѵ�
	_frameWidth = frameW;
	int frameNumWidth = totalW / _frameWidth;

	//���� ������ �� ���Ѵ�
	_frameHeight = frameH;
	int frameNumHeight = totalH / _frameHeight;

	//�� ������ ���� ���Ѵ�
	_frameNum = frameNumWidth * frameNumHeight;

	_frameList.clear();
	//������ ��ġ ����Ʈ ����
	for (int i = 0; i < frameNumHeight; i++)
	{
		for (int j = 0; j < frameNumWidth; j++)
		{
			POINT framePos;
			framePos.x = j * _frameWidth;
			framePos.y = i * _frameHeight;

			//������ ��ġ ����Ʈ�� �߰��Ѵ�
			_frameList.push_back(framePos);
		}
	}

	//�⺻���� ����Ʈ��...
	setDefPlayFrame();
		
	return S_OK;
}

void animation::release(void)
{
}

//ó������ ������ �ִϸ��̼�... (����Ʈ ��)
void animation::setDefPlayFrame(BOOL reverse, BOOL loop)
{
	//���R�̳�?
	_loop = loop;

	//�÷��� ����Ʈ ������..
	_playList.clear();

	//�Դٸ� ���ٸ���?
	if (reverse)
	{
		//������?
		if (loop)
		{
			//���� ������..
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			
			//�ö� ������
			for (int i = _frameNum - 2; i > 0; i--)
			{
				_playList.push_back(i);
			}
		}
		else
		{
			//���� ������..
			for (int i = 0; i < _frameNum; i++)
			{
				_playList.push_back(i);
			}
			
			//�ö� ������
			for (int i = _frameNum - 2; i >= 0; i--)
			{
				_playList.push_back(i);
			}
		}
	}
	else
	{
		//���� ������..
		for (int i = 0; i < _frameNum; i++)
		{
			_playList.push_back(i);
		}
	}
}

//���ϴ� �ε��� �ִϸ��̼�...
void animation::setPlayFrame(int* arr, int arrLen, BOOL loop)
{
	//���R�̳�?
	_loop = loop;

	//�÷��� ����Ʈ ������..
	_playList.clear();

	for (int i = 0; i < arrLen; i++)
	{
		_playList.push_back(arr[i]);
	}
}

//���� �ִϸ��̼�...
void animation::setPlayFrame(int start, int end, BOOL reverse, BOOL loop)
{
	//���R�̳�?
	_loop = loop;

	//�÷��� ����Ʈ ������..
	_playList.clear();

	//���۰� ���� ������.......
	if (start == end)
	{
		_playList.clear();
		stop();
		return;
	}

	//������������ ���������Ӻ��� ū ���
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
	//��ŸƮ < ����
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

//�ʴ� ���� Ƚ����...
void animation::setFPS(int framePerSec)
{
	_frameUpdateSec = 1.0f / framePerSec;
}

//������ ������Ʈ...
void animation::frameUpdate(float elapsedTime)
{
	if (_isPlay)
	{
		_elapsedSec += elapsedTime;

		//������ ������Ʈ �ð��� �Ǿ�����....
		if (_elapsedSec >= _frameUpdateSec)
		{
			_elapsedSec -= _frameUpdateSec;
			_nowPlayIdx++;
			if (_nowPlayIdx == _playList.size())
			{
				//�����̳�?
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

//�ִϸ��̼� ����...
void animation::start(void)
{
	_isPlay = true;
	_nowPlayIdx = 0;
}

//�ִϸ��̼� ����...
void animation::stop(void)
{
	_isPlay = false;
	_nowPlayIdx = 0;
}

//�ִϸ��̼� �Ͻ� ����...
void animation::pause(void)
{
	_isPlay = false;
}

//�ִϸ��̼� �Ͻ��������� �ٽ� ����.....
void animation::resume(void)
{
	_isPlay = true;
}