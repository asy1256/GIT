#include "StdAfx.h"
#include "utils.h"

namespace MY_UTIL
{
	//거리 구한다
	float getDistance(float startX, float startY, float endX, float endY)
	{
		float dx = endX - startX;
		float dy = endY - startY;

		return sqrt(dx * dx + dy * dy);
	}
	
	//시작점부터 끝 점까지의 각을 라디안으로 구한다
	float getAngle(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - startY;

		float d = sqrt(x * x + y * y);
		
		float angle = acos(x / d);
		if (y > 0) angle = PI2 - angle;
		return angle;
	}
}