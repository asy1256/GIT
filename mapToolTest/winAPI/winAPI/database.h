#pragma once

#include "singletonbase.h"

#include <vector>
#include <map>

class elements
{
public:
	const char* name;
	float increaseSpeed;
	float maxSpeed;
	float angle;
	int currentHP;
	int maxHP;

	elements(void) {};
	~elements(void) {};
};

class database : public singletonBase <database>
{
private:
	typedef vector<string> arrElements;
	typedef vector<string>::iterator iterElements;
	typedef map<string, elements*> arrElement;
	typedef map<string, elements*>::iterator iterElement;

private:
	arrElement _mTotalElement;

public:
	int roomcount = 0;
	int nowroom = 0;
	bool blank = false;
	vector<bool> roomclear;
	vector<bool> roomfight;
	vector<int> roomsequnce;
	POINT spon = PointMake(0, 0);
	POINT past = PointMake(0, 0);
	POINT moveTo = PointMake(0, 0);
	POINT moveadd = PointMake(0, 0);

public:
	HRESULT init(void);
	void release(void);

	void loadDatabase(void);

	elements* getElementData(string str)
	{
		return _mTotalElement.find(str)->second;
	}

	void setElementDataIncreaseSpeed(string str, float is);
	void setElementDataMaxSpeed(string str, float ms);
	void setElementDataAngle(string str, float a);
	void setElementDataCurrentHP(string str, int ch);
	void setElementDataMaxHP(string str, int mh);

	float getElementDataIncreaseSpeed(string str);
	float getElementDataMaxSpeed(string str);
	float getElementDataAngle(string str);
	int getElementDataCurrentHP(string str);
	int getElementDataMaxHP(string str);
	
	database(void);
	~database(void);
};