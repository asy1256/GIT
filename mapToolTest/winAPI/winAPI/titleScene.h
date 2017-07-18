#pragma once
#include "gameNode.h"
class titleScene : public gameNode
{
private:
	image* _bimg;

	RECT _button[3];

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	titleScene();
	virtual ~titleScene();
};