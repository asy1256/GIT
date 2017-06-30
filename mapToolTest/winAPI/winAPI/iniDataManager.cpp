#include "StdAfx.h"
#include "iniDataManager.h"

iniDataManager::iniDataManager(void)
{
}

iniDataManager::~iniDataManager(void)
{
}

HRESULT iniDataManager::init(void)
{
	return S_OK;
}

void iniDataManager::release(void)
{
}

//저장
void iniDataManager::addData(const char* subject, const char* title, const char* body)
{
	tagIniData iniData;
	iniData.subject = subject;
	iniData.title = title;
	iniData.body = body;

	arrIniData vIniData;
	vIniData.push_back(iniData);

	_vIniData.push_back(vIniData);
}

void iniDataManager::iniSave(char* fileName)
{
	char str[256];
	char dir[128];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	for (int i = 0; i < _vIniData.size(); i++)
	{
		arrIniData vData = _vIniData[i];
		WritePrivateProfileString(vData[0].subject,
			vData[0].title, vData[0].body, str);
		vData.clear();
	}

	_vIniData.clear();
}

//읽는다
char* iniDataManager::loadDataString(const char* fileName, const char* subject, const char* title)
{
	char str[256];
	char dir[128];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	char data[64] = {NULL};
	GetPrivateProfileString(subject, title, "", data, 64, str);

	return data;
}

int iniDataManager::loadDataInteger(const char* fileName, const char* subject, const char* title)
{
	char str[256];
	char dir[128];

	ZeroMemory(dir, sizeof(dir));
	sprintf(dir, "\\%s.ini", fileName);

	GetCurrentDirectory(256, str);
	strncat_s(str, 256, dir, 254);

	char data[64] = {NULL};
	return GetPrivateProfileInt(subject, title, 0, str);
}