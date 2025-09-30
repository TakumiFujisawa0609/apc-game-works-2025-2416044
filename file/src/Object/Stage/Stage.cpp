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
	cubeWidth_ = 4;

	// 足場
	platformDepth_ = 16;

	for (int pd = 0; pd < platformDepth_; ++pd) {
		auto& ptr = platformList_.emplace_back();
		ptr = new Block(cubeWidth_);
		ptr->SetType(Block::TYPE::PLATFORM);
		ptr->SetModelHandle(normalModel_);
		ptr->SetPosition(
			{ Block::HALF_BLOCK_SIZE, -Block::HALF_BLOCK_SIZE, pd * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
		);
	}

	cubeDepth_ = 2;
	wave_ = 3;

	// キューブ
	for (int cp = 0; cp < wave_; ++cp) {
		auto& waveList = cubeList_.emplace_back();
		for (int cd = 0; cd < cubeDepth_; ++cd) {
			auto& subList = waveList.emplace_back();
			for (int cw = 0; cw < cubeWidth_; ++cw) {
				auto& ptr = subList.emplace_back();
				ptr = new Block();
				ptr->SetType(Block::TYPE::NORMAL);
				ptr->SetModelHandle(advantageModel_);
				ptr->SetPosition(
					{ cw * Block::BLOCK_SIZE + Block::HALF_BLOCK_SIZE, Block::HALF_BLOCK_SIZE, (cp * cubeDepth_ + cd) * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
				);
			}
		}
	}

	spinTimer_ = 0;
	isSpinning_ = false;
	fastForward_ = false;

	return true;
}

void Stage::Update() {
	if (!isSpinning_) { // 回転終了後の処理
		for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
			// 既に消えている場合、処理の必要が無いのでスキップ
			if (!cube->IsAlive() || cube->GetState() == Block::STATE::FALL) continue;

			// 足場からはみ出している場合は落下させる
			if (cube->GetPosition().z < platformDepth_ * (-Block::BLOCK_SIZE)) {
				// 落下
				cube->ChangeState(Block::STATE::FALL);
				fallCount_++;
			}
			else {
				// 回転を止める
				cube->ChangeState(Block::STATE::STOP);
			}
		}

		auto delFlag = true;
		if (cubeList_.size() > 0) {
			auto& waveList = cubeList_.back();
			for (auto& subList : waveList) {
				for (auto& cube : subList) {
					if (!cube->IsAlive()) continue;
					else delFlag = false;
				}
				if (!delFlag) break;
			}
		}
		else delFlag = false;

		if (delFlag) {
			ReleaseWave();
			spinTimer_ += SPIN_DELAY_FRAME;
		}

		auto actFlag = false;
		if (cubeList_.size() > 0) {
			auto& waveList = cubeList_.back();
			for (auto& subList : waveList) {
				for (auto& cube : subList) {
					if (cube->GetState() != Block::STATE::STOP) continue;
					else actFlag = true;
				}
				if (actFlag) break;
			}
		}

		if (fastForward_) spinTimer_ = 0;
		if (spinTimer_ > 0) spinTimer_--;

		if (actFlag) if (spinTimer_ <= 0) {
			// 次の回転を開始
			isSpinning_ = true;
		}
	}
	else { // 回転中の処理
		++spinTimer_;
		if (cubeList_.size() > 0) {
			auto& waveList = cubeList_.back();
			for (auto& subList : waveList) for (auto& cube : subList) {
				// 既に消えている場合、処理の必要が無いのでスキップ
				if (!cube->IsAlive()) continue;

				// 衝突判定用（回転中は、キューブ後方の下辺の衝突判定が消える）
				cube->ChangeState(Block::STATE::SPIN);

				// キューブの回転量
				auto cubeRot = cube->GetRotation();

				// 追加される回転量
				float deg = -SPIN_DEGREE;
				if (fastForward_) deg = -FAST_SPIN_DEGREE;
				float nextRot = DX_PI_F / 180.F * deg;

				// 合計回転量が-90度をオーバーしないように調整
				if (cubeRot.x + nextRot < -DX_PI_F / 2.F) nextRot = -DX_PI_F / 2.F - cubeRot.x;

				// 行列計算（原点への平行移動＋回転＋座標への平行移動）
				Matrix4x4 mat = TranslationMatrix(cube->GetMatrixPosition()) * RotationMatrixX(nextRot) * TranslationMatrix(-cube->GetMatrixPosition());

				// キューブの座標を更新
				cube->SetPosition(mat * cube->GetPosition());

				// キューブの回転を更新
				cubeRot.x += nextRot;
				cube->SetRotation(cubeRot);

				// キューブの回転量が-90度になったら回転処理を停止
				if (cubeRot.x <= -DX_PI_F / 2.F) isSpinning_ = false;
			}
		}
	}

	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		cube->Update();
	}

	for (auto& platform : platformList_) {
		platform->Update();
		if (!platform->IsAlive()) {
			delete platform;
			platformList_.remove(platform);
			break;
		}
	}
}

void Stage::Draw() {
	for (auto& platform : platformList_) {
		platform->Draw();
	}

	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		cube->Draw();
	}

	int x, y;
	GetWindowSize(&x, &y);

	DrawFormatString(x - 128, y - 32, 0xFFFFFFU, "落下数: %d", fallCount_);
}

bool Stage::Release() {
	for (auto& platform : platformList_) {
		if (platform != nullptr) {
			platform->Release();
			delete platform;
		}
	}
	platformList_.clear();

	for (auto& waveList : cubeList_) {
		for (auto& subList : waveList) {
			for (auto& cube : subList) {
				if (cube != nullptr) {
					cube->Release();
					delete cube;
				}
			}
			subList.clear();
		}
		waveList.clear();
	}
	cubeList_.clear();

	MV1DeleteModel(normalModel_);
	MV1DeleteModel(advantageModel_);
	MV1DeleteModel(forbiddenModel_);

	return true;
}

bool Stage::ReleaseWave() {
	if (cubeList_.size() <= 0) return false;

	auto& waveList = cubeList_.back();
	for (auto& subList : waveList) {
		for (auto& cube : subList) {
			cube->Release();
			delete cube;
		}
		subList.clear();
	}
	waveList.clear();
	cubeList_.pop_back();

	return true;
}

void Stage::GetPlatformSize(int& x, int& z) const {
	x = cubeWidth_;
	z = platformDepth_;
}

std::list<std::list<std::list<Block*>>> Stage::GetCubeList() const {
	return cubeList_;
}

void Stage::SetFastForward(bool b) {
	fastForward_ = b;
}
