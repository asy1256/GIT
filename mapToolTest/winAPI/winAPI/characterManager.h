#pragma once

#include "gameNode.h"
#include "player.h"
#include "bullets.h"

class characterManager : public gameNode
{
private:
	vector<gameNode*> _anything;
	bulletE* _ebullet;

	player* _pl;

public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	inline player* &getPlayer(void) { return _pl; }

	characterManager();
	virtual ~characterManager();
};