#pragma once

#include "singletonbase.h"

enum Clan
{
	FREE,
	OPEN,
	CLOSE
};

struct tagAstile
{
	Clan clan;
	int idX, idY;
	int travalcost;
	int vectorcost;
	int totalcost;
	tagAstile* previous;
};

class aStar : public singletonBase <aStar>
{
private:
	vector<POINT> _vPath;
	vector<tagAstile> _closeList;
	vector<tagAstile> _openList;
	tagAstile _aStile[TILEY][TILEX];

public:
	HRESULT init(void);
	void release(void);

	void clearset(void);
	vector<POINT> getPath(int startX, int startY, int endX, int endY);
	void vectorcostSet(POINT &current);
	void travalcostSet(int endX, int endY);
	void currentMove(POINT &current);
	void pathPind(int endX, int endY);

	aStar();
	virtual ~aStar();
};