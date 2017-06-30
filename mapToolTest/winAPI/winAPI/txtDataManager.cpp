#include "StdAfx.h"
#include "txtDataManager.h"

HRESULT txtDataManager::init(void)
{
	return S_OK;
}

void txtDataManager::release(void)
{
}

//¿˙¿Â
void txtDataManager::save(const char* saveFileName, vector<string> vStr)
{
	HANDLE file;
	char str[128];
	DWORD write;

	strncpy_s(str, 128, vectorArrayCombine(vStr), 126);

	file = CreateFile(saveFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, str, strlen(str), &write, NULL);
	CloseHandle(file);
}

vector<string> txtDataManager::load(const char* loadFileName)
{
	HANDLE file;
	char str[128];
	DWORD read;

	file = CreateFile(loadFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, str, strlen(str), &read, NULL);
	CloseHandle(file);

	return charArraySeparation(str);
}

char* txtDataManager::vectorArrayCombine(vector<string> vArray)
{
	char str[128];

	ZeroMemory(str, sizeof(str));

	for (int i = 0; i < vArray.size(); i++)
	{
		strncat_s(str, 128, vArray[i].c_str(), 126);
		if (i + 1 < vArray.size()) strcat(str, ",");
	}

	return str;
}


vector<string> txtDataManager::charArraySeparation(char charArray[])
{
	vector<string> vArray;

	char* separator = ", +=%$#@";
	char* token;

	token = strtok(charArray, separator);
	vArray.push_back(token);

	while(NULL != (token = strtok(NULL, separator)))
	{
		vArray.push_back(token);
	}

	return vArray;
}
