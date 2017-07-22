#include "stdafx.h"
#include "character.h"

character::character()
{
}

character::~character()
{
}

HRESULT character::init(void)
{
	gameNode::init();

	memset(&_ch, 0, sizeof(_ch));

	return S_OK;
}

void character::release(void)
{
	gameNode::release();
}

void character::update(void)
{
	gameNode::update();
}

void character::update(int plX, int plY)
{
	gameNode::update();
}

void character::render(HDC hdc)
{

}