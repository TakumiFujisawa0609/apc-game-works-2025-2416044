#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance() {
	// instance_ 変数が空ポインタならインスタンスを生成
	if (instance_ == nullptr) instance_ = new SceneManager;
}

SceneManager& SceneManager::GetInstance() {
	return *instance_;
}

bool SceneManager::Init() {
	ClassInit();
	ParamInit();
	return true;
}

void SceneManager::Update() {
	// 逆イテレータで for ループ
	for (auto rit = scenes_.rbegin(); rit != scenes_.rend(); rit++) {

		// 逆イテレータの指すポインタ変数が空ポインタと異なるか
		if (*rit != nullptr) {

			// 更新処理
			(*rit)->Update();

			// 最も手前側のシーンだけ処理する
			break;
		}
	}
}

void SceneManager::Draw() {
	// 拡張 for 文で for ループ
	for (auto& scene : scenes_)

		// 描画処理
		scene->Draw();
}

bool SceneManager::Release() {
	return true;
}

int SceneManager::IntScene(SceneBase::SCENE scene) {
	return static_cast<int>(scene);
}

bool SceneManager::ClassInit() {
	return true;
}

void SceneManager::ParamInit() {
	// リザーブ領域をあらかじめ指定
	scenes_.reserve(2);
}

void SceneManager::ChooseScene() {
	auto next = scenes_.back()->GetNextScene();

	if (scenes_.size() == 2) {
		scenes_.pop_back();

		if (scenes_[0]->GetMyScene() == next) return;
	}
	else if (scenes_.back()->GetNextScene() == SceneBase::SCENE::PAUSE) {
		//scene = new PauseScene();
	}

	SceneBase* scene = ChangeScene(next);
}

SceneBase* SceneManager::ChangeScene(SceneBase::SCENE scene) {
	// 現在のシーンを解放
	ReleaseScene();

	// シーンを切り替える（PAUSE 以外）
	switch (scene) {
	case SceneBase::SCENE::TITLE:
		//scene = new TitleScene();
		break;
	case SceneBase::SCENE::GAME:
		//scene = new GameScene();
		break;
	case SceneBase::SCENE::SETTING:
		//scene = new SettingScene();
		break;
	}

	SceneBase* ret = nullptr;

	return ret;
}

void SceneManager::ReleaseScene() {
	// リスト上の末尾のシーンを削除・解放
	scenes_.back()->Release();
	delete scenes_.back();
	scenes_.pop_back();
}
