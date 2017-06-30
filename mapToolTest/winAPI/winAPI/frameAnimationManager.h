#pragma once

#include "singletonbase.h"
#include <vector>

class frameAnimation;

class frameAnimationManager : public singletonBase <frameAnimationManager>
{
private:
	typedef vector<frameAnimation*> arrAnimation;
	typedef vector<frameAnimation*>::iterator iterAnimation;

private:
	arrAnimation _vAnimation;
	iterAnimation _viAnimation;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void animationXFrame(image* img, int xFrame, float count, bool rewind);
	void animationYFrame(image* img, int yFrame, float count, bool rewind);

	void animationXFrame(image* img, int xFrame, 
		float count, bool rewind, void* cbFunction);
	void animationYFrame(image* img, int yFrame, 
		float count, bool rewind, void* cbFunction);

	frameAnimationManager(void);
	~frameAnimationManager(void);
};

