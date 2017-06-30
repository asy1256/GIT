#include "StdAfx.h"
#include "soundManager.h"

soundManager::soundManager(void)
{
}

soundManager::~soundManager(void)
{
}

HRESULT soundManager::init(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//채널수 설정
	_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 생성
	_sound = new Sound*[TOTALSOUNDBUFFER];
	_channel = new Channel*[TOTALSOUNDBUFFER];

	//초기화
	ZeroMemory(_sound, sizeof(Sound*) * TOTALSOUNDBUFFER);
	ZeroMemory(_channel, sizeof(Channel*) * TOTALSOUNDBUFFER);

	return S_OK;
}

void soundManager::release(void)
{
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//메모리 지우고..
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	//시스템 닫는다
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}

	_mTotalSounds.clear();
}

void soundManager::addSound(string keyName, 
	string soundName, bool background, bool loop)
{
	//반복이냐?
	if (loop)
	{
		//배경음임?
		if (background)
		{
			_system->createStream(soundName.c_str(),
				FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(),
				FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	else 
	{
		//한번만 사운드 재생
		_system->createSound(soundName.c_str(),
			FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//맵에 사운드를 키값과 함께 넣어준다
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void soundManager::play(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);
		}
	}
} 

//범위 0.0 ~ 1.0
void soundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//사운드 플레이~~~
			_system->playSound(FMOD_CHANNEL_FREE, 
				*iter->second, false, &_channel[count]);
			_channel[count]->setVolume(volume);
		}
	}
}

void soundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool soundManager::isPause(string keyName)
{
	bool pause;
	arrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&pause);
			break;
		}
	}

	return pause;
}

bool soundManager::isPlay(string keyName)
{
	bool pause;
	arrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&pause);
			break;
		}
	}

	return pause;
}

//음악 길이값 가져옴.
unsigned int soundManager::getLength(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	
	for (iter; iter != _mTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			(*iter->second)->getLength(&length, FMOD_TIMEUNIT_MS);

			return length;
		}
	}

	return NULL;
}

//음악 재생 위치 설정
void soundManager::setPosition(string keyName, unsigned int ms)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPosition(ms, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

unsigned int soundManager::getPosition(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();
	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			_channel[count]->getPosition(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
	}

	return NULL;
}

//fmod 시스템 갱신한다
void soundManager::update(void)
{
	_system->update();
	//볼륨이 바뀌거나
	//재생이 끝난 사운드를 채널에서 빼는등의 작업을 자동으로 해줌..
}

