#pragma once
#include "gameNode.h"
class titleScene : public gameNode
{
private:
	
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	titleScene();
	virtual ~titleScene();
};