#pragma once

#include "singletonbase.h"

#include <map>
#include <string>

//fmod.hpp 추가해준다
#include "inc/fmod.hpp"

//lib 링크
#pragma comment (lib, "lib/fmodex_vc.lib")

//채널 갯수 (버퍼~!!!)
#define EXTRACHANNLEBUFFER 5
#define SOUNDBUFFER 10

//총 사운드 갯수
#define TOTALSOUNDBUFFER (EXTRACHANNLEBUFFER + SOUNDBUFFER)

using namespace FMOD;

class soundManager : public singletonBase <soundManager>
{
private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundsIter;
	typedef map<string, Channel**> arrChannels;
	typedef map<string, Channel**>::iterator arrChannelsIter;

private:
	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;

public:
	HRESULT init(void);
	void release(void);

	void addSound(string keyName, string soundName, bool background, bool loop);
	void play(string keyName);
	void play(string keyName, float volume);
	void stop(string keyName);
	void pause(string keyName);
	void resume(string keyName);

	bool isPause(string keyName);
	bool isPlay(string keyName);

	//음악 길이값 가져옴.
	unsigned int getLength(string keyName);

	//음악 재생 위치 설정
	void setPosition(string keyName, unsigned int ms);
	unsigned int getPosition(string keyName);

	//fmod 시스템 갱신한다
	void update(void);

	soundManager(void);
	virtual ~soundManager(void);
};

