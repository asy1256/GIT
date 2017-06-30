#pragma once

#include <math.h>

#define DEG_TO_RAD 0.017453f
#define PI 3.141592654f
#define PI2 6.283185f
#define PI_2 float(PI / 2.0f)
#define PI_4 float(PI / 4.0f)
#define PI_8 float(PI / 8.0f)
#define PI_16 float(PI / 16.0f)
#define PI_32 float(PI / 32.0f)
#define PI_64 float(PI / 64.0f)
#define PI_128 float(PI / 128.0f)
#define PI_256 float(PI / 256.0f)

#define FLOAT_EPSILON 0.001f

//실수를 정수형으로 변환한다..!!
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON)

//두 실수가 같은가 확인한다!!
#define FLOAT_EQUAL(f1, f2) (fabs(f1 - f2) <= FLOAT_EPSILON)
//fabs는 소수점의 차 절대값을 반환하는 함수이다.

namespace MY_UTIL
{
	//거리 구한다
	float getDistance(float startX, float startY, float endX, float endY);
	
	//시작점부터 끝 점까지의 각을 라디안으로 구한다
	float getAngle(float startX, float startY, float endX, float endY);
}