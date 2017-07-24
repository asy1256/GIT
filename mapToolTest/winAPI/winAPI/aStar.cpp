#include "stdafx.h"
#include "aStar.h"

aStar::aStar()
{
}

aStar::~aStar()
{
}

HRESULT aStar::init(void)
{
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			_aStile[y][x].clan = FREE;
			_aStile[y][x].idX = x;
			_aStile[y][x].idY = y;
			_aStile[y][x].totalcost = _aStile[y][x].travalcost = _aStile[y][x].vectorcost = 0;
			_aStile[y][x].previous = NULL;
		}
	}

	return S_OK;
}

void aStar::release(void)
{

}

void aStar::clearset(void)
{
	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			_aStile[y][x].clan = FREE;
			_aStile[y][x].totalcost = _aStile[y][x].travalcost = _aStile[y][x].vectorcost = 0;
			_aStile[y][x].previous = NULL;
		}
	}

	_openList.clear();
	_closeList.clear();
	_vPath.clear();
}

vector<POINT> aStar::getPath(int startX, int startY, int endX, int endY)
{
	clearset();

	bool whille = true;
	POINT currentTile = PointMake(startX, startY);
	_closeList.push_back(_aStile[currentTile.y][currentTile.x]);
	_aStile[currentTile.y][currentTile.x].clan = CLOSE;
	int sX, sY, eX, eY;
	if (startX < endX)
	{
		sX = startX;
		eX = endX + 1;
	}
	else if (startX > endX)
	{
		sX = endX;
		eX = startX + 1;
	}
	else
	{
		sX = startX - 1;
		eX = startX + 1;
	}
	if (startY < endY)
	{
		sY = startY;
		eY = endY + 1;
	}
	else if (startY > endY)
	{
		sY = endY;
		eY = startY + 1;
	}
	else
	{
		sY = startY - 1;
		eY = startY + 1;
	}

	travalcostSet(endX, endY);

	while (whille)
	{
		vectorcostSet(currentTile);
		currentMove(currentTile);
		if (currentTile.x == endX && currentTile.y == endY) { whille = false; }
	}
	if (currentTile.x == endX)
	{
		_aStile[0][0].idX = 0;
	}

	pathPind(endX, endY);

	return _vPath;
}

void aStar::vectorcostSet(POINT &current)
{
	for (int y = current.y - 1; y < current.y + 2; ++y)
	{
		for (int x = current.x - 1; x < current.x + 2; ++x)
		{
			if (y == current.y && x == current.x) { continue; }
			if (y == current.y || x == current.x) { _aStile[y][x].vectorcost = 10; }
			else { _aStile[y][x].vectorcost = 14; }
			_aStile[y][x].totalcost = _aStile[y][x].vectorcost + _aStile[y][x].travalcost;
		}
	}
}

void aStar::travalcostSet(int endX, int endY)
{
	int trvX, trvY;
	trvX = trvY = 0;

	for (int y = 0; y < TILEY; ++y)
	{
		for (int x = 0; x < TILEX; ++x)
		{
			if (x < endX) { trvX = endX - x; }
			else { trvX = x - endX + 1; }
			if (y < endY) { trvY = endY - y; }
			else { trvY = y - endY; }
			_aStile[y][x].travalcost = (trvX + trvY) * 10;
		}
	}

	/*for (int y = startY - 1; y < endY + 1; ++y)
	{
		for (int x = startX - 1; x < endX + 1; ++x)
		{
			if (x < endX) { trvX = endX - x; }
			else { trvX = x - endX + 1; }
			if (y < endY) { trvY = endY - y; }
			else { trvY = y - endY; }
			_aStile[y][x].travalcost = (trvX + trvY) * 10;
		}
	}*/
}

void aStar::currentMove(POINT &current)
{
	vector<POINT> _vtemp;
	vector<POINT>::iterator _vitemp;

	//현재 더미 위치주변 8타일 가져오기
	for (int y = current.y - 1; y < current.y + 2; ++y)
	{
		for (int x = current.x - 1; x < current.x + 2; ++x)
		{
			if (x == current.x && y == current.y) { continue; }
			_vtemp.push_back(PointMake(x, y));
		}
	}
	//가져온 8타일에서 갈수 없는곳과 오픈 혹은 클로즈 리스트에 이미 들어간 애들 제거
	for (_vitemp = _vtemp.begin(); _vitemp != _vtemp.end();)
	{
		if (_aStile[_vitemp->y][_vitemp->x].clan != FREE || !_Tile[_vitemp->y][_vitemp->x].pass)
		{
			_vitemp = _vtemp.erase(_vitemp++);
		}
		else if ((!_Tile[_vitemp->y][_vitemp->x - 1].pass && _vitemp->x != current.x) || (!_Tile[_vitemp->y][_vitemp->x + 1].pass && _vitemp->x != current.x) ||
			(!_Tile[_vitemp->y - 1][_vitemp->x].pass && _vitemp->y != current.y) || (!_Tile[_vitemp->y + 1][_vitemp->x].pass && _vitemp->y != current.y))
		{
			_vitemp = _vtemp.erase(_vitemp++);
		}
		else { ++_vitemp; }
	}
	//살아남은 애들 오픈리스트에 집어넣자
	for (_vitemp = _vtemp.begin(); _vitemp != _vtemp.end(); ++_vitemp)
	{
		_aStile[_vitemp->y][_vitemp->x].clan = OPEN;
		_aStile[_vitemp->y][_vitemp->x].previous = &_aStile[current.y][current.x];
		_openList.push_back(_aStile[_vitemp->y][_vitemp->x]);
	}
	//오픈리스트 뒤져서 현재 타일로부터 가장 가까운 타일을 찾자
	int tempn = 1000;
	vector<tagAstile>::iterator iter;

	for (int i = 0; i < _openList.size(); ++i)
	{
		if (tempn < _openList[i].totalcost) { continue; }
		tempn = _openList[i].totalcost;
		current.x = _openList[i].idX;
		current.y = _openList[i].idY;
		iter = _openList.begin() + i;
	}
	if (!_openList.empty()) { }
	_openList.erase(iter);
	_closeList.push_back(_aStile[current.y][current.x]);
	_aStile[current.y][current.x].clan = CLOSE;
}

void aStar::pathPind(int endX, int endY)
{	
	for (tagAstile* iter = &_aStile[endY][endX]; iter->previous != NULL;)
	{
		_vPath.push_back(PointMake(iter->idX, iter->idY));
		iter = iter->previous;
	}
	std::reverse(_vPath.begin(), _vPath.end());
}