#pragma once
#include <vector>
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

	int IntScene(SceneBase::SCENE);

private:
	static SceneManager* instance_;

	SceneBase::SCENE nowScene_;
	std::vector<SceneBase*> scenes_;

	SceneManager() {}
	~SceneManager() {}

	bool ClassInit();
	void ParamInit();
	void ChooseScene();
	SceneBase* ChangeScene(SceneBase::SCENE);
	void ReleaseScene();

};
