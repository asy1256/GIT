#include "StdAfx.h"
#include "keyAnimationManager.h"
#include "animation.h"
#include "image.h"

keyAnimationManager::keyAnimationManager(void)
{
}

keyAnimationManager::~keyAnimationManager(void)
{
}

HRESULT keyAnimationManager::init(void)
{
	return S_OK;
}

void keyAnimationManager::release(void)
{
	deleteAll();
}

void keyAnimationManager::update(void)
{
	arrAnimationIter iter = _mTotalAnimations.begin();

	for (iter; iter != _mTotalAnimations.end(); ++iter)
	{
		if (!iter->second->isPlay()) continue;
		iter->second->frameUpdate(TIMEMANAGER->getElapsedTime());
	}
}

void keyAnimationManager::render(void)
{
}

//ó������ �׳� �ִϸ��̼�....
void keyAnimationManager::addDefPlayFrameAnimation(string aniKeyName, 
		char* imgKeyName, int fps, bool reverse, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setDefPlayFrame(reverse, loop);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

//�������� ������ �κи� �ִϸ��̼�....
void keyAnimationManager::addArrayFrameAnimation(string aniKeyName, 
	char* imgKeyName, int* arr, int arrLen, int fps, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

//������ ���� �ִϸ��̼�...
void keyAnimationManager::addCoordinateFrameAnimation(string aniKeyName, 
	char* imgKeyName, int start, int end, int fps, bool reverse, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

void keyAnimationManager::addDefPlayFrameAnimation(string aniKeyName, 
	char* imgKeyName, int fps, bool reverse, bool loop, void* cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setDefPlayFrame(reverse, loop, (CALLBACK_FUNCTION)cbFunction);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

void keyAnimationManager::addArrayFrameAnimation(string aniKeyName, 
	char* imgKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop, (CALLBACK_FUNCTION)cbFunction);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

void keyAnimationManager::addCoordinateFrameAnimation(string aniKeyName, 
	char* imgKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop, (CALLBACK_FUNCTION)cbFunction);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

void keyAnimationManager::addDefPlayFrameAnimation(string aniKeyName, 
	char* imgKeyName, int fps, bool reverse, bool loop, void* cbFunctionParameter, void* obj)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setDefPlayFrame(reverse, loop, (CALLBACK_FUNCTION_PARAMETER)cbFunctionParameter, obj);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

void keyAnimationManager::addArrayFrameAnimation(string aniKeyName, 
	char* imgKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunctionParameter, void* obj)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop, (CALLBACK_FUNCTION_PARAMETER)cbFunctionParameter, obj);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

void keyAnimationManager::addCoordinateFrameAnimation(string aniKeyName, 
	char* imgKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunctionParameter, void* obj)
{
	image* img = IMAGEMANAGER->findImage(imgKeyName);
	
	animation* ani = new animation;
	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop, (CALLBACK_FUNCTION_PARAMETER)cbFunctionParameter, obj);
	ani->setFPS(fps);

	_mTotalAnimations.insert(make_pair(aniKeyName, ani));
}

//�ִϸ��̼� ã�´�....
animation* keyAnimationManager::findAnimation(string aniKeyName)
{
	arrAnimationIter iter = _mTotalAnimations.find(aniKeyName);
	if (iter != _mTotalAnimations.end())
	{
		return iter->second;
	}

	return NULL;
}

//�ִ� ����
void keyAnimationManager::start(string aniKeyName)
{
	_mTotalAnimations.find(aniKeyName)->second->start();
}

//�ִ�����
void keyAnimationManager::stop(string aniKeyName)
{
	_mTotalAnimations.find(aniKeyName)->second->stop();
}

//�Ͻ�����
void keyAnimationManager::pause(string aniKeyName)
{
	_mTotalAnimations.find(aniKeyName)->second->pause();
}

//�ִ� �Ͻ����� �ٽ� ���
void keyAnimationManager::resume(string aniKeyName)
{
	_mTotalAnimations.find(aniKeyName)->second->resume();
}

//��� �����
void keyAnimationManager::deleteAll(void)
{
	arrAnimationIter iter = _mTotalAnimations.begin();

	for (; iter != _mTotalAnimations.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mTotalAnimations.erase(iter);
		}
		else ++iter;
	}

	_mTotalAnimations.clear();
}

