#include "StdAfx.h"
#include "saveLoadTest.h"

saveLoadTest::saveLoadTest(void)
{
}

saveLoadTest::~saveLoadTest(void)
{
}

HRESULT saveLoadTest::init(void)
{
	gameNode::init();

	return S_OK;
}

void saveLoadTest::release(void)
{
	gameNode::release();
}

void saveLoadTest::update(void)
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		write();
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		read();
	}
}

void saveLoadTest::render(void)
{

}

//�б�
void saveLoadTest::read(void)
{
	HANDLE file;
	char str[64];
	DWORD read;

	memset(str, 0, 64);
	file = CreateFile("�ȼ���.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, str, 64, &read, NULL);
	CloseHandle(file);

	MessageBox(_hWnd, str, "�ȼ�����?", MB_OK);
}

//����
void saveLoadTest::write(void)
{
	HANDLE file;
	char str[64] = "SAFE_DELETE(���ϸ��� �ȼ���)";
	DWORD write;

	file = CreateFile("�ȼ����� ���� ��������.mkv", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, str, strlen(str), &write, NULL);
	CloseHandle(file);
}
