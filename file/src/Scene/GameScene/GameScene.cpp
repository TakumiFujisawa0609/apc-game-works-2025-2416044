#include <algorithm>
#include "../../Common/Fader.h"
#include "../../Common/GeometryDxLib.h"
#include "../../Common/MathUtil.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Object/Camera/Camera.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Stage/Block.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Trap/Trap.h"
#include "GameScene.h"

bool GameScene::SystemInit() {
	camera_ = new Camera();
	camera_->Init();

	stage_ = new Stage(this);
	stage_->SystemInit();

	player_ = new Player();
	player_->SystemInit();

	trap_ = new Trap(stage_);

	camera_->SetFollowTarget(player_);

	return true;
}

void GameScene::SetStageNumber(unsigned int stage_num) {
	stageNum_ = std::clamp(stage_num, 0u, 2u);
}

bool GameScene::GameInit() {
	stage_->GameInit(stageNum_);

	int px = 0, pz = 0;
	stage_->GetPlatformSize(px, pz);

	player_->GameInit(px, pz);

	trap_->Init();

	return true;
}

void GameScene::Update() {
	auto& ins = InputManager::GetInstance();

	if (!stage_->IsClear()) {
		if (SceneManager::GetInstance().GetFaderPtr()->IsFadeEnd() && ins.DownMap("ポーズ")) {
			if (player_->GetState() != Player::STATE::OVER)
				nextScene_ = SceneBase::SCENE::PAUSE;
			else
				nextScene_ = SceneBase::SCENE::TITLE;
		}
	}

	if (!stage_->IsClear() &&
		player_->GetState() != Player::STATE::OVER) {
		// マーキング＆マーク起動
		if (ins.DownMap("ワナ"))
			trap_->SetTrap(player_->GetPos());

		// アドバンスドマーク起動
		if (ins.DownMap("スーパーワナ"))
			trap_->ExecuteAdvTrap();
	}

	if (player_->GetPos().y > Player::FALL_FINISH_Y) {
		stage_->Update();

		if (SceneManager::GetInstance().GetFaderPtr()->IsFadeEnd() && !stage_->IsClear()) {
			player_->Update();

			// マーク更新
			trap_->Update();

			Collision();
		}
		else {
			player_->UpdateClear();
			trap_->Reset();
		}
	}

	if (stage_->IsEnd() > 300) {
		if (++stageNum_ < 3) {
			SceneManager::GetInstance().GetFaderPtr()->ForceSetMode(Fader::FADE_MODE::FADE_OUT);
			SceneManager::GetInstance().GetFaderPtr()->SetFadeMode(Fader::FADE_MODE::FADE_IN, 60U, 120U);
			GameInit();
		}
		else {
			nextScene_ = SceneBase::SCENE::TITLE;
		}
	}

	if (stage_->IsClear()) {
		camera_->ChangeCameraMode(Camera::MODE::FIXED_CLEAR);
	}
	else if (player_->GetState() == Player::STATE::OVER) {
		camera_->ChangeCameraMode(Camera::MODE::FIXED_OVER);
	}
	else if (camera_->GetCameraMode() != Camera::MODE::FIXED_PERFECT) {
		// 高速進行＆視点
		if (player_->GetState() == Player::STATE::STOMP || ins.NowMap("高速送り")) {
			if (!stage_->IsVanishing())
				stage_->SetFastForward(true);
			else
				stage_->SetFastForward(false);

			camera_->ChangeCameraMode(Camera::MODE::FIXED_FAST);
		}
		else {
			stage_->SetFastForward(false);
			camera_->ChangeCameraMode(Camera::MODE::FOLLOW);
		}
	}
}

void GameScene::Draw() {
	int px, pz;
	stage_->GetPlatformSize(px, pz);

	if (camera_->GetCameraMode() == Camera::MODE::FIXED_CLEAR)
		camera_->BeforeDraw(px, pz, stage_->IsClear());
	else
		camera_->BeforeDraw(px, pz);

	stage_->Draw();

	trap_->Draw();

	player_->Draw();
}

void GameScene::DrawUI() {
	// ウインドウサイズ取得処理
	int x, y;
	GetWindowSize(&x, &y);

	// フォント取得
	auto f = FontManager::GetInstance().GetFontData("汎用");
	auto fs = FontManager::GetInstance().GetFontData("汎用（小）");
	auto fl = FontManager::GetInstance().GetFontData("汎用（大）");

	if (player_->GetState() != Player::STATE::OVER) {
		stage_->DrawUI();

		std::string waveStr = "";
		for (int i = 1; i <= Stage::PHASE_MAX; i++) {
			if (i <= stage_->GetPhase()) waveStr += "■";
			else waveStr += "□";
		}
		DrawFormatStringToHandle(48, 48, 0xFFFFFFU, f.handle, "[%s]%s", STAGE_WIDE_NUMBER[stageNum_], waveStr.c_str());

		DrawFormatStringToHandle(48, 120, 0xFFFFFFU, f.handle, "%09u", score_ * 100u);
	}

	if (player_->GetPos().y == Player::FALL_FINISH_Y) {
		DrawFormatStringToHandle(385, 450, 0xFFFFFFU, fl.handle, "G A M E O V E R");
	}
	else if (player_->GetState() != Player::STATE::OVER && stage_->IsClear()) {
		auto counter = stage_->IsClear();
		if (counter >= Stage::CLEAR_WAIT_TIMER) {
			counter -= Stage::CLEAR_WAIT_TIMER;
			DrawFormatStringToHandle(385, 415, 0xFFFFFFU, fl.handle, "  CLEAR BONUS  ");
			DrawFormatStringToHandle(385, 485, 0xFFFFFFU, fl.handle, "     %5d", counter / Stage::CLEAR_PLATFORM_MOVE_TIMER * 1000);
		}
		else {
			if (stageNum_ == 2) {
				DrawFormatStringToHandle(385, 450, 0xFFFFFFU, fl.handle, "A L L C L E A R");
			}
			else {
				DrawFormatStringToHandle(385, 450, 0xFFFFFFU, fl.handle, "   C L E A R   ");
			}
		}
	}
	else {
		if (player_->IsInvincible()) {
			DrawFormatStringToHandle(385, 450, 0xFFFFFFU, fl.handle, "  A g a i n !  ");
		}
		else if (camera_->GetCameraMode() == Camera::MODE::FIXED_PERFECT) {
			DrawFormatStringToHandle(385, 450, 0xFFFFFFU, fl.handle, "P e r f e c t !");
		}
	}
}

bool GameScene::Release() {
	stage_->Release();
	delete stage_;

	player_->Release();
	delete player_;

	//camera_->Release();
	delete camera_;

	trap_->Release();
	delete trap_;

	SceneManager::GetInstance().SetLastScore(score_);

	return true;
}

void GameScene::Collision() {
	// キューブ判定
	CollisionCube();

	// 足場判定
	if (player_->GetState() != Player::STATE::OVER) CollisionPlatform();

	// 踏み潰され判定
	CollisionStomp();

	return;
}

void GameScene::Restart() {
	stageNum_++;

	GameInit();

	SceneManager::GetInstance().GetFaderPtr()->SetFadeMode(
		Fader::FADE_MODE::FADE_OUT, 0U);
}

Player* GameScene::GetPlayerPtr() { return player_; }

Stage* GameScene::GetStagePtr() { return stage_; }

Camera* GameScene::GetCameraPtr() { return camera_; }

Trap* GameScene::GetTrapPtr() { return trap_; }

unsigned int GameScene::GetScore() const { return score_; }

void GameScene::AddScore(int integer) {
	if ((long long)score_ + integer < 0) {
		score_ = 0u;
	}
	else if (score_ + integer > SCORE_MAX) {
		score_ = SCORE_MAX;
	}
	else {
		score_ += integer;
	}
}

void GameScene::CollisionCube() {
	VECTOR plPos = player_->GetPos();
	VECTOR plPrevPos = VSub(player_->GetPos(), player_->GetMove());
	VECTOR retPos = plPos;

	auto c1 = stage_->GetCubeList();

	// キューブ1個ずつ判定する
	for (auto& c2 : c1) for (auto& c3 : c2) for (auto& c4 : c3) {
		// キューブが生存していなければ衝突しない、
		// キューブが地面より下であれば衝突しない
		if (!c4->IsAlive() ||
			c4->GetState() == Block::STATE::RISING && !c4->IsRising()) continue;

		// プレイヤーが移動していなければ、判定できない
		if (!GeometryDxLib::VEquals(plPos, plPrevPos)) {
			// 手前 → キューブ
			if (plPos.z >= c4->GetMatrixPosition().z && c4->GetMatrixPosition().z >= plPrevPos.z &&
				((plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) ||
					(plPrevPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPrevPos.x))) {
				// キューブの手前側に押し戻す
				retPos.z = c4->GetMatrixPosition().z - 1;
			}

			if (!(c4->GetState() == Block::STATE::RISING || c4->GetState() == Block::STATE::WAIT ||
				c4->GetState() == Block::STATE::STOP || c4->GetState() == Block::STATE::VANISH)) { // 移動中のみ判定
				// 奥 → キューブ
				if (plPos.z <= c4->GetMatrixPosition().z && c4->GetMatrixPosition().z <= plPrevPos.z &&
					((plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) ||
						(plPrevPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPrevPos.x))) {
					// キューブの奥側に押し戻す
					retPos.z = c4->GetMatrixPosition().z + 1;
				}
			}
			else { // 停止中のみ判定
				// 奥 → キューブ
				if (plPos.z <= c4->GetMatrixPosition().z + Block::BLOCK_SIZE && c4->GetMatrixPosition().z + Block::BLOCK_SIZE <= plPrevPos.z &&
					((plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPos.x) ||
						(plPrevPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE >= plPrevPos.x))) {
					// キューブの奥側に押し戻す
					retPos.z = c4->GetMatrixPosition().z + Block::BLOCK_SIZE + 1;
				}

				// 左 → キューブ
				if (plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE && c4->GetPosition().x - Block::HALF_BLOCK_SIZE >= plPrevPos.x &&
					((plPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPos.z) ||
						(plPrevPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPrevPos.z))) {
					// キューブの左側に押し戻す
					retPos.x = c4->GetPosition().x - Block::HALF_BLOCK_SIZE - 1;
				}

				// 右 → キューブ
				if (plPos.x <= c4->GetPosition().x + Block::HALF_BLOCK_SIZE && c4->GetPosition().x + Block::HALF_BLOCK_SIZE <= plPrevPos.x &&
					((plPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPos.z) ||
						(plPrevPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE && c4->GetPosition().z + Block::HALF_BLOCK_SIZE >= plPrevPos.z))) {
					// キューブの右側に押し戻す
					retPos.x = c4->GetPosition().x + Block::HALF_BLOCK_SIZE + 1;
				}
			}
		}

		// 上昇中
		if (plPos.z <= c4->GetPosition().z + Block::HALF_BLOCK_SIZE &&
			plPos.z >= c4->GetPosition().z - Block::HALF_BLOCK_SIZE &&
			plPos.x <= c4->GetPosition().x + Block::HALF_BLOCK_SIZE &&
			plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE &&
			c4->IsRising()) {
			// 転倒
			player_->Rolling();
			return;
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

		if (player_->GetState() == Player::STATE::ROLLING ||
			playerZ > platformZ - 1 && prevPlatformZ > platformZ)
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
		// キューブが生存していなければ、衝突もしない
		if (!c4->IsAlive()) continue;

		if (c4->GetRotation().x < MathUtil::DegToRad(Stage::STOMP_DEGREE_THRESHOLD) &&
			plPos.z <= c4->GetMatrixPosition().z &&
			plPos.z >= c4->GetMatrixPosition().z - Block::BLOCK_SIZE &&
			plPos.x <= c4->GetPosition().x + Block::HALF_BLOCK_SIZE &&
			plPos.x >= c4->GetPosition().x - Block::HALF_BLOCK_SIZE
			) {
			player_->Stomp();
			stage_->StartStep();
		}
	}
}
