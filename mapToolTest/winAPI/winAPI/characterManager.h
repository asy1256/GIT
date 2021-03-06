#pragma once

#include "gameNode.h"
#include "bulletK.h"
#include "bulletB.h"
#include "bulletKing.h"
#include "shotgunR.h"
#include "player.h"
#include "bullets.h"
#include "gunnut.h"

class characterManager : public gameNode
{
private:
	vector<character*> _vCharacter;
	vector<character*>::iterator _viCharacter;
	bulletE* _ebullet;

	player* _pl;
	character* _temp;

private:
	vector<bool> _vRoomfight;
	int _nowroom;


public:
	virtual HRESULT init(float x, float y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);

	virtual void enemyspawn(void);

	inline player* &getPlayer(void) { return _pl; }
	inline vector<character*> &getCharacterVector(void) { return _vCharacter; }
	virtual bulletE* &getBullet(void) { return _ebullet; }

	characterManager();
	virtual ~characterManager();
};