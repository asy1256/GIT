#pragma once

#include <vector>
//================================================
//####### 2017.03.21 전역변수 #############
//================================================

//PointMake
inline POINT PointMake(int x, int y)
{
	POINT pt = {x, y};
	return pt;
}

//RectMake
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = {x, y, x + width, y + height};
	return rc;
}

//RectMakeCenter
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = {x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2)};
	return rc;
}

//LineMake
inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

//LinesMake
inline void LinesMake(HDC hdc, std::vector<POINT> vPt)
{
	std::vector<POINT>::iterator viPt;

	int i = 0;
	for (viPt = vPt.begin(); viPt != vPt.end(); ++viPt, i++)
	{
		if (i == 0) MoveToEx(hdc, viPt->x, viPt->y, NULL);
		else LineTo(hdc, viPt->x, viPt->y);
	}
}

//RectangleMake
inline void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

//RectangleMake
inline void RectangleMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}

//EllipseMake
inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

//EllipseMakeCenter
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}









