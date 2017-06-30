#include "StdAfx.h"
#include "imageManager.h"


imageManager::imageManager(void)
{
}


imageManager::~imageManager(void)
{
}

//셋업
HRESULT imageManager::init(void)
{
	return S_OK;
}

//해제
void imageManager::release(void)
{
	deleteAll();
}

//키값 설정해서 이미지 추가한다
image* imageManager::addImage(std::string strKey, int width, int height)
{
	//추가하려는 키 값의 이미지가 있냐?
	image* img = findImage(strKey);

	//추가하려는 이미지가 있으면 추가하지 않고 대상 이미지 리턴...
	if (img) return img;

	//없으면 이미지 생성
	img = new image;

	//이미지 셋팅하고 이미지가 초기화에 문제가 있으면 NULL 리턴한다
	if (FAILED(img->init(width, height)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//생성된 이미지를 맵에 추가한다
	_mImageList.insert(make_pair(strKey, img));

	//해당 이미지 리턴
	return img;
}

image* imageManager::addImage(std::string strKey, const char* fileName,
	int width, int height, BOOL tran, COLORREF transColor)
{
	//추가하려는 키 값의 이미지가 있냐?
	image* img = findImage(strKey);

	//추가하려는 이미지가 있으면 추가하지 않고 대상 이미지 리턴...
	if (img) return img;

	//없으면 이미지 생성
	img = new image;

	//이미지 셋팅하고 이미지가 초기화에 문제가 있으면 NULL 리턴한다
	if (FAILED(img->init(fileName, width, height, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//생성된 이미지를 맵에 추가한다
	_mImageList.insert(make_pair(strKey, img));

	//해당 이미지 리턴
	return img;
}

image* imageManager::addImage(std::string strKey, const char* fileName, int x, int y, 
	int width, int height, BOOL tran, COLORREF transColor)
{
	//추가하려는 키 값의 이미지가 있냐?
	image* img = findImage(strKey);

	//추가하려는 이미지가 있으면 추가하지 않고 대상 이미지 리턴...
	if (img) return img;

	//없으면 이미지 생성
	img = new image;

	//이미지 셋팅하고 이미지가 초기화에 문제가 있으면 NULL 리턴한다
	if (FAILED(img->init(fileName, (float)x, (float)y, width, height, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//생성된 이미지를 맵에 추가한다
	_mImageList.insert(make_pair(strKey, img));

	//해당 이미지 리턴
	return img;
}
	
//키값 + 프레임
image* imageManager::addFrameImage(std::string strKey, const char* fileName, float x, float y, 
	int width, int height, int frameX, int frameY, BOOL tran, COLORREF transColor)
{
	//추가하려는 키 값의 이미지가 있냐?
	image* img = findImage(strKey);

	//추가하려는 이미지가 있으면 추가하지 않고 대상 이미지 리턴...
	if (img) return img;

	//없으면 이미지 생성
	img = new image;

	//이미지 셋팅하고 이미지가 초기화에 문제가 있으면 NULL 리턴한다
	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//생성된 이미지를 맵에 추가한다
	_mImageList.insert(make_pair(strKey, img));

	//해당 이미지 리턴
	return img;
}

image* imageManager::addFrameImage(std::string strKey, const char* fileName, 
	int width, int height, int frameX, int frameY, BOOL tran, COLORREF transColor)
{
	//추가하려는 키 값의 이미지가 있냐?
	image* img = findImage(strKey);

	//추가하려는 이미지가 있으면 추가하지 않고 대상 이미지 리턴...
	if (img) return img;

	//없으면 이미지 생성
	img = new image;

	//이미지 셋팅하고 이미지가 초기화에 문제가 있으면 NULL 리턴한다
	if (FAILED(img->init(fileName, width, height, frameX, frameY, tran, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	//생성된 이미지를 맵에 추가한다
	_mImageList.insert(make_pair(strKey, img));

	//해당 이미지 리턴
	return img;
}

//일반렌더
void imageManager::render(std::string strKey, HDC hdc)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc); 
}

void imageManager::render(std::string strKey, HDC hdc, int destX, int destY)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc, destX, destY); 
}

void imageManager::render(std::string strKey, HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc, sourX, sourY, sourWidth, sourHeight); 
}

void imageManager::render(std::string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	image* img = findImage(strKey);
	if(img) img->render(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight); 
}

//프레임 렌더
void imageManager::frameRender(std::string strKey, HDC hdc, int destX, int destY)
{
	image* img = findImage(strKey);
	if(img) img->frameRender(hdc, destX, destY); 
}

void imageManager::frameRender(std::string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = findImage(strKey);
	if(img) img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}

//루프 렌더
void imageManager::loopRender(std::string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	image* img = findImage(strKey);
	if(img) img->loopRender(hdc, drawArea, offsetX, offsetY); 
}

//이미지 찾는다
image* imageManager::findImage(std::string strKey)
{
	//해당키 검색하고..
	mapImageIter key = _mImageList.find(strKey);

	//해당 키값을 찾았으면...
	if (key != _mImageList.end())
	{
		return key->second;
	}

	return NULL;
}

//이미지 지운다
BOOL imageManager::deleteImage(std::string strKey)
{
	//해당키 검색하고..
	mapImageIter key = _mImageList.find(strKey);

	//해당 키값을 찾으면..
	if (key != _mImageList.end())
	{
		//이미지 해제
		key->second->release();

		//메모리 해제
		SAFE_DELETE(key->second);

		//맵리스트에서 삭제
		_mImageList.erase(key);

		return TRUE;
	}
	return FALSE;
}

//전체 삭제한다
BOOL imageManager::deleteAll(void)
{
	//해당키 검색하고..
	mapImageIter iter = _mImageList.begin();

	//맵 전체를 돌면서 지워준다
	for (; iter != _mImageList.end();)
	{
		//지워지면 반복자 증가시키지 않는다
		if (iter->second != NULL)
		{
			iter->second->release();
			delete iter->second;
			iter = _mImageList.erase(iter);
		}
		else 
		{
			//반복자 증가..
			++iter;
		}
	}

	_mImageList.clear();

	return TRUE;
}

//이미지 경로 이름을 파일 이름으로 검색한다
BOOL imageManager::getFileName(std::string& strFile)
{
	if (strFile.empty()) return false;

	//스트링 포지션 위치 값
	size_t pos;

	//경로 자른다
	//뒤에서 부터 '/'문자 만나는 지점 찾아서 pos에 대입한다
	pos = strFile.find_first_of("/");

	//찾았으면...
	if (pos != std::string::npos)
	{
		//잘라낸다
		strFile = strFile.substr(pos + 1, strFile.length());
	}
	else //못 찾았으면... '\'로 찾는다 
	{
		//다시 찾는다
		pos = strFile.find_last_of( "\\" );

		if (pos != std::string::npos)
		{
			//잘라낸다
			strFile = strFile.substr(pos + 1, strFile.length());
		}
	}

	//확장자 짜르기
	pos = strFile.find(".");

	if (pos != std::string::npos) strFile = strFile.substr(0, pos);
	else return FALSE;

	return TRUE;
}

