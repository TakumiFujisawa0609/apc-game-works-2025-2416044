#pragma once
#include <chrono>
#include <list>
#include "../Scene/SceneBase.h"

class SceneBase;
class Fader;

class SceneManager {
private:
	static SceneManager* instance_;

	SceneManager() {}
	~SceneManager() {}

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;

public:
	static void CreateInstance() { if (instance_ == nullptr) instance_ = new SceneManager; }
	static SceneManager& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

	bool Init();
	void Update();
	void Draw();
	bool Release();
	void ReleaseScene();

	std::list<SceneBase*> GetSceneList();
	Fader* GetFaderPtr();
	float GetDeltaTime() const;

	void SetNextStartStage(unsigned int);
	bool IsPause() const;
	bool PrevPause() const;

	void SetLastScore(unsigned int);
	unsigned int GetLastScore() const;

private:
	std::list<SceneBase*> sceneList_;
	Fader* fader_;

	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	bool ClassInit();
	void ParamInit();

	void ChangeScene(SceneBase::SCENE);
	bool Fade();
	void DoChangeScene(SceneBase::SCENE);

	unsigned int nextStartStage_;
	bool isPause_;
	bool prevPause_;
	SceneBase::SCENE waitSceneId_;
	unsigned int lastScore_;

};
