#include "../Stage/Block.h"
#include "../Stage/Stage.h"
#include "Trap.h"

Trap::Trap(Stage* s) : stage_(s) {}

bool Trap::Init() {
	Reset();

	waitExecute_ = -1;

	return true;
}

void Trap::Update(const VECTOR& player_pos) {
	bool nowKey = CheckHitKey(KEY_INPUT_J);

	if (nowKey && !prevKey_)
		if (!setTrap_) {
			setTrap_ = true;
			int x, z;
			stage_->ConvertStagePos(player_pos, x, z);
			stagePos_ = { (float)x, (float)z };
		}
		else {
			readyTrap_ = true;
		}

	prevKey_ = nowKey;
}

void Trap::Draw() {
	if (!setTrap_) return;

	VECTOR vt2 = stage_->ConvertWorldPos((int)stagePos_.x + 1, (int)stagePos_.y);
	VECTOR vt1 = stage_->ConvertWorldPos((int)stagePos_.x, (int)stagePos_.y + 1);
	vt1.y = 1.f;

	if (waitExecute_ == -1)
		DrawCube3D(vt1, vt2, 0x4040ff, 0xffffff, true);
	else
		DrawCube3D(vt1, vt2, 0xff4040, 0xffffff, true);

	VECTOR vc1 = stage_->ConvertWorldPos((int)stagePos_.x, (int)stagePos_.y);
	vc1.x += Block::HALF_BLOCK_SIZE; vc1.z -= Block::HALF_BLOCK_SIZE;
	vc1.y = Block::HALF_BLOCK_SIZE * 3.f;
	VECTOR vc2 = vc1;
	vc1.y += Block::HALF_BLOCK_SIZE / 2.f;

	if (waitExecute_ == -1)
		DrawCone3D(vc2, vc1, 20.f, 8, 0x4040ff, 0xffffff, true);
	else
		DrawCone3D(vc2, vc1, 20.f, 8, 0xff4040, 0xffffff, true);
}

bool Trap::Execute() {
	if (CheckTrapReady() || waitExecute_ >= 0) {
		if (!stage_->IsSpinning() && waitExecute_ == 0) {
			waitExecute_ = -1;
			return true;
		}
		else {
			if (waitExecute_ <= -1)
				waitExecute_ = 5;
			else if (waitExecute_ > 0)
				--waitExecute_;
		}
	}

	return false;
}

void Trap::Reset() { setTrap_ = readyTrap_ = false; }

bool Trap::CheckTrapReady() const { return setTrap_ && readyTrap_; }

Vector2 Trap::GetStagePos() const { return stagePos_; }
