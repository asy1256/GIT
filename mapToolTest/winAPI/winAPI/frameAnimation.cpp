#include "StdAfx.h"
#include "frameAnimation.h"


frameAnimation::frameAnimation(void)
	:_callbackFunction(NULL)
{
}


frameAnimation::~frameAnimation(void)
{
}

HRESULT frameAnimation::init(void)
{
	gameNode::init();

	_ad = ANIMATIONDIRECTION_NULL;
	_worldTimeCount = TIMEMANAGER->getWorldTime();
	_isPlay = false;
	_rewind = false;

	return S_OK;
}

void frameAnimation::release(void)
{
	gameNode::release();
}

void frameAnimation::render(void)
{
}

void frameAnimation::update(void)
{
	gameNode::update();

	if (!_isPlay) return;

	switch(_ad)
	{
		case ANIMATIONDIRECTION_X:
			if (_count + _worldTimeCount <= TIMEMANAGER->getWorldTime())
			{
				_worldTimeCount = TIMEMANAGER->getWorldTime();

				_xFrameCount++;
				_img->setFrameX(_xFrameCount);

				if (_xFrameCount > _xFrame)
				{
					if (_rewind) _img->setFrameX(0);
					else _img->setFrameX(_xFrameCount - 1);
					_isPlay = false;

					if (_callbackFunction != NULL) _callbackFunction();
				}
			}
		break;
		case ANIMATIONDIRECTION_Y:
			if (_count + _worldTimeCount <= TIMEMANAGER->getWorldTime())
			{
				_worldTimeCount = TIMEMANAGER->getWorldTime();

				_yFrameCount++;
				_img->setFrameY(_yFrameCount);

				if (_yFrameCount > _yFrame)
				{
					if (_rewind) _img->setFrameY(0);
					else _img->setFrameY(_yFrameCount - 1);
					_isPlay = false;

					if (_callbackFunction != NULL) _callbackFunction();
				}
			}
		break;
	}
}

void frameAnimation::animationXFrame(image* img, int xFrame, float count, bool rewind)
{
	if (_isPlay) return;
	_isPlay = true;

	_count = count;
	_ad = ANIMATIONDIRECTION_X;
	_worldTimeCount = TIMEMANAGER->getWorldTime();
	_img = img;
	_xFrame = xFrame;
	_yFrame = 0;
	_xFrameCount = _yFrameCount = 0;
	_rewind = rewind;
}

void frameAnimation::animationYFrame(image* img, int yFrame, float count, bool rewind)
{
	if (_isPlay) return;
	_isPlay = true;

	_count = count;
	_ad = ANIMATIONDIRECTION_Y;
	_worldTimeCount = TIMEMANAGER->getWorldTime();
	_img = img;
	_xFrame = 0;
	_yFrame = yFrame;
	_xFrameCount = _yFrameCount = 0;
	_rewind = rewind;
}

void frameAnimation::animationXFrame(image* img, int xFrame, float count, 
		bool rewind, CALLBACK_FUNCTION cbFuntion)
{
	_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFuntion);
	animationXFrame(img, xFrame, count, rewind);
}

void frameAnimation::animationYFrame(image* img, int yFrame, float count, 
		bool rewind, CALLBACK_FUNCTION cbFuntion)
{
	_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFuntion);
	animationYFrame(img, yFrame, count, rewind);
}


