#include "stdafx.h"
#include "player.h"

player::player()
{
}

player::~player()
{
}

HRESULT player::init(void)
{
	character::init();

	return S_OK;
}

void player::release(void)
{
	character::release();
}

void player::update(void)
{
	character::update();
}

void player::render(void)
{

}