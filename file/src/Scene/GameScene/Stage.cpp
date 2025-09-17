#include <DxLib.h>
#include "Block.h"
#include "Stage.h"

bool Stage::SystemInit() {
	normalModel_ = MV1LoadModel("Data/Model/Blocks/Block_Stone.mv1");
	advantageModel_ = MV1LoadModel("Data/Model/Blocks/Block_Blank.mv1");
	forbiddenModel_ = MV1LoadModel("Data/Model/Blocks/Block_Crate.mv1");

	return true;
}

bool Stage::GameInit() {
	// 足場
	platformDepth_ = 15;

	for (int pd = 0; pd < platformDepth_; ++pd) {
		platformList_.emplace_back();
		for (int pw = 0; pw < PLATFORM_WIDTH; ++pw) {
			platformList_[pd].emplace_back();
			platformList_[pd][pw] = new Block();
			platformList_[pd][pw]->SetType(Block::TYPE::PLATFORM);
			platformList_[pd][pw]->SetModelHandle(advantageModel_);
			platformList_[pd][pw]->SetPosition(
				{ pw * Block::BLOCK_SIZE + Block::HALF_BLOCK_SIZE, -Block::HALF_BLOCK_SIZE, pd * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
			);
		}
	}

	cubeDepth_ = 2;
	wave_ = 3;

	// キューブ
	for (int cd = 0; cd < cubeDepth_ * wave_; ++cd) {
		cubeList_.emplace_back();
		for (int cw = 0; cw < CUBE_WIDTH; ++cw) {
			cubeList_[cd].emplace_back();
			cubeList_[cd][cw] = new Block();
			cubeList_[cd][cw]->SetType(Block::TYPE::NORMAL);
			cubeList_[cd][cw]->SetModelHandle(advantageModel_);
			cubeList_[cd][cw]->SetPosition(
				{ cw * Block::BLOCK_SIZE + Block::HALF_BLOCK_SIZE, Block::HALF_BLOCK_SIZE, cd * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
			);
		}
	}

	spinTimer_ = 0;
	delayTimer_ = 0;
	totalRotate_ = 0.F;

	return true;
}

void Stage::Update() {
	if (spinTimer_ <= 0) { // 回転終了後の処理
		if (delayTimer_ >= DELAY_FRAME) {
			// 回転をリセット
			totalRotate_ = 0.F;
		}

		// 足場の深さを用いて、キューブの落下処理を行う
		FallCube();

		// 次の回転に備えて、行列用座標を更新
		for (auto& i : cubeList_) for (auto& cube : i) {
			// 既に消えている場合、処理の必要が無いのでスキップ
			if (!cube->IsAlive()) continue;

			// 行列用座標を更新
			Vector3 pos = cube->GetPosition();
			Vector3 matPos = { 0.F, pos.y - Block::HALF_BLOCK_SIZE, pos.z - Block::HALF_BLOCK_SIZE };
			cube->SetMatrixPosition(matPos);
		}
		
		if (--delayTimer_ <= 0) {
			// 次の回転を開始
			spinTimer_ = SPIN_FRAME;
			delayTimer_ = DELAY_FRAME;
		}
	}
	else { // 回転中の処理
		--spinTimer_;

		// 反時計回りに回転させる
		totalRotate_ += DX_PI_F / 180.F * -SPIN_DEGREE;

		int nowCubeDepth = 0;
		for (auto i = cubeList_.rbegin(); i != cubeList_.rend(); ++i) {
			if (++nowCubeDepth > cubeDepth_) break;
			for (auto& cube : (*i)) {
				// 既に消えている場合、処理の必要が無いのでスキップ
				if (!cube->IsAlive()) continue;

				// キューブの座標
				Vector3 pos = cube->GetPosition();

				// キューブの行列用座標（Y軸座標とZ軸座標がマイナス方向の端）
				Vector3 matPos = cube->GetMatrixPosition();

				// 行列計算（原点への平行移動＋回転＋座標への平行移動）
				Matrix4x4 mat = TranslationMatrix(matPos) * RotationMatrixX(DX_PI_F / 180.F * -SPIN_DEGREE) * TranslationMatrix(-matPos);

				// キューブの座標を更新
				cube->SetPosition(mat * pos);

				// キューブの回転を更新
				cube->SetRotation({ totalRotate_, 0.F, 0.F });
			}
		}
	}
}

void Stage::Draw() {
	for (auto& i : platformList_) for (auto& platform : i) {
		platform->Draw();
	}

	for (auto& i : cubeList_) for (auto& cube : i) {
		cube->Draw();
	}
}

bool Stage::Release() {
	for (auto& i : platformList_) for (auto& platform : i) {
		if (platform != nullptr) {
			platform->Release();
			delete platform;
		}
	}
	platformList_.clear();

	for (auto& i : cubeList_) for (auto& cube : i) {
		if (cube != nullptr) {
			cube->Release();
			delete cube;
		}
	}
	cubeList_.clear();

	MV1DeleteModel(normalModel_);
	MV1DeleteModel(advantageModel_);
	MV1DeleteModel(forbiddenModel_);

	return true;
}

void Stage::FallCube() {
	for (auto& i : cubeList_) for (auto& cube : i) {
		// 消えている場合はスキップ
		if (!cube->IsAlive()) continue;
		Vector3 pos = cube->GetPosition();
		// 足場からはみ出している場合は落下させる
		if (pos.z < platformDepth_ * (-Block::BLOCK_SIZE)) {
			// 落下
			pos.y -= Block::HALF_BLOCK_SIZE / 2;
			cube->SetPosition(pos);
			// 地面に到達した場合
			if (pos.y < CUBE_FALL_RANGE_MAX) {
				// エフェクト無しで消す
				cube->SetAlive(false);
			}
		}
	}
}
