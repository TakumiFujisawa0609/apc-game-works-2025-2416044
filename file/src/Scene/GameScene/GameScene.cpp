#include "../../Common/GeometryDxLib.h"
#include "../../Object/Camera/Camera.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Stage/Block.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Trap/Trap.h"
#include "GameScene.h"

bool GameScene::SystemInit() {
	camera_ = new Camera();
	camera_->Init();

	stage_ = new Stage();
	stage_->SystemInit();

	player_ = new Player();
	player_->SystemInit();

	trap_ = new Trap(stage_);

	camera_->SetFollowTarget(player_);

	return true;
}

bool GameScene::GameInit() {
	stage_->GameInit();

	int px = 0, pz = 0;
	stage_->GetPlatformSize(px, pz);

	player_->GameInit(px, pz);

	trap_->Init();

	return true;
}

void GameScene::Update() {
	stage_->Update();
	player_->Update();

	Collision();

	trap_->Update(player_->GetPos());
	if (trap_->Execute()) {
		stage_->VanishBlock(trap_->GetStagePos());
		trap_->Reset();
	}

	if (player_->GetState() == Player::STATE::STOMP || CheckHitKey(KEY_INPUT_L)) {
		stage_->SetFastForward(true);
		camera_->ChangeCameraMode(Camera::MODE::FIXED_FAST);
	}
	else {
		stage_->SetFastForward(false);
		camera_->ChangeCameraMode(Camera::MODE::FOLLOW);
	}
}

void GameScene::Draw() {
	int px, pz;
	stage_->GetPlatformSize(px, pz);

	camera_->BeforeDraw(px, pz);
	stage_->Draw();
	trap_->Draw();
	player_->Draw();

	// ウインドウサイズ取得処理
	int x, y;
	GetWindowSize(&x, &y);

	if (player_->GetState() == Player::STATE::OVER) {
		DrawRotaFormatString(x / 2, y / 2, 1.0, 1.0, 0.0, 0.0, 0.0, 0xffffffu, 0x000000u, 0, "GAME OVER");
	}
}

bool GameScene::Release() {
	stage_->Release();
	delete stage_;

	player_->Release();
	delete player_;

	//camera_->Release();
	delete camera_;

	//trap_->Release();
	delete trap_;

	return true;
}

void GameScene::Collision() {
	// 足場判定
	if (player_->GetState() != Player::STATE::OVER) CollisionPlatform();

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
				((plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) ||
					(plPrevPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPrevPos.x))) {
				retPos.z = c4->GetMatrixPosition().z - 1;
			}

			// 奥 → キューブ
			if (c4->GetState() == Block::STATE::STOP) { // 停止中のみ判定
				if (plPos.z <= c4->GetMatrixPosition().z + Block::BLOCK_SIZE && c4->GetMatrixPosition().z + Block::BLOCK_SIZE <= plPrevPos.z &&
					((plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) ||
						(plPrevPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPrevPos.x))) {
					retPos.z = c4->GetMatrixPosition().z + Block::BLOCK_SIZE + 1;
				}
			}
			else {
				if (plPos.z <= c4->GetMatrixPosition().z && c4->GetMatrixPosition().z <= plPrevPos.z &&
					((plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) ||
						(plPrevPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPrevPos.x))) {
					retPos.z = c4->GetMatrixPosition().z + 1;
				}
			}

			// 左 → キューブ
			if (c4->GetState() == Block::STATE::STOP && // 停止中のみ判定
				plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x - Block::HALF_BLOCK_SIZE >= plPrevPos.x &&
				((plPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPos.z) ||
					(plPrevPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPrevPos.z))) {
				retPos.x = c4->GetPosition().x - Block::HALF_BLOCK_SIZE - 1;
			}

			// 右 → キューブ
			if (c4->GetState() == Block::STATE::STOP && // 停止中のみ判定
				plPos.x <= c4->GetPosition().x + Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE <= plPrevPos.x &&
				((plPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPos.z) ||
					(plPrevPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPrevPos.z))) {
				retPos.x = c4->GetPosition().x + Block::HALF_BLOCK_SIZE + 1;
			}
		}

	player_->SetPos(retPos);
}

void GameScene::CollisionPlatform() {
	VECTOR playerPos = player_->GetPos();
	VECTOR retPos = playerPos;

	int platformX, platformZ;
	stage_->GetPlatformSize(platformX, platformZ);
	VECTOR platform = { platformX * Block::BLOCK_SIZE, 0.f, platformZ * -Block::BLOCK_SIZE };

	// 左
	if (playerPos.x < 0.f) retPos.x = 0.f;
	// 右
	if (playerPos.x > platform.x) retPos.x = platform.x - 1;
	// 奥
	if (playerPos.z > 0.f) retPos.z = 0.f;
	// 手前
	if (playerPos.z < platform.z) {
		int playerX, playerZ;
		stage_->ConvertStagePos(playerPos, playerX, playerZ);
		int prevPlatformZ = stage_->GetPrevPlatformSizeZ();

		if (playerZ > platformZ - 1 && prevPlatformZ > platformZ)
			player_->Over();
		else
			retPos.z = platform.z + 1;
	}

	player_->SetPos(retPos);
}

void GameScene::CollisionStomp() {
	VECTOR plPos = player_->GetPos();

	auto c1 = stage_->GetCubeList();

	for (auto& c2 : c1) for (auto& c3 : c2) for (auto& c4 : c3) {
		if (c4->GetRotation().x < DegToRad(-75.f) &&
			plPos.z <= c4->GetMatrixPosition().z &&
			plPos.z >= c4->GetMatrixPosition().z - Block::BLOCK_SIZE &&
			plPos.x <= c4->GetPosition().x + Block::HALF_BLOCK_SIZE &&
			plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE
			) {
			player_->Stomp();
		}
	}
}
