#pragma once
#include <list>
#include "../Scene/SceneBase.h"

class SceneBase;

class SceneManager {
public:
	static void CreateInstance();
	static SceneManager& GetInstance();

	bool Init();
	void Update();
	void Draw();
	bool Release();
	void ReleaseScene();

	std::list<SceneBase*> GetSceneList();

private:
	static SceneManager* instance_;

	std::list<SceneBase*> sceneList_;

	SceneManager() {}
	~SceneManager() {}

	bool ClassInit();
	void ParamInit();

	void ChangeScene(SceneBase::SCENE);

};
