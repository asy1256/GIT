#pragma once

#include "singletonBase.h"
#include <iostream>
#include <time.h>

class randomFunction : public singletonBase <randomFunction>
{
public:

	randomFunction(void)
	{
		//srand(time(NULL)); 오늘부로 버림...
		srand(GetTickCount());
		//컴퓨터 시간... 켰을때부터 시간을 잰다..
	}
	~randomFunction(void)
	{
	}

	//getInt
	int getInt(int num) {return rand() % num;}
	int getFromIntTo(int fromNum, int toNum)
	{
		return (rand() % (toNum - fromNum + 1)) + fromNum;
	}

	//rand 함수의 최대값 32767
	float getFloat() {return ((float)rand() / (float)RAND_MAX);}

	float getFloat(float num)
	{
		return ((float)rand() / (float)RAND_MAX) * num;
	}

	float getFromFloatTo(float fromFloat, float toFloat)
	{
		float rnd = (float)rand() / (float)RAND_MAX;
		return (rnd * (toFloat - fromFloat) + fromFloat);
	}
};


