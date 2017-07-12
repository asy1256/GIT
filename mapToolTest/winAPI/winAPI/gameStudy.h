#pragma once

#include "gamenode.h"
#include "loadingScene.h"
#include "titleScene.h"
#include "mapTool.h"
#include "gungeonScene.h"

class gameStudy : public gameNode
{
private:

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	gameStudy(void);
	virtual ~gameStudy(void);
};

