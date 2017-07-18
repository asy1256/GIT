#pragma once
#include "gameNode.h"
class titleScene : public gameNode
{
private:
	image* _bimg;

	RECT _button[3];
<<<<<<< HEAD

=======
	
>>>>>>> 435d19df0aabdf37fbd42fb6d81ff6b5a9aece44
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	titleScene();
	virtual ~titleScene();
};