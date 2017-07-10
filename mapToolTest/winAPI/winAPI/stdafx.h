// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

//================================================
//####### 2017.03.20 헤더파일 인끌루드 #############
//================================================
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN //MFC가 아닌 응용 프로그램의
//경우에는 WIN32_LEAN_AND_MEAN를 정의해 빌드 시간을 단축시킨다.

#include <windows.h> //윈도우 헤더 인클루드

#include <stdio.h> //스탠다드 입출력 헤더파일 인클루드
//printf, scanf, puts, gets.. 등등
#include <tchar.h> //윈도우에서 사용할 문자열 출력 헤더 인클루드
//TextOut, DrawText, wsprintf..등등..

//프로젝트는 기본적으로 유니코드임. 영어, 한글 모두 2바이트...
//MBCS (Multi Byte Character Set) -> 
//기존에 사용하던 멀티바이트 형태의 문자열임
//WBCS (Wide Byte Character Set) ->
//모든 문자열을 2바이트로 처리한다.. 유니코드 기반..

#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "txtDataManager.h"
#include "timeManager.h"
#include "sceneManager.h"
#include "database.h"
#include "utils.h"
#include "frameAnimationManager.h"
#include "soundManager.h"
#include "keyAnimationManager.h"
#include "iniDataManager.h"

//================================================
//############### 2017.03.20 디파인 ###############
//================================================

using namespace MY_UTIL;

#define WINNAME (LPSTR)(TEXT("APIWindow"))
#define WINSTARTX 100
#define WINSTARTY 0
#define WINSIZEX 1024
#define WINSIZEY 768
#define WINCENTER (PointMake(WINSIZEX / 2, WINSIZEY / 2))
#define WINSTYLE WS_CAPTION | WS_SYSMENU

#define SAFE_DELETE(p)			{ if(p) {delete		(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{ if(p) {delete[]	(p); (p) = NULL;}}
#define SAFE_RELEASE(p)			{ if(p) {(p)->release(); (p) = NULL;}}

#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define TXTMANAGER txtDataManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define DATABASE database::getSingleton()
#define FRAMEANIMANAGER frameAnimationManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define KEYANIMANAGER keyAnimationManager::getSingleton()
#define INIMANAGER iniDataManager::getSingleton()

//================================================
//############### 2017.03.20 전역변수 ###############
//================================================
extern HINSTANCE	_hInstance; //프로그램 인스턴스
extern HWND			_hWnd;		//윈도우 핸들
extern POINT		_ptMouse;	//마우스
extern bool			_leftButtonDown;
extern bool			_wheelUp;
extern bool			_wheelDown;
extern int			_TILEMINISIZE;


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include "globalData.h"