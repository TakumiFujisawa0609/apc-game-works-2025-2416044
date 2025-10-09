#pragma once
#include <chrono>
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
	float GetDeltaTime() const;

private:
	static SceneManager* instance_;

	std::list<SceneBase*> sceneList_;

	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	SceneManager() {}
	~SceneManager() {}

	bool ClassInit();
	void ParamInit();

	SceneBase* ChangeScene(SceneBase::SCENE);

};
