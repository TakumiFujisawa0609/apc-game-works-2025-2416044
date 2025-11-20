#include <DxLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene/TitleScene.h"
#include "../Scene/GameScene/GameScene.h"
#include "../Scene/Pause/Pause.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

bool SceneManager::Init() {
	ClassInit();
	ParamInit();
	return true;
}

void SceneManager::Update() {
	// 中身が無い状態で動かすと色々危ないので
	if (sceneList_.size() <= 0) return;

	// デルタタイム更新
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (!Fade()) {
		// 配列末尾のポインタを取る
		auto back = sceneList_.back();

		// アクティブなシーンだけ更新する
		back->Update();

		back->LateUpdate();

		prevPause_ = isPause_;
		isPause_ = back->GetMyScene() == SceneBase::SCENE::PAUSE ?
			true : false;

		auto next = back->GetNextScene();

		// 次のシーン != 現在のシーン
		if (back->GetMyScene() != next) {
			// シーン遷移
			ChangeScene(next);
		}
	}
}

void SceneManager::Draw() {
	// 非アクティブのシーンも描画する
	for (auto scene : sceneList_) {
		scene->Draw();
	}

	// フェーダー等の画面エフェクトを挟む
	fader_->Draw();

	for (auto scene : sceneList_) {
		scene->DrawUI();
	}
}

bool SceneManager::Release() {
	// シーンが無くなるまで解放する
	while (sceneList_.size() > 0) ReleaseScene();

	delete fader_;

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

std::list<SceneBase*> SceneManager::GetSceneList() { return sceneList_; }

Fader* SceneManager::GetFaderPtr() { return fader_; }

float SceneManager::GetDeltaTime() const { return deltaTime_; }

bool SceneManager::IsPause() const { return isPause_; }

bool SceneManager::PrevPause() const { return prevPause_; }

bool SceneManager::ClassInit() {
	fader_ = new Fader();

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
	ChangeLightTypeDir({ 0.0F, -1.0F, 0.8F });

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	DoChangeScene(SceneBase::SCENE::TITLE);
}

void SceneManager::ChangeScene(SceneBase::SCENE scene) {
	if (scene == SceneBase::SCENE::PAUSE ||
		sceneList_.back()->GetMyScene() == SceneBase::SCENE::PAUSE && scene == SceneBase::SCENE::GAME) {

		DoChangeScene(scene);
		return;
	}

	waitSceneId_ = scene;

	fader_->SetFadeMode(Fader::FADE_MODE::FADE_OUT, 60U, 0U, 60U);
}

bool SceneManager::Fade() {
	// 現在のフェードモードを取得
	auto fmode = fader_->GetFadeMode();

	switch (fmode) {
	case Fader::FADE_MODE::FADE_OUT:
		// 処理が完了次第
		if (fader_->IsFadeEnd()) {
			// シーンを切り替える
			DoChangeScene(waitSceneId_);

			// フェードモードをフェードインに
			fader_->SetFadeMode(Fader::FADE_MODE::FADE_IN, 60U, 120U);
		}
		break;
	case Fader::FADE_MODE::FADE_IN:
		// 処理が完了次第
		if (fader_->IsFadeEnd()) {
			// フェードモードを無しに
			fader_->SetFadeMode(Fader::FADE_MODE::NONE, 0U);
		}
		break;
	default:
		// フェードモードが無しの場合、
		// この関数の後の処理に移行させるためにfalseを返す
		return false;
	}

	// 例外的な処理は、ここで記述する
	
	// 特定のシーンにおいて、フェード中や待機中でも処理が流れるように変更
	if (sceneList_.back()->GetMyScene() == SceneBase::SCENE::GAME &&
		fader_->GetFadeMode() == Fader::FADE_MODE::FADE_IN &&
		(fader_->GetNowProc() == Fader::PROC::FADE || fader_->GetNowProc() == Fader::PROC::WAIT)) {
		return false;
	}

	return true;
}

void SceneManager::DoChangeScene(SceneBase::SCENE scene) {
	SceneBase* ret = nullptr;

	if (scene != SceneBase::SCENE::PAUSE) {
		while (sceneList_.size() > 0) {
			// 現在アクティブなシーンが目標のシーンなら、関数から抜ける
			if (sceneList_.back()->GetMyScene() == scene) {
				sceneList_.back()->SetScene(scene);
				return;
			}

			// 現在のシーンを解放
			else ReleaseScene();
		}

		// シーンを切り替える
		switch (scene) {
		case SceneBase::SCENE::TITLE:
			ret = new TitleScene();
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
		ret = new Pause();
	}

	if (ret != nullptr) {
		ret->SetScene(scene);
		ret->SystemInit();
		ret->GameInit();
		sceneList_.push_back(ret);
	}
	return;
}
