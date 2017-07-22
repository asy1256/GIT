#pragma once

#include "gameNode.h"
#include "bulletK.h"
#include "bulletB.h"
#include "shotgunR.h"
#include "player.h"
#include "bullets.h"

class characterManager : public gameNode
{
private:
	vector<character*> _vCharacter;
	vector<character*>::iterator _viCharacter;
	bulletE* _ebullet;

	player* _pl;
	character* _temp;

public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	inline player* &getPlayer(void) { return _pl; }

	characterManager();
	virtual ~characterManager();
};