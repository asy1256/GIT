#include "StdAfx.h"
#include "image.h"
#include "animation.h"

#pragma comment(lib, "msimg32.lib")

image::image(void)
: _imageInfo(NULL),
_fileName(NULL),
_tran(FALSE),
_transColor(RGB(0, 0, 0))
{
}

image::~image(void)
{
}

//빈 비트맵 만든다
HRESULT image::init(int width, int height)
{
	//또 초기화 할때 대비...
	if (_imageInfo != NULL) release();

	//DC얻어온다
	HDC hdc = GetDC(_hWnd);

	_imageInfo						= new IMAGE_INFO;
	_imageInfo->loadType			= LOAD_EMPTY;
	_imageInfo->resID				= 0;
	_imageInfo->hMemDC				= CreateCompatibleDC(hdc);
	//비트맵을 출력하기 위해서 메모리 DC로 만들어 준다 (화면DC는 느림)

	_imageInfo->hBit				= (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit				= (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x					= 0;
	_imageInfo->y					= 0;
	_imageInfo->width				= width;
	_imageInfo->height				= height;

	_fileName = NULL;

	//얻은 DC해제한다
	ReleaseDC(_hWnd, hdc);

	//투명컬러 셋팅한다
	_tran = FALSE;
	_transColor = RGB(0, 0, 0);

	//리소스 생성 실패하면...
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

//이미지 설정 (파일로부터 읽는다)
HRESULT image::init(const char* fileName, int width, int height, BOOL tran, COLORREF transColor)
{
	//파일 경로가 잘못 되었다면
	if (fileName == NULL) return E_FAIL;

	//또 초기화 할때 대비...
	if (_imageInfo != NULL) release();

	//DC얻어온다
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 생성
	_imageInfo						= new IMAGE_INFO;
	_imageInfo->loadType			= LOAD_RESOURCE;
	_imageInfo->resID				= 0;
	_imageInfo->hMemDC				= CreateCompatibleDC(hdc);
	//비트맵을 출력하기 위해서 메모리 DC로 만들어 준다 (화면DC는 느림)

	_imageInfo->hBit				= (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit				= (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x					= 0;
	_imageInfo->y					= 0;
	_imageInfo->width				= width;
	_imageInfo->height				= height;


	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//얻은 DC해제한다
	ReleaseDC(_hWnd, hdc);

	//투명컬러 셋팅한다
	_tran = tran;
	_transColor = transColor;

	//리소스 생성 실패하면...
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT image::init(const char* fileName, float x, float y, int width, int height, BOOL tran, COLORREF transColor)
{
	//파일 경로가 잘못 되었다면
	if (fileName == NULL) return E_FAIL;

	//또 초기화 할때 대비...
	if (_imageInfo != NULL) release();

	//DC얻어온다
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 생성
	_imageInfo						= new IMAGE_INFO;
	_imageInfo->loadType			= LOAD_RESOURCE;
	_imageInfo->resID				= 0;
	_imageInfo->hMemDC				= CreateCompatibleDC(hdc);
	//비트맵을 출력하기 위해서 메모리 DC로 만들어 준다 (화면DC는 느림)

	_imageInfo->hBit				= (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit				= (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x					= x - (width / 2);
	_imageInfo->y					= y - (height / 2);
	_imageInfo->width				= width;
	_imageInfo->height				= height;

	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//얻은 DC해제한다
	ReleaseDC(_hWnd, hdc);

	//투명컬러 셋팅한다
	_tran = tran;
	_transColor = transColor;

	//리소스 생성 실패하면...
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

//이미지 + 프레임 파일 초기하
HRESULT image::init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL tran, COLORREF transColor)
{
	//파일 경로가 잘못 되었다면
	if (fileName == NULL) return E_FAIL;

	//또 초기화 할때 대비...
	if (_imageInfo != NULL) release();

	//DC얻어온다
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 생성
	_imageInfo						= new IMAGE_INFO;
	_imageInfo->loadType			= LOAD_RESOURCE;
	_imageInfo->resID				= 0;
	_imageInfo->hMemDC				= CreateCompatibleDC(hdc);
	//비트맵을 출력하기 위해서 메모리 DC로 만들어 준다 (화면DC는 느림)

	_imageInfo->hBit				= (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit				= (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x					= x - (width / 2);
	_imageInfo->y					= y - (height / 2);
	_imageInfo->currentFrameX		= 0;
	_imageInfo->currentFrameY		= 0;
	_imageInfo->maxFrameX			= frameX - 1;
	_imageInfo->maxFrameY			= frameY - 1;
	_imageInfo->width				= width;
	_imageInfo->height				= height;
	_imageInfo->frameWidth			= width / frameX;
	_imageInfo->frameHeight			= height / frameY;

	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//얻은 DC해제한다
	ReleaseDC(_hWnd, hdc);

	//투명컬러 셋팅한다
	_tran = tran;
	_transColor = transColor;

	//리소스 생성 실패하면...
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, int frameX, int frameY, BOOL tran, COLORREF transColor)
{
	//파일 경로가 잘못 되었다면
	if (fileName == NULL) return E_FAIL;

	//또 초기화 할때 대비...
	if (_imageInfo != NULL) release();

	//DC얻어온다
	HDC hdc = GetDC(_hWnd);

	//이미지 정보 생성
	_imageInfo						= new IMAGE_INFO;
	_imageInfo->loadType			= LOAD_RESOURCE;
	_imageInfo->resID				= 0;
	_imageInfo->hMemDC				= CreateCompatibleDC(hdc);
	//비트맵을 출력하기 위해서 메모리 DC로 만들어 준다 (화면DC는 느림)

	_imageInfo->hBit				= (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit				= (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x					= 0;
	_imageInfo->y					= 0;
	_imageInfo->currentFrameX		= 0;
	_imageInfo->currentFrameY		= 0;
	_imageInfo->maxFrameX			= frameX - 1;
	_imageInfo->maxFrameY			= frameY - 1;
	_imageInfo->width				= width;
	_imageInfo->height				= height;
	_imageInfo->frameWidth			= width / frameX;
	_imageInfo->frameHeight			= height / frameY;

	//파일 경로 이름 복사한다
	int len = strlen(fileName);
	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);

	//얻은 DC해제한다
	ReleaseDC(_hWnd, hdc);

	//투명컬러 셋팅한다
	_tran = tran;
	_transColor = transColor;

	//리소스 생성 실패하면...
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}

	return S_OK;
}

//삭제
void image::release(void)
{
	if (_imageInfo)
	{
		//이미지 삭제한다
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//포인터 삭제한다 (파일 이름)
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);

		//투명 컬러 초기화
		_tran = false;
		_transColor = RGB(0, 0, 0);
	}
}

//투명 키 셋팅
void image::setTransColor(BOOL tran, COLORREF transColor)
{
	_tran = tran;
	_transColor = transColor;
}

//렌더
void image::render(HDC hdc)
{
	if (_tran)
	{
		//TransparentBlt
		GdiTransparentBlt(
			hdc,					//복사할 장소의 DC
			(int)_imageInfo->x,		//복사될 좌표 시작점 x
			(int)_imageInfo->y,		//복사될 좌표 시작점 y
			_imageInfo->width,		//복사될 크기 width
			_imageInfo->height,		//복사될 크기 height
			_imageInfo->hMemDC,		//복사대상 DC
			0,						//복사 시작지점 x
			0,						//복사 시작지점 y
			_imageInfo->width,		//복사될 영역지정한다 width
			_imageInfo->height,		//복사될 영역지정한다 height
			_transColor				//복사에서 제외할 색상 지정 (컬러키)
			);
	}
	else
	{
		BitBlt(hdc, (int)_imageInfo->x, (int)_imageInfo->y, 
			_imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
		//bitblt DC간의 영역끼리 서로 고속복사를 해준다
	}
}

void image::render(HDC hdc, int destX, int destY)
{
	if (_tran)
	{
		//TransparentBlt
		GdiTransparentBlt(
			hdc,					//복사할 장소의 DC
			destX,					//복사될 좌표 시작점 x
			destY,					//복사될 좌표 시작점 y
			_imageInfo->width,		//복사될 크기 width
			_imageInfo->height,		//복사될 크기 height
			_imageInfo->hMemDC,		//복사대상 DC
			0,						//복사 시작지점 x
			0,						//복사 시작지점 y
			_imageInfo->width,		//복사될 영역지정한다 width
			_imageInfo->height,		//복사될 영역지정한다 height
			_transColor				//복사에서 제외할 색상 지정 (컬러키)
			);
	}
	else
	{
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
		//bitblt DC간의 영역끼리 서로 고속복사를 해준다
	}
}

void image::render(HDC hdc, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_tran)
	{
		//TransparentBlt
		GdiTransparentBlt(
			hdc,					//복사할 장소의 DC
			(int)_imageInfo->x,		//복사될 좌표 시작점 x
			(int)_imageInfo->y,		//복사될 좌표 시작점 y
			_imageInfo->width,		//복사될 크기 width
			_imageInfo->height,		//복사될 크기 height
			_imageInfo->hMemDC,		//복사대상 DC
			sourX,					//복사 시작지점 x
			sourY,					//복사 시작지점 y
			sourWidth,				//복사될 영역지정한다 width
			sourHeight,				//복사될 영역지정한다 height
			_transColor				//복사에서 제외할 색상 지정 (컬러키)
			);
	}
	else
	{
		BitBlt(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
		//bitblt DC간의 영역끼리 서로 고속복사를 해준다
	}
}

void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_tran)
	{
		//TransparentBlt
		GdiTransparentBlt(
			hdc,					//복사할 장소의 DC
			destX,					//복사될 좌표 시작점 x
			destY,					//복사될 좌표 시작점 y
			sourWidth,				//복사될 크기 width
			sourHeight,				//복사될 크기 height
			_imageInfo->hMemDC,		//복사대상 DC
			sourX,					//복사 시작지점 x
			sourY,					//복사 시작지점 y
			sourWidth,				//복사될 영역지정한다 width
			sourHeight,				//복사될 영역지정한다 height
			_transColor				//복사에서 제외할 색상 지정 (컬러키)
			);
	}
	else
	{
		BitBlt(hdc, destX, destY, sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
		//bitblt DC간의 영역끼리 서로 고속복사를 해준다
	}
}

//프레임 렌더
void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_tran)
	{
		//TransparentBlt
		GdiTransparentBlt(hdc, destX, destY,					
			_imageInfo->frameWidth, _imageInfo->frameHeight,				
			_imageInfo->hMemDC,		
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 
			_imageInfo->currentFrameY * _imageInfo->frameHeight,					
			_imageInfo->frameWidth,				
			_imageInfo->frameHeight,				
			_transColor				
			);
	}
	else
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, 
			_imageInfo->frameHeight,
			_imageInfo->hMemDC, 
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			SRCCOPY);
		//bitblt DC간의 영역끼리 서로 고속복사를 해준다
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (currentFrameX > _imageInfo->maxFrameX) _imageInfo->currentFrameX = _imageInfo->maxFrameX;
	if (currentFrameY > _imageInfo->maxFrameY) _imageInfo->currentFrameY = _imageInfo->maxFrameY;

	if (_tran)
	{
		//TransparentBlt
		GdiTransparentBlt(hdc, destX, destY,					
			_imageInfo->frameWidth, _imageInfo->frameHeight,				
			_imageInfo->hMemDC,		
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 
			_imageInfo->currentFrameY * _imageInfo->frameHeight,					
			_imageInfo->frameWidth,				
			_imageInfo->frameHeight,				
			_transColor				
			);
	}
	else
	{
		BitBlt(hdc, destX, destY, _imageInfo->frameWidth, 
			_imageInfo->frameHeight,
			_imageInfo->hMemDC, 
			_imageInfo->currentFrameX * _imageInfo->frameWidth, 
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			SRCCOPY);
		//bitblt DC간의 영역끼리 서로 고속복사를 해준다
	}
}

//백그라운드 루프!!
void image::loopRender(HDC hdc, const LPRECT drawArea,
	int offsetX, int offsetY)
{
	//옵셋 값이 음수인 경우에 보정
	if (offsetX < 0) offsetX = _imageInfo->width + 
		(offsetX % _imageInfo->width);
	if (offsetY < 0) offsetY = _imageInfo->height + 
		(offsetY % _imageInfo->height);

	//그리려 하는 소스의 영역임.
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//그려지는 DC 영역
	RECT rcDest;

	//그리려 하는 전체 영역
	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//세로 루핑
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//소스 영역 높이 계산한다!
		rcSour.top = (y + offsetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//소스 영역이 그리는 영역을 넘어갔으면...
		if (y + sourHeight > drawAreaH)
		{
			//넘어간 양만큼 바텀값 올린다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//그려지는 영역임
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//가로 루핑
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//소스 영역 가로 계산한다!
			rcSour.left = (x + offsetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			//소스 영역이 그리는 영역을 넘어갔으면...
			if (x + sourWidth > drawAreaW)
			{
				//넘어간 양만큼 라이트값을 왼쪽으로~~
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//그려지는 영역임
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//그린다
			render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top, rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}

	}

}

//애니렌더
void image::aniRender(HDC hdc, int destX, int destY, animation* ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y,
		ani->getFrameWidth(), ani->getFrameHeight());
}

//알파블랜더
void image::alphaRender(HDC hdc, int destX, int destY)
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 130;
	bf.AlphaFormat = 0;

	if (_tran)
	{
		AlphaBlend(
			hdc,					//복사할 장소의 DC
			destX,					//복사될 좌표 시작점 x
			destY,					//복사될 좌표 시작점 y
			_imageInfo->width,		//복사될 크기 width
			_imageInfo->height,		//복사될 크기 height
			_imageInfo->hMemDC,					//복사대상 DC
			0,						//복사 시작지점 x
			0,						//복사 시작지점 y
			_imageInfo->width,		//복사될 영역지정한다 width
			_imageInfo->height,		//복사될 영역지정한다 height
			bf						//알파값
		);
	}
	else
	{
		BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
		//bitblt DC간의 영역끼리 서로 고속복사를 해준다
	}
}