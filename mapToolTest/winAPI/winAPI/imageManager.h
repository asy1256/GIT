#pragma once

#include "singletonbase.h"
#include "image.h"
#include <string>
#include <map>

//====================================================
//####### 2017.03.20 헤더파일 image class #############
//====================================================

class imageManager : public singletonBase<imageManager>
{
public:
	typedef std::map<std::string, image*> mapImageList;
	typedef std::map<std::string, image*>::iterator mapImageIter;

private:
	mapImageList _mImageList;

	//이미지 경로 이름을 파일 이름으로 검색한다
	BOOL getFileName(std::string& strFile);

public:
	//셋업
	HRESULT init(void);

	//해제
	void release(void);

	//키값 설정해서 이미지 추가한다
	image* addImage(std::string strKey, int width, int height);
	image* addImage(std::string strKey, const char* fileName,
		int width, int height, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));
	image* addImage(std::string strKey, const char* fileName, int x, int y, 
		int width, int height, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));
	
	//키값 + 프레임
	image* addFrameImage(std::string strKey, const char* fileName, float x, float y, 
		int width, int height, int frameX, int frameY, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));
	image* addFrameImage(std::string strKey, const char* fileName, 
		int width, int height, int frameX, int frameY, BOOL tran = FALSE, 
		COLORREF transColor = RGB(255, 0, 255));

	//이미지 찾는다
	image* findImage(std::string strKey);

	//이미지 지운다
	BOOL deleteImage(std::string strKey);

	//전체 삭제한다
	BOOL deleteAll(void);

	//일반렌더
	void render(std::string strKey, HDC hdc);
	void render(std::string strKey, HDC hdc, int destX, int destY);
	void render(std::string strKey, HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight);
	void render(std::string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);

	//프레임 렌더
	void frameRender(std::string strKey, HDC hdc, int destX, int destY);
	void frameRender(std::string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//루프 렌더
	void loopRender(std::string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);

	imageManager(void);
	~imageManager(void);
};

