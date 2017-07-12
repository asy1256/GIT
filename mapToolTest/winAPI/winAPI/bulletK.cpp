#include "stdafx.h"
#include "bulletK.h"

bulletK::bulletK()
{
}

bulletK::~bulletK()
{
}

HRESULT bulletK::init(void)
{
	character::init();

	return S_OK;
}

void bulletK::release(void)
{
	character::release();
}

void bulletK::update(void)
{
	character::update();
}

void bulletK::render(void)
{

}