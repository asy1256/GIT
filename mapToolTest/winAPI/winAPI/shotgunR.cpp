#include "stdafx.h"
#include "shotgunR.h"

shotgunR::shotgunR()
{
}

shotgunR::~shotgunR()
{
}

HRESULT shotgunR::init(void)
{
	character::init();

	return S_OK;
}

void shotgunR::release(void)
{
	character::release();
}

void shotgunR::update(void)
{
	character::update();
}

void shotgunR::render(void)
{

}