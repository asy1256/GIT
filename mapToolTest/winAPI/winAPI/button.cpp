#include "StdAfx.h"
#include "button.h"


button::button(void)
	:_callbackFunction(NULL),
	_callbackFunctionParameter(NULL),
	_obj(NULL)
{
}


button::~button(void)
{
}

HRESULT button::init(const char* imageName, int x, int y, 
	POINT btnDownFramePoint, POINT btnUpFramePoint)
{
	gameNode::init();

	_direction = BUTTONDIRECTION_NULL;

	_callbackFunction = NULL;

	_x = x;
	_y = y;

	_btnDownFramePoint = btnDownFramePoint;
	_btnUpFramePoint = btnUpFramePoint;

	_imageName = imageName;
	_image = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(x, y, _image->getFrameWidth(),
		_image->getFrameHeight());

	return S_OK;
}

HRESULT button::init(const char* imageName, int x, int y, 
		POINT btnDownFramePoint, POINT btnUpFramePoint,
		CALLBACK_FUNCTION cbFunction)
{
	gameNode::init();

	_direction = BUTTONDIRECTION_NULL;

	_callbackFunction = static_cast<CALLBACK_FUNCTION>(cbFunction);

	_x = x;
	_y = y;

	_btnDownFramePoint = btnDownFramePoint;
	_btnUpFramePoint = btnUpFramePoint;

	_imageName = imageName;
	_image = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(x, y, _image->getFrameWidth(),
		_image->getFrameHeight());

	return S_OK;
}

HRESULT button::init(const char* imageName, int x, int y, 
	POINT btnDownFramePoint, POINT btnUpFramePoint,
	CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj)
{
	gameNode::init();

	_direction = BUTTONDIRECTION_NULL;

	_callbackFunctionParameter = static_cast<CALLBACK_FUNCTION_PARAMETER>(cbFunction);
	_obj = obj;

	_x = x;
	_y = y;

	_btnDownFramePoint = btnDownFramePoint;
	_btnUpFramePoint = btnUpFramePoint;

	_imageName = imageName;
	_image = IMAGEMANAGER->findImage(imageName);

	_rc = RectMakeCenter(x, y, _image->getFrameWidth(),
		_image->getFrameHeight());

	return S_OK;
}

void button::release(void)
{
	gameNode::release();
}

void button::update(void)
{
	gameNode::update();

	if (PtInRect(&_rc, _ptMouse))
	{
		if (_leftButtonDown)
		{
			_direction = BUTTONDIRECTION_DOWN;
		}
		else if (!_leftButtonDown && _direction == BUTTONDIRECTION_DOWN)
		{
			_direction = BUTTONDIRECTION_UP;
			if (_obj == NULL) _callbackFunction();
			else _callbackFunctionParameter(_obj);
		}
	}
	else _direction = BUTTONDIRECTION_NULL;
}

void button::render(void)
{
	switch (_direction)
	{
		case BUTTONDIRECTION_UP: case BUTTONDIRECTION_NULL:
			_image->frameRender(getMemDC(), _rc.left, _rc.top,
				_btnUpFramePoint.x, _btnUpFramePoint.y);
		break;
		case BUTTONDIRECTION_DOWN:
			_image->frameRender(getMemDC(), _rc.left, _rc.top,
				_btnDownFramePoint.x, _btnDownFramePoint.y);
		break;
	}
}
