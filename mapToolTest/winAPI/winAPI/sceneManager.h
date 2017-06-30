#pragma once

#include "singletonbase.h"
#include <string>
#include <map>

class gameNode;

//=====================================================
//####### 2017.03.20 헤더파일 sceneManager #############
//=====================================================

using namespace std;

class sceneManager : public singletonBase <sceneManager>
{
public:
	typedef map<string, gameNode*> mapSceneList;
	typedef map<string, gameNode*>::iterator mapSceneIter;

private:
	static gameNode* _currentScene; //현재 씬
	static gameNode* _readyScene; //교체준비중인 씬

	mapSceneList _mSceneList; //씬리스트

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//씬 추가한다
	gameNode* addScene(string sceneName, gameNode* scene);

	//씬 체인지
	HRESULT changeScene(string sceneName);

	sceneManager(void) {};
	virtual ~sceneManager(void) {};

	//friend DWORD CALLBACK loadingThread(LPVOID prc);
};

