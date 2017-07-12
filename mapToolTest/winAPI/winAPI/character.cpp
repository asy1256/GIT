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

void character::render(void)
{

}