#include "stdafx.h"
#include "gameStudy.h"

//================================================
//####### 2017.03.10 전역변수 #############
//================================================
HINSTANCE _hInstance;
HWND _hWnd;
POINT _ptMouse;
bool _leftButtonDown;

gameStudy _gs;

//================================================
//####### 2017.03.10 함수 프로토타입 #############
//================================================
LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
void setWindowSize( int x, int y, int width, int height );
int APIENTRY WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow )
{
	MSG message;
	//윈도우 정보를 셋팅하기 위한 구조체 선언
	WNDCLASS wndClass;

	_hInstance = hInstance;
	wndClass.cbClsExtra = 0; //클래스 여분 메모리
	wndClass.cbWndExtra = 0; //윈도우 여분 메모리
							 //백그라운드 색상 설정
	wndClass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC) WndProc; //윈도우 프로시져
	wndClass.lpszClassName = WINNAME; //클래스이름
	wndClass.lpszMenuName = NULL; //메뉴이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	//윈도우 등록한다.
	RegisterClass( &wndClass );

	//윈도우 생성한다.
	_hWnd = CreateWindow(
		WINNAME, //윈도우 클래스의 이름
		WINNAME, //윈도우 타이틀 바 이름
		WINSTYLE, //윈도우 스타일~~
		WINSTARTX, //윈도우 화면 좌표 x
		WINSTARTY, //윈도우 화면 좌표 y
		WINSIZEX, //윈도우 화면 좌표 width
		WINSIZEY, //윈도우 화면 좌표 height
		NULL, //부모윈도우
		(HMENU) NULL, //메뉴핸들
		hInstance, //인스턴스 윈도우 지정
		NULL //윈도우 및 자식 윈도우를 생성하면 지정해주되 사용하지
			 //않으면... NULL....................
	);

	setWindowSize( WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY );

	//화면에 윈도우 보여준다
	ShowWindow( _hWnd, cmdShow );

	//게임 스터디 초기화
	if (FAILED(_gs.init()))
	{
		//실패하면 바로 종료한다
		return 0;
	}
	
	
	//게임용...
	while (true)
	{
		if (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{	
			TIMEMANAGER->update(60.0f); //초당 60으로 고정
			_gs.update();
			_gs.render();
		}
	}

	/* 잘가라 변태야~!!
	//일반프로그래밍용..
	while ( GetMessage( &message, 0, 0, 0 ) )
	{
		TranslateMessage( &message );
		DispatchMessage( &message );
	}*/

	//게임 스터디 해제
	_gs.release();

	//등록된 윈도우 클래스 제거.....
	UnregisterClass( WINNAME, hInstance );

	return message.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return _gs.MainProc(hWnd, iMessage, wParam, lParam);
}

//윈도우 사이즈 재설정
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc = {0, 0, width, height};
	//POINT pt;

	//사이즈 재설정 함수
	AdjustWindowRect(&rc, WINSTYLE, false); //false는 메뉴가 있는지 없는지...
	SetWindowPos( _hWnd, NULL, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE );
}



