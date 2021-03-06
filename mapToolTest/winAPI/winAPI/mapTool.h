#pragma once

#include "gameNode.h"
#include "camera.h"

struct tagfakebutton
{
	RECT rc;
	bool select;
	bool onmouse;
};

enum selecttilekind
{
	S_NONE,
	S_FLOOR,
	W_FLOOR,
	S_WALL,
	S_WALL_PIECE,
	W_WALL,
	W_WALL_PIECE,
	CARPT_A,
	CARPT_B,
	CARPT_A_PIECE,
	CARPT_B_PIECE,
	W_BARREL,
	B_BARREL,
	O_TP,
	O_DOOR,
	L_TABLE,
	W_TABLE,
	K_BULLET,
	B_BULLET,
	KING_BULLET,
	R_SHOTGUN,
	GUNUT,
	START_ROOM,
	SHOP_ROOM,
	STON_ROOM,
	WOOD_ROOM,
	T_ERASE,
	OB_ERASE,
	R_ERASE
};

enum drawtype
{
	D_NONE,
	STICK,
	SAMIAUTO,
	AUTO
};

struct tagbigsample
{
	selecttilekind kind;
	drawtype type;
	RECT rc;
	bool select;
};

class mapTool : public gameNode
{
private:
	image* _cimg;						//카메라에서 사용할 이미지
	image* _miniimg;					//미니맵그릴때 사용할 이미지
	image* _sample;						//타일 샘플 이미지
	image* _book;						//타일 선택창 이미지
	image* _alhpa;						//미니맵으로 사용할거 알파블랜더 테스트 이미지
	image* _minimapcase;
	image* _black;
	image* _ok;
	image* _no;
	image* _cover;
	image* _bigsamplecase;
	image* _bosscase;
	image* _mapoption;
	image* _bigsampleimg;
	image* _enemysampleimg;
	image* _bosssampleimg;
	image* _roomsampleimg;
	image* _booktag;
	image* _scrollbardown;
	image* _scrollbar;
	image* _barrels;
	image* _enemys;
	image* _boss;
private:

	tagTile _tile[TILEY][TILEX];		//타일
	tagfakebutton _tagbutton[6];		//샘플북 태그
	drawtype _nowdraw;					//선택한 타일 드로우 방식
	selecttilekind _nowselecct;			//현재 선택한 타일

	POINT _campt;						//카메라 이동에 의한 마우스 좌표동기화에 필요한 포인트 변수
	POINT _start;						//드래그 영역 그릴때 처음 클릭한 좌표 저장하는 포인트 변수
	POINT _minimappt;					//미니맵 드래그 해서 확인할용도
	POINT _basept;

	RECT _dragrc;						//드래그 영역 그려줄 렉트
	tagbigsample _bigsample[5][2];		//샘플 이미지 그려줄 RC
	tagbigsample _enemysample[2][2];	//적군 샘플 그려줄 RC
	tagbigsample _bosssample;			//보스 샘플 그려줄 RC
	tagbigsample _roomsample[2][2];		//방 샘플 그려줄 RC
	tagbigsample _option[5];			//맵툴 옵션 버튼

	bool _sampleOpen;					//타일선택창 열었는지 확인
	bool _mapOpen;						//미니맵 열었는지 확인
	bool _minimapdraw;
	int _page;							//지금 펼치고 있는 샘플북 페이지
	int _makeroom;						//지금 만드는 방에 부여될 번호
	float _scroolmax;					//스크롤바 최대치
	float _currentscrool;				//스크롤바 수치
	float _scroolpos;					//현재 스크롤바 위치

	camera* _cam;						//카메라아아아
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual HDC getBackDC(void) { return _cimg->getMemDC(); }
	virtual void setup(void);
	virtual void keycontrol(void);
	virtual void tileselect(void);
	virtual void pageselect(void);
	virtual void tiledraw(void);
	virtual void mousemove(void);
	virtual void bookup(void);
	virtual void draw(void);
	virtual void minidraw(void);
	virtual void selectdraw(void);

	mapTool();
	virtual ~mapTool();
};

