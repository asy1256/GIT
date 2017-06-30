#include "StdAfx.h"
#include "frameAnimationManager.h"
#include "frameAnimation.h"

frameAnimationManager::frameAnimationManager(void)
{
}

frameAnimationManager::~frameAnimationManager(void)
{
}

HRESULT frameAnimationManager::init(void)
{
	return S_OK;
}

void frameAnimationManager::release(void)
{
	for (_viAnimation = _vAnimation.begin(); _viAnimation != _vAnimation.end();)
	{
		if (_vAnimation.size() != 0)
		{
			(*_viAnimation)->release();
			SAFE_DELETE(*_viAnimation);
			_viAnimation = _vAnimation.erase(_viAnimation);
		}
		else ++_viAnimation;
	}

	_vAnimation.clear();
}

void frameAnimationManager::update(void)
{
	for (int i = 0; i < _vAnimation.size(); i++)
	{
		_vAnimation[i]->update();
		if (!_vAnimation[i]->getIsPlay())
		{
			_vAnimation[i]->release();
			SAFE_DELETE(_vAnimation[i]);
			_vAnimation.erase(_vAnimation.begin() + i);
			break;
		}
	}
}

void frameAnimationManager::render(void)
{
}

void frameAnimationManager::animationXFrame(image* img, int xFrame, float count, bool rewind)
{
	frameAnimation* fa = new frameAnimation;
	fa->init();
	fa->animationXFrame(img, xFrame, count, rewind);
	_vAnimation.push_back(fa);
}

void frameAnimationManager::animationYFrame(image* img, int yFrame, float count, bool rewind)
{
	frameAnimation* fa = new frameAnimation;
	fa->init();
	fa->animationYFrame(img, yFrame, count, rewind);
	_vAnimation.push_back(fa);
}

void frameAnimationManager::animationXFrame(image* img, int xFrame, 
	float count, bool rewind, void* cbFunction)
{
	frameAnimation* fa = new frameAnimation;
	fa->init();
	fa->animationXFrame(img, xFrame, count, rewind, (CALLBACK_FUNCTION)cbFunction);
	_vAnimation.push_back(fa);
}

void frameAnimationManager::animationYFrame(image* img, int yFrame, 
	float count, bool rewind, void* cbFunction)
{
	frameAnimation* fa = new frameAnimation;
	fa->init();
	fa->animationYFrame(img, yFrame, count, rewind, (CALLBACK_FUNCTION)cbFunction);
	_vAnimation.push_back(fa);
}
