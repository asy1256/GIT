#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

HRESULT Object::init(float x, float y, OBJECT type, int roomnum)
{
	gameNode::init();

	memset(&_ob, 0, sizeof(_ob));

	return S_OK;
}

HRESULT Object::init(float x, float y, OBJECT type)
{
	gameNode::init();

	memset(&_ob, 0, sizeof(_ob));

	return S_OK;
}

void Object::release(void)
{
	gameNode::release();

}

void Object::update(void)
{
	gameNode::update();

}

void Object::render(HDC hdc)
{

}