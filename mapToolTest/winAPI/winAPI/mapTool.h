#pragma once

#include "gameNode.h"
#include "camera.h"

class mapTool : public gameNode
{
private:
	tagTile _tile[TILEY][TILEX];
	image* _img; //카메라에서 사용할 친구
	image* _sample; //타일 샘플
	image* _book; //타일 선택창
	image* _alhpa; //미니맵으로 사용할거 알파블랜더 테스트
	POINT _campt, _mouse, _ptadd; //카메라 이동에 의한 마우스 좌표동기화에 필요한 변수
	POINT _start, _end; //선택한 타일 구간의 배열 받으려고 했는데 기각 end는 삭제예정
	RECT _dragrc; //선택한 타일 구간 확인할 렉트
	bool _sampleOpen; //타일선택창 열었는지 확인
	
	camera* _cam;
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual HDC getBackDC(void) { return _img->getMemDC(); }
	virtual void setup(void);
	virtual void keycontrol(void);
	virtual void tileselect(void);
	virtual void tiledraw(void);
	virtual void mousemove(void);
	virtual void draw(void);
	virtual void selectdraw(void);

	mapTool();
	virtual ~mapTool();
};

