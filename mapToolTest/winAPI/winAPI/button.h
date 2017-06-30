#pragma once

#include "gamenode.h"

typedef void (*CALLBACK_FUNCTION)(void);
typedef void (*CALLBACK_FUNCTION_PARAMETER)(void*);

enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,
	BUTTONDIRECTION_UP,
	BUTTONDIRECTION_DOWN
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;
	const char* _imageName;
	image* _image;

	RECT _rc;
	float _x, _y;

	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction;
	CALLBACK_FUNCTION_PARAMETER _callbackFunctionParameter;

	void* _obj;

public:
	HRESULT init(const char* imageName, int x, int y, 
		POINT btnDownFramePoint, POINT btnUpFramePoint);

	HRESULT init(const char* imageName, int x, int y, 
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction);

	HRESULT init(const char* imageName, int x, int y, 
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);
	
	void release(void);
	void update(void);
	void render(void);

	button(void);
	virtual ~button(void);
};

