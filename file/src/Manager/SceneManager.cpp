#include <DxLib.h>
#include "../Scene/GameScene/GameScene.h"
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
	if (sceneList_.size() <= 0) return;

	// 配列末尾のポインタを取る
	auto back = sceneList_.back();

	// アクティブなシーンだけ更新する
	back->Update();

	// 次のシーン != 現在のシーン
	if (back->GetNextScene() != back->GetMyScene()) {
		// シーン遷移
		ChangeScene(back->GetNextScene());
	}
}

void SceneManager::Draw() {
	// 非アクティブのシーンも描画する
	for (auto scene : sceneList_) scene->Draw();
}

bool SceneManager::Release() {
	// シーンが無くなるまで解放する
	while (sceneList_.size() > 0) ReleaseScene();

	// リストの後始末
	sceneList_.clear();

	return true;
}

void SceneManager::ReleaseScene() {
	// 現在アクティブなシーンを解放して削除
	sceneList_.back()->Release();
	delete sceneList_.back();

	// リストからも削除
	sceneList_.pop_back();
}

std::list<SceneBase*> SceneManager::GetSceneList() {
	return sceneList_;
}

bool SceneManager::ClassInit() {
	return true;
}

void SceneManager::ParamInit() {
	// 3D 描画機能の有効化
	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);

	// 正面から斜め下に向かったライト
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	ChangeScene(SceneBase::SCENE::GAME);
}

void SceneManager::ChangeScene(SceneBase::SCENE scene) {
	SceneBase* ret = nullptr;

	if (scene != SceneBase::SCENE::PAUSE) {
		while (sceneList_.size() > 0) {
			// 現在アクティブなシーンが目標のシーンなら、関数から抜ける
			if (sceneList_.back()->GetMyScene() == scene) return;

			// 現在のシーンを解放
			else ReleaseScene();
		}

		// シーンを切り替える
		switch (scene) {
		case SceneBase::SCENE::TITLE:
			//ret = new TitleScene();
			break;
		case SceneBase::SCENE::GAME:
			ret = new GameScene();
			break;
		case SceneBase::SCENE::SETTING:
			//ret = new SettingScene();
			break;
		}
	}
	else {
		// ポーズシーン
		//ret = new PauseScene();
	}

	if (ret != nullptr) {
		ret->SystemInit();
		ret->GameInit();
		sceneList_.push_back(ret);
	}
}
