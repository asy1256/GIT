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
	TELEPORTER,
	DOOR,
	L_TABLE,
	W_TABLE,
	K_BULLET,
	B_BULLET,
	R_SHOTGUN,
	GUNUT,
	START_ROOM,
	SHOP_ROOM,
	STON_ROOM,
	WOOD_ROOM,
	BOX_ROOM,
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
	image* _img;						//ī�޶󿡼� ����� �̹���
	image* _sample;						//Ÿ�� ���� �̹���
	image* _book;						//Ÿ�� ����â �̹���
	image* _alhpa;						//�̴ϸ����� ����Ұ� ���ĺ��� �׽�Ʈ �̹���

	tagTile _tile[TILEY][TILEX];		//Ÿ��
	tagTile _startRoom[24][22];			//���۹� ����
	tagfakebutton _tagbutton[6];		//���ú� �±�
	drawtype _nowdraw;					//������ Ÿ�� ��ο� ���
	selecttilekind _nowselecct;			//���� ������ Ÿ��

	POINT _campt, _mouse, _ptadd;		//ī�޶� �̵��� ���� ���콺 ��ǥ����ȭ�� �ʿ��� ����Ʈ ����
	POINT _start;						//�巡�� ���� �׸��� ó�� Ŭ���� ��ǥ �����ϴ� ����Ʈ ����
	
	RECT _dragrc;						//�巡�� ���� �׷��� ��Ʈ
	tagbigsample _bigsample[5][2];		//���� �̹��� �׷��� RC
	tagbigsample _enemysample[2][2];	//���� ���� �׷��� RC
	tagbigsample _option[5];			//���� �ɼ� ��ư
	
	bool _sampleOpen;					//Ÿ�ϼ���â �������� Ȯ��
	int _page;							//���� ��ġ�� �ִ� ���ú� ������
	int _makeroom;						//���� ����� �濡 �ο��� ��ȣ
	float _scroolmax;					//��ũ�ѹ� �ִ�ġ
	float _currentscrool;				//��ũ�ѹ� ��ġ
	float _scroolpos;					//���� ��ũ�ѹ� ��ġ
	
	camera* _cam;						//ī�޶�ƾƾ�
public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual HDC getBackDC(void) { return _img->getMemDC(); }
	virtual void setup(void);
	virtual void keycontrol(void);
	virtual void tileselect(void);
	virtual void pageselect(void);
	virtual void tiledraw(void);
	virtual void mousemove(void);
	virtual void bookup(void);
	virtual void draw(void);
	virtual void selectdraw(void);

	mapTool();
	virtual ~mapTool();
};

