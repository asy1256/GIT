#pragma once

#include "gameNode.h"
#include "camera.h"

class mapTool : public gameNode
{
private:
	tagTile _tile[TILEY][TILEX];
	image* _img; //ī�޶󿡼� ����� ģ��
	image* _sample; //Ÿ�� ����
	image* _book; //Ÿ�� ����â
	image* _alhpa; //�̴ϸ����� ����Ұ� ���ĺ��� �׽�Ʈ
	POINT _campt, _mouse, _ptadd; //ī�޶� �̵��� ���� ���콺 ��ǥ����ȭ�� �ʿ��� ����
	POINT _start, _end; //������ Ÿ�� ������ �迭 �������� �ߴµ� �Ⱒ end�� ��������
	RECT _dragrc; //������ Ÿ�� ���� Ȯ���� ��Ʈ
	bool _sampleOpen; //Ÿ�ϼ���â �������� Ȯ��
	
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

