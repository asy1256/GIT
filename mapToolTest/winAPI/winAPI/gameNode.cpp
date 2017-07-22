#include "StdAfx.h"
#include "gameNode.h"

gameNode::gameNode(void)
	: _managerInit(FALSE)
{
}

gameNode::~gameNode(void)
{
}

HRESULT gameNode::init(void) //init, setup, create...
{
	_hdc = GetDC(_hWnd);
	return S_OK;
}

HRESULT gameNode::init(bool mangerInit) //init, setup, create...
{
	_hdc = GetDC(_hWnd);
	_managerInit = mangerInit;

	if (_managerInit)
	{
		KEYMANAGER->init();			//키 매니져 셋팅
		IMAGEMANAGER->init();		//이미지 매니져 셋팅
		TIMEMANAGER->init();		//타임매니져 셋팅
		TXTMANAGER->init();			//txt매니져 셋팅
		SCENEMANAGER->init();		//씬매니져 셋팅
		DATABASE->init();			//데이터베이스 셋팅
		FRAMEANIMANAGER->init();	//프레임애니 메니져 셋팅
		SOUNDMANAGER->init();		//사운드매니져 셋팅
		KEYANIMANAGER->init();		//키애니 매니져 셋팅
		INIMANAGER->init();			//ini 매니져 셋팅
		ASTAR->init();				//ASTAR 셋팅
	}

	return S_OK;
}

void gameNode::release(void)
{
	if (_managerInit)
	{
		//랜덤 싱글톤 해제
		RND->releaseSingleton();

		//키 매니져 싱글톤 해제
		KEYMANAGER->release();
		KEYMANAGER->releaseSingleton();

		//이미지 매니져 싱글톤 해제
		IMAGEMANAGER->release();
		IMAGEMANAGER->releaseSingleton();

		//타임 매니져 싱글톤 해제
		TIMEMANAGER->release();
		TIMEMANAGER->releaseSingleton();

		//txt매니져 싱글톤 해제
		TXTMANAGER->release();
		TXTMANAGER->releaseSingleton();

		//씬 매니져 싱글톤 해제
		SCENEMANAGER->release();
		SCENEMANAGER->releaseSingleton();

		//db 싱글톤 해제
		DATABASE->release();
		DATABASE->releaseSingleton();

		//프레임애니 매니져 해제
		FRAMEANIMANAGER->release();
		FRAMEANIMANAGER->releaseSingleton();

		//사운드매니져 해제
		SOUNDMANAGER->release();
		SOUNDMANAGER->releaseSingleton();

		//키애니매니져 해제
		KEYANIMANAGER->release();
		KEYANIMANAGER->releaseSingleton();

		//ini매니져 해제
		INIMANAGER->release();
		INIMANAGER->releaseSingleton();

		//ASTAR 해제
		ASTAR->release();
		ASTAR->releaseSingleton();
	}

	ReleaseDC(_hWnd, _hdc);
}

//업데이트 (변경된 값에 대한 수치 반영...) == WM_TIMER
void gameNode::update(void)
{
}

//렌더 (화면 갱신 및 출력 담당한다) == WM_PAINT
void gameNode::render(void)
{
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, 
		WPARAM wParam, LPARAM lParam)
{
	switch ( iMessage )
	{
		case WM_MOUSEMOVE:
			_ptMouse.x = static_cast<float>(LOWORD(lParam));
			_ptMouse.y = static_cast<float>(HIWORD(lParam));
		break;
		case WM_LBUTTONDOWN:
			_leftButtonDown = true;
		break;
		case WM_LBUTTONUP:
			_leftButtonDown = false;
		break;
		case WM_MOUSEWHEEL:
			if ((SHORT)HIWORD(wParam) > 0)
			{
				_wheelUp = true;
			}
			else
			{
				_wheelDown = true;
			}
			break;
		case WM_KEYDOWN:
			switch ( wParam )
			{
				//ESC키.....
				case VK_ESCAPE:
					PostMessage( hWnd, WM_DESTROY, 0, 0 );
				break;
			}
		break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;
	}

	//윈도우 프로시져에서 처리되지 않은 나머지 메시지 처리한다.
	return ( DefWindowProc( hWnd, iMessage, wParam, lParam ) );
}
