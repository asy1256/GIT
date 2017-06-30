#include "StdAfx.h"
#include "database.h"


database::database(void)
{
}

database::~database(void)
{
}

HRESULT database::init(void)
{
	loadDatabase();

	return S_OK;
}

void database::release(void)
{

}

//데이터 로드..
void database::loadDatabase(void)
{
	arrElements vTemp;
	vTemp = TXTMANAGER->load("database.txt");

	string str;
	int count = 0;

	for (unsigned int i = 0; i < vTemp.size(); i++)
	{
		if (vTemp[i] == "|")
		{
			elements* em = new elements; 
			str = vTemp[i + 1];
			_mTotalElement.insert(make_pair(vTemp[i + 1], em));
			if (i != 0) count += 7;
			continue;
		}

		iterElement mIter = _mTotalElement.find(str);
		if (i == count + 1) mIter->second->name = vTemp[i].c_str();
		else if (i == count + 2) mIter->second->increaseSpeed = (float)atof(vTemp[i].c_str());
		else if (i == count + 3) mIter->second->maxSpeed = (float)atof(vTemp[i].c_str());
		else if (i == count + 4) mIter->second->angle = (float)atof(vTemp[i].c_str());
		else if (i == count + 5) mIter->second->currentHP = (int)atoi(vTemp[i].c_str());
		else if (i == count + 6) mIter->second->maxHP = (int)atoi(vTemp[i].c_str());
	}
	vTemp.clear();
}

void database::setElementDataIncreaseSpeed(string str, float is)
{
	_mTotalElement.find(str)->second->increaseSpeed = is;
}

void database::setElementDataMaxSpeed(string str, float ms)
{
	_mTotalElement.find(str)->second->maxSpeed = ms;
}

void database::setElementDataAngle(string str, float a)
{
	_mTotalElement.find(str)->second->angle = a;
}

void database::setElementDataCurrentHP(string str, int ch)
{
	_mTotalElement.find(str)->second->currentHP = ch;
}

void database::setElementDataMaxHP(string str, int mh)
{
	_mTotalElement.find(str)->second->maxHP = mh;
}

float database::getElementDataIncreaseSpeed(string str)
{
	return _mTotalElement.find(str)->second->increaseSpeed;
}

float database::getElementDataMaxSpeed(string str)
{
	return _mTotalElement.find(str)->second->maxSpeed;
}

float database::getElementDataAngle(string str)
{
	return _mTotalElement.find(str)->second->angle;
}

int database::getElementDataCurrentHP(string str)
{
	return _mTotalElement.find(str)->second->currentHP;
}

int database::getElementDataMaxHP(string str)
{
	return _mTotalElement.find(str)->second->maxHP;
}



