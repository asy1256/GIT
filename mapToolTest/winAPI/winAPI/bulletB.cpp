#include "stdafx.h"
#include "bulletB.h"

bulletB::bulletB()
{
}

bulletB::~bulletB()
{
}

HRESULT bulletB::init(void)
{
	character::init();

	return S_OK;
}

void bulletB::release(void)
{
	character::release();
}

void bulletB::update(void)
{
	character::update();
}

void bulletB::render(HDC hdc)
{

}