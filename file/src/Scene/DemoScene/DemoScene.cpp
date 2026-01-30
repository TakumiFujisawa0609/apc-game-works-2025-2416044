#include <DxLib.h>
#include "../../Manager/InputManager.h"
#include "DemoScene.h"

bool DemoScene::SystemInit() {
	screenID_ = MakeScreen(1280, 720);
	if (screenID_ == -1) return false;

	movieID_ = LoadGraph("data/video/知性の立方体_基本ルール説明.mp4");
	if (movieID_ == -1) return false;

	PlayMovieToGraph(movieID_);

	return true;
}

void DemoScene::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定") || ins.DownMap("ワナ") || ins.DownMap("戻る") ||
		!GetMovieStateToGraph(movieID_)) {
		nextScene_ = SceneBase::SCENE::TITLE;
	}
}

void DemoScene::Draw() {
	SetDrawScreen(screenID_);

	DrawGraph(0, 0, movieID_, FALSE);

	SetDrawScreen(DX_SCREEN_BACK);

	DrawRotaGraph(1280 / 2, 960 / 2, MOVIE_EXTEND_RATE, 0.0, screenID_, false);
}

bool DemoScene::Release() {
	DeleteGraph(movieID_);

	DeleteGraph(screenID_);

	return true;
}
