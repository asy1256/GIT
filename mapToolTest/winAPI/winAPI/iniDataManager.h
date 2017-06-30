#pragma once

#include "singletonbase.h"
#include <vector>

using namespace std;

class iniDataManager : public singletonBase<iniDataManager>
{
private:
	struct tagIniData
	{
		const char* subject;
		const char* title;
		const char* body;
	};

	typedef vector<tagIniData> arrIniData;
	typedef vector<tagIniData>::iterator arrIniDataIter;
	typedef vector<arrIniData> arrIniDatas;
	typedef vector<arrIniData>::iterator arrIniDatasIter;

private:
	arrIniDatas _vIniData;

public:
	HRESULT init(void);
	void release(void);

	//저장
	void addData(const char* subject, const char* title, const char* body);
	void iniSave(char* fileName);

	//읽는다
	char* loadDataString(const char* fileName, const char* subject, const char* title);
	int loadDataInteger(const char* fileName, const char* subject, const char* title);

	iniDataManager(void);
	~iniDataManager(void);
};

