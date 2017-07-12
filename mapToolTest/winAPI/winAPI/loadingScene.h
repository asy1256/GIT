#pragma once

#include "gameNode.h"
#include "loading.h"

class loadingScene : public gameNode
{
private:
	loading* _loading;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	loadingScene();
	virtual ~loadingScene();
};

