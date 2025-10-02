#include "../Stage/Block.h"
#include "../Stage/Stage.h"
#include "Trap.h"

Trap::Trap(Stage* s) : stage_(s) {}

bool Trap::Init() {
	Reset();

	return true;
}

void Trap::Update(const VECTOR& player_pos) {
	bool nowHitKey = CheckHitKey(KEY_INPUT_SPACE);

	if (nowHitKey && !prevSpaceKey_)
		if (!setTrap_) {
			setTrap_ = true;
			int x, z;
			stage_->ConvertStagePos(player_pos, x, z);
			stagePos_ = { (float)x, (float)z };
		}
		else {
			readyTrap_ = true;
		}

	prevSpaceKey_ = nowHitKey;
}

void Trap::Draw() {
	if (!setTrap_) return;

	VECTOR vt1 = stage_->ConvertWorldPos((int)stagePos_.x, (int)stagePos_.y - 1);
	VECTOR vt2 = stage_->ConvertWorldPos((int)stagePos_.x + 1, (int)stagePos_.y);
	vt2.y = 1.f;
	DrawCube3D(vt1, vt2, 0x0000ff, 0x0000ff, true);

	VECTOR vc1 = stage_->ConvertWorldPos((int)stagePos_.x, (int)stagePos_.y);
	vc1.x += Block::HALF_BLOCK_SIZE; vc1.z -= Block::HALF_BLOCK_SIZE;
	vc1.y = Block::HALF_BLOCK_SIZE * 3.f;
	VECTOR vc2 = vc1;
	vc1.y += Block::HALF_BLOCK_SIZE / 2.f;
	DrawCone3D(vc2, vc1, 20.f, 8, 0x0000ff, 0x0000ff, true);
}

void Trap::Reset() { setTrap_ = readyTrap_ = false; }

bool Trap::CheckTrapReady() const { return setTrap_ && readyTrap_; }

Vector2 Trap::GetStagePos() const { return stagePos_; }
