#pragma once

#include "gameNode.h"
#include "progressBar.h"
#include <vector>
#include <string>

enum LOADING_KIND
{
	LOADING_KIND_ADDIMAGE_00,			//빈 이미지
	LOADING_KIND_ADDIMAGE_01,			//x, y 없는 이미지
	LOADING_KIND_ADDIMAGE_02,			//x, y 있는 이미지
	LOADING_KIND_ADDFRAMEIMAGE_00,		//프레임 x, y 없는 이미지
	LOADING_KIND_ADDFRAMEIMAGE_01,		//프레임 x, y 있는 이미지
	LOADING_KIND_ADDSOUND,				//사운드 추가
	LOADING_KIND_END
};

//이미지 리소스 구조체
struct tagImageResource
{
	string keyName;
	const char* fileName;
	float x, y;
	int width, height;
	int frameX, frameY;
	bool trans;
	COLORREF transColor;
};
//사운드 리소스 구조체
struct tagsoundResource
{
	string keyName;
	string soundName;
	bool background;
	bool loop;
};

//로딩 아이템
class loadItem
{
	LOADING_KIND _kind;

	tagImageResource _imageResource;
	tagsoundResource _soundResource;

public:
	HRESULT initForImage(string keyName, int width, int height);
	HRESULT initForImage(string keyName, const char* fileName, 
		int width, int height, bool trans, COLORREF transColor);
	HRESULT initForImage(string keyName, const char* fileName, 
		float x, float y, int width, int height, bool trans, COLORREF transColor);
	HRESULT initForFrameImage(string keyName, const char* fileName, 
		int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);
	HRESULT initForFrameImage(string keyName, const char* fileName, 
		float x, float y, int width, int height, int frameX, int frameY, 
		bool trans, COLORREF transColor);
	HRESULT initForSound(string keyName, string soundName, bool background, bool loop);

	void release(void);

	//로딩종류 접근자
	LOADING_KIND getLoadingKind(void) {return _kind;}

	//이미지 리소스 접근자
	tagImageResource getImageResource(void) {return _imageResource;}

	//사운드 리소스 접근자
	tagsoundResource getSoundResource(void) { return _soundResource; }

	loadItem(void);
	virtual ~loadItem(void);
};

//로딩 클래스..
class loading : public gameNode
{
private:
	typedef vector<loadItem*> arrLoadItem;
	typedef vector<loadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem _vLoadItem;

	image* _background;
	progressBar* _loadingBar;

	int _current;

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void loadImage(string keyName, int width, int height);
	void loadImage(string keyName, const char* fileName, 
		int width, int height, bool trans, COLORREF transColor);
	void loadImage(string keyName, const char* fileName, 
		float x, float y, int width, int height, bool trans, COLORREF transColor);
	void loadFrameImage(string keyName, const char* fileName, 
		int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);
	void loadFrameImage(string keyName, const char* fileName, 
		float x, float y, int width, int height, int frameX, int frameY, 
		bool trans, COLORREF transColor);
	void loadSound(string keyName, string soundName, bool background, bool loop);

	BOOL loadNext(void);

	loading(void);
	virtual ~loading(void);
};

