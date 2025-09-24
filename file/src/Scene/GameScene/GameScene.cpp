#include "../../Object/Player/Player.h"
#include "../../Object/Stage/Stage.h"
#include "GameScene.h"

bool GameScene::SystemInit() {
	stage_ = new Stage();
	stage_->SystemInit();

	player_ = new Player();
	player_->SystemInit();

	return true;
}

bool GameScene::GameInit() {
	stage_->GameInit();

	int px = 0, pz = 0;
	stage_->GetPlatformSize(px, pz);

	player_->GameInit(px, pz);

	return true;
}

void GameScene::Update() {
	stage_->Update();
	player_->Update();
}

void GameScene::Draw() {
	stage_->Draw();
	player_->Draw();
}

bool GameScene::Release() {
	stage_->Release();
	delete stage_;

	player_->Release();
	delete player_;

	return true;
}
