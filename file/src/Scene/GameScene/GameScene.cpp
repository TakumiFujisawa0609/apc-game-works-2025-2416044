#include "../../Common/GeometryDxLib.h"
#include "../../Object/Camera/Camera.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Stage/Block.h"
#include "../../Object/Stage/Stage.h"
#include "GameScene.h"

bool GameScene::SystemInit() {
	camera_ = new Camera();
	camera_->Init();

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

	Collision();
}

void GameScene::Draw() {
	int px, pz;
	stage_->GetPlatformSize(px, pz);

	camera_->BeforeDraw(player_->GetPos(), px, pz);
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

void GameScene::Collision() {
	// 足場判定
	CollisionPlatform();

	// キューブ判定
	CollisionCube();

	// 踏み潰され判定
	CollisionStomp();

	return;
}

void GameScene::CollisionCube() {
	VECTOR plPos = player_->GetPos();
	VECTOR plPrevPos = VSub(player_->GetPos(), player_->GetMove());
	VECTOR retPos = plPos;

	auto c1 = stage_->GetCubeList();

	// プレイヤーが移動していなければ、判定できない
	if (!GeometryDxLib::VEquals(plPos, plPrevPos))
		// キューブ1個ずつ判定する
		for (auto& c2 : c1) for (auto& c3 : c2) for (auto& c4 : c3) {
			// キューブが生存していなければ、衝突もしない
			if (!c4->IsAlive()) continue;

			// 手前 → キューブ
			if (plPos.z >= c4->GetMatrixPosition().z && c4->GetMatrixPosition().z >= plPrevPos.z &&
				plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) {
				retPos.z = c4->GetMatrixPosition().z;
			}

			// 奥 → キューブ
			if (c4->GetState() == Block::STATE::STOP) { // 停止中のみ判定
				if (plPos.z <= c4->GetMatrixPosition().z + Block::BLOCK_SIZE && c4->GetMatrixPosition().z + Block::BLOCK_SIZE <= plPrevPos.z &&
					plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) {
					retPos.z = c4->GetMatrixPosition().z + Block::BLOCK_SIZE;
				}
			}
			else {
				if (plPos.z <= c4->GetMatrixPosition().z && c4->GetMatrixPosition().z <= plPrevPos.z &&
					plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) {
					retPos.z = c4->GetMatrixPosition().z;
				}
			}

			// 左 → キューブ
			if (c4->GetState() == Block::STATE::STOP && // 停止中のみ判定
				plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x - Block::HALF_BLOCK_SIZE >= plPrevPos.x &&
				plPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPos.z) {
				retPos.x = c4->GetPosition().x - Block::HALF_BLOCK_SIZE;
			}

			// 右 → キューブ
			if (c4->GetState() == Block::STATE::STOP && // 停止中のみ判定
				plPos.x <= c4->GetPosition().x + Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE <= plPrevPos.x &&
				plPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPos.z) {
				retPos.x = c4->GetPosition().x + Block::HALF_BLOCK_SIZE ;
			}
		}

	player_->SetPos(retPos);
}

void GameScene::CollisionPlatform() {
	VECTOR plPos = player_->GetPos();
	VECTOR retPos = plPos;

	int px = 0, pz = 0;
	stage_->GetPlatformSize(px, pz);

	// 奥
	if (plPos.z > 0.f) retPos.z = 0.f;
	// 手前
	if (plPos.z < pz * -Block::BLOCK_SIZE) retPos.z = pz * -Block::BLOCK_SIZE - 1;
	// 左
	if (plPos.x < 0.f) retPos.x = 0.f;
	// 右
	if (plPos.x > px * Block::BLOCK_SIZE) retPos.x = px * Block::BLOCK_SIZE - 1;

	player_->SetPos(retPos);
}

void GameScene::CollisionStomp() {
	VECTOR plPos = player_->GetPos();
	VECTOR retPos = plPos;

	auto c1 = stage_->GetCubeList();

	for (auto& c2 : c1) for (auto& c3 : c2) for (auto& c4 : c3) {
		if (c4->GetState() == Block::STATE::STOP &&
			plPos.z < c4->GetPosition().z + Block::HALF_BLOCK_SIZE &&
			plPos.z > c4->GetPosition().z - Block::HALF_BLOCK_SIZE &&
			plPos.x < c4->GetPosition().x + Block::HALF_BLOCK_SIZE &&
			plPos.x > c4->GetPosition().x - Block::HALF_BLOCK_SIZE
			) {
			player_->Stomp();
		}
	}

	player_->SetPos(retPos);
}
