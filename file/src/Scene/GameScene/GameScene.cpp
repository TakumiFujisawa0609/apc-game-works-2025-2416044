#include "Stage.h"
#include "GameScene.h"

bool GameScene::SystemInit() {
	stage_ = new Stage();
	stage_->SystemInit();

	return true;
}

bool GameScene::GameInit() {
	stage_->GameInit();

	return true;
}

void GameScene::Update() {
	stage_->Update();
}

void GameScene::Draw() {
	stage_->Draw();
}

bool GameScene::Release() {
	stage_->Release();
	delete stage_;

	return true;
}
