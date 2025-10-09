#include "Block.h"
#include "Stage.h"
#include "../../Common/GeometryDxLib.h"

bool Stage::SystemInit() {
	normalModel_ = MV1LoadModel("Data/Model/Blocks/Block_Stone.mv1");
	advantageModel_ = MV1LoadModel("Data/Model/Blocks/Block_Blank.mv1");
	forbiddenModel_ = MV1LoadModel("Data/Model/Blocks/Block_Crate.mv1");

	return true;
}

bool Stage::GameInit() {
	blockWidth_ = 4;

	// 足場
	platformDepth_ = 24;

	for (int pd = 0; pd < platformDepth_; ++pd) {
		auto& ptr = platformList_.emplace_back();
		ptr = new Block(blockWidth_);
		ptr->SetType(Block::TYPE::PLATFORM);
		ptr->SetModelHandle(normalModel_);
		ptr->SetStageIndex(0, pd);
		ptr->SetPosition(
			{ Block::HALF_BLOCK_SIZE, -Block::HALF_BLOCK_SIZE, pd * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
		);
	}

	cubeDepth_ = 2;
	phase_ = 3;
	wave_ = 4;

	// キューブ
	for (int cp = 0; cp < phase_; ++cp) {
		auto& waveList = cubeList_.emplace_back();
		for (int cd = 0; cd < cubeDepth_; ++cd) {
			auto& subList = waveList.emplace_back();
			for (int cw = 0; cw < blockWidth_; ++cw) {
				auto& ptr = subList.emplace_back();
				ptr = new Block();
				ptr->SetType(Block::TYPE::NORMAL);
				ptr->SetModelHandle(normalModel_);
				ptr->SetStageIndex(cw, cp * cubeDepth_ + cd);
				ptr->SetPosition(
					{ cw * Block::BLOCK_SIZE + Block::HALF_BLOCK_SIZE, Block::HALF_BLOCK_SIZE, (cp * cubeDepth_ + cd) * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
				);
			}
		}
	}

	spinTimer_ = 0;
	extraTimer_ = 0;
	isSpinning_ = false;
	fastForward_ = false;

	fallCount_ = 0;

	return true;
}

void Stage::Update() {
	if (!isSpinning_) { // 回転終了後の処理
		UpdateStop();
	}
	else { // 回転中の処理
		UpdateSpin();
	}

	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		cube->Update();
	}

	if (fallCount_ >= blockWidth_) {
		fallCount_ -= blockWidth_;
		
		for (auto rit = platformList_.rbegin(); rit != platformList_.rend(); rit++) {
			if ((*rit)->GetState() == Block::STATE::STOP) {
				(*rit)->ChangeState(Block::STATE::ALERT);
				break;
			}
		}
	}

	int count = 0;
	prevPlatformDepth_ = platformDepth_;
	for (auto& platform : platformList_) {
		platform->Update();

		if (platform->GetState() != Block::STATE::FALL) count++;

		if (!platform->IsAlive()) {
			delete platform;
			platformList_.remove(platform);
			break;
		}
	}
	platformDepth_ = count;
}

void Stage::Draw() {
	// 足場
	for (auto& platform : platformList_) {
		platform->Draw();
	}

	// キューブ
	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		cube->Draw();
	}

	// GUI
	// ウインドウサイズ取得処理
	int x, y;
	GetWindowSize(&x, &y);
	// 回転
	if (isSpinning_) DrawFormatString(32, y - 32, 0xFFFFFFU, "回転中");
	// 落下数
	DrawFormatString(x - 256, y - 32, 0xFFFFFFU, "足場崩壊まで: %d/%d", fallCount_, blockWidth_);
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

void Stage::VanishBlock(const Vector2& trap_stage_pos) {
	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		if (cube->GetStageIndex() == trap_stage_pos) {
			cube->ChangeState(Block::STATE::VANISH);

			if (cube->GetType() == Block::TYPE::FORBIDDEN) fallCount_ += blockWidth_;

			spinTimer_ = SPIN_DELAY_FRAME;
			return;
		}
	}
}

void Stage::ConvertStagePos(const VECTOR& pos, int& x, int& z) {
	x = (int)(pos.x / Block::BLOCK_SIZE);
	z = (int)(pos.z / -Block::BLOCK_SIZE);
}

VECTOR Stage::ConvertWorldPos(int x, int z) {
	VECTOR ret = {};
	ret.x = x * Block::BLOCK_SIZE;
	ret.z = z * -Block::BLOCK_SIZE;
	return ret;
}

void Stage::GetPlatformSize(int& x, int& z) const {
	x = blockWidth_;
	z = platformDepth_;
}

int Stage::GetPrevPlatformSizeZ() const { return prevPlatformDepth_; }

std::list<std::list<std::list<Block*>>> Stage::GetCubeList() const { return cubeList_; }

void Stage::SetFastForward(bool b) { fastForward_ = b; }

bool Stage::IsSpinning() const { return isSpinning_; }

void Stage::UpdateStop() {
	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		// 既に消えている場合、処理の必要が無いのでスキップ
		if (!cube->IsAlive() || cube->GetState() == Block::STATE::FALL) continue;

		// 足場からはみ出している場合は落下させる
		if (cube->GetPosition().z < platformDepth_ * (-Block::BLOCK_SIZE)) {
			// 落下
			cube->ChangeState(Block::STATE::FALL);
			if (cube->GetType() != Block::TYPE::FORBIDDEN) fallCount_++;
		}
		else {
			// 回転を止める
			cube->ChangeState(Block::STATE::STOP);
		}
	}
	
	auto stopFlag = true;
	if (cubeList_.size() > 0) {
		auto& waveList = cubeList_.back();
		for (auto& subList : waveList) {
			for (auto& cube : subList) {
				if (cube->IsAlive() &&
					cube->GetState() != Block::STATE::FALL &&
					cube->GetState() != Block::STATE::VANISH) {
					stopFlag = false;
					break;
				}
			}
			if (!stopFlag) break;
		}
	}

	auto delFlag = true;
	if (cubeList_.size() > 0) {
		auto& waveList = cubeList_.back();
		for (auto& subList : waveList) {
			for (auto& cube : subList) {
				if (cube->IsAlive()) {
					delFlag = false;
					break;
				}
			}
			if (!delFlag) break;
		}
	}
	else delFlag = false;

	if (delFlag) {
		ReleaseWave();
		extraTimer_ = EXTRA_DELAY_FRAME;
		return;
	}

	if (fastForward_) spinTimer_ = 0;

	if (spinTimer_ > 0) spinTimer_--;
	else if (extraTimer_ > 0) extraTimer_--;

	if (!stopFlag && extraTimer_ <= 0 && spinTimer_ <= 0) {
		// 次の回転を開始
		isSpinning_ = true;
	}
}

void Stage::UpdateSpin() {
	if (cubeList_.size() > 0) {
		++spinTimer_;
		auto& waveList = cubeList_.back();
		for (auto& subList : waveList) for (auto& cube : subList) {
			// 既に消えているか消失中の場合、処理の必要が無いのでスキップ
			if (!cube->IsAlive() || cube->GetState() == Block::STATE::VANISH || cube->GetState() == Block::STATE::FALL) continue;

			// 衝突判定用（回転中は、キューブ後方の下辺の衝突判定が消える）
			cube->ChangeState(Block::STATE::SPIN);

			// キューブの回転量
			auto cubeRot = cube->GetRotation();

			// 追加される回転量
			float deg = -SPIN_DEGREE;
			if (fastForward_) deg = -FAST_SPIN_DEGREE;
			float nextRot = DX_PI_F / 180.f * deg;

			// 合計回転量が-90度をオーバーしないように調整
			if (cubeRot.x + nextRot < -DX_PI_F / 2.f)
				nextRot = -DX_PI_F / 2.f - cubeRot.x;

			// 行列計算（原点への平行移動＋回転＋座標への平行移動）
			Matrix4x4 mat = TranslationMatrix(cube->GetMatrixPosition()) * RotationMatrixX(nextRot) * TranslationMatrix(-cube->GetMatrixPosition());

			// キューブの座標を更新
			cube->SetPosition(mat * cube->GetPosition());

			// キューブの回転を更新
			cubeRot.x += nextRot;
			cube->SetRotation(cubeRot);

			// キューブの回転量が-90度になったら回転処理を停止
			if (cubeRot.x <= -DX_PI_F / 2.f) {
				auto idx = cube->GetStageIndex();
				cube->SetStageIndex({ idx.x, idx.y + 1 });
				if (isSpinning_) isSpinning_ = false;
			}
		}
	}
	else isSpinning_ = false;
}
