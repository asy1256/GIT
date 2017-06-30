#pragma once

#include "singletonbase.h"
#include <vector>

class txtDataManager : public singletonBase <txtDataManager>
{
public:
	HRESULT init(void);
	void release(void);

	//¿˙¿Â...
	void save(const char* saveFileName, vector<string> vStr);
	char* txtDataManager::vectorArrayCombine(vector<string> vArray);
	
	vector<string> load(const char* loadFileName);
	vector<string> charArraySeparation(char charArray[]);

public:
	txtDataManager(void) {};
	~txtDataManager(void) {};
};

