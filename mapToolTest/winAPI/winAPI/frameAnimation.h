#pragma once

#include "gamenode.h"

enum ANIMATIONDIRECTION
{
	ANIMATIONDIRECTION_NULL,
	ANIMATIONDIRECTION_X,
	ANIMATIONDIRECTION_Y
};

typedef void (*CALLBACK_FUNCTION)(void);

class frameAnimation : public gameNode
{
private:
	ANIMATIONDIRECTION _ad;

	//æ»æ‚∑™¡‹
	image* _img;
	float _worldTimeCount;
	float _count;
	int _xFrame, _yFrame;
	int _xFrameCount;
	int _yFrameCount;

	bool _isPlay; //«√∑π¿Ã¡ﬂ?
	bool _rewind; //∏« æ’¿∏∑Œ..

	CALLBACK_FUNCTION _callbackFunction;

public:
	HRESULT init(void);
	void release(void);
	void render(void);
	void update(void);

	void animationXFrame(image* img, int xFrame, float count, bool rewind = TRUE);
	void animationYFrame(image* img, int yFrame, float count, bool rewind = TRUE);

	void animationXFrame(image* img, int xFrame, float count, 
		bool rewind, CALLBACK_FUNCTION cbFuntion);
	void animationYFrame(image* img, int yFrame, float count, 
		bool rewind, CALLBACK_FUNCTION cbFuntion);

	bool getIsPlay(void) {return _isPlay;}

	frameAnimation(void);
	virtual ~frameAnimation(void);
};

