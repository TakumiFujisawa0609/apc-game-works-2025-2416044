#include "../Stage/Block.h"
#include "../Stage/Stage.h"
#include "Trap.h"

Trap::Trap(Stage* s) : stage_(s) {}

bool Trap::Init() {
	Reset();

	waitTrapExecute_ = -1;

	return true;
}

void Trap::Update() {
	for (auto it = traps_.begin(); it != traps_.end();) {
		auto& t = (*it);

		if (t.depWait > 0) t.depWait--;

		if (t.exeWait > 0) t.exeWait--;

		if (t.exeWait == 0 && !stage_->IsSpinning()) {
			stage_->VanishBlock(t.stagePos);
			it = traps_.erase(it);
		}
		else {
			it++;
		}
	}
}

void Trap::SetTrap(const VECTOR& pos, TRAP_TYPE type) {
	switch (type) {
	case TRAP_TYPE::NORMAL:
		SetNormalTrap(pos);
		break;
	case TRAP_TYPE::ADVANCE:
		SetAdvanceTrap(pos);
		break;
	}
}

void Trap::Draw() {
	for (auto& t : traps_) {
		VECTOR vt2 = stage_->ConvertWorldPos((int)t.stagePos.x + 1, (int)t.stagePos.y);
		VECTOR vt1 = stage_->ConvertWorldPos((int)t.stagePos.x, (int)t.stagePos.y + 1);
		vt1.y = 1.f;

		if (t.exeWait == -1)
			DrawCube3D(vt1, vt2, t.color, 0xffffff, true);
		else
			DrawCube3D(vt1, vt2, COLOR_EXECUTE, 0xffffff, true);

		VECTOR vc1 = stage_->ConvertWorldPos((int)t.stagePos.x, (int)t.stagePos.y);
		vc1.x += Block::HALF_BLOCK_SIZE; vc1.z -= Block::HALF_BLOCK_SIZE;
		vc1.y = Block::HALF_BLOCK_SIZE * 3.f;
		VECTOR vc2 = vc1;
		vc1.y += Block::HALF_BLOCK_SIZE / 2.f;

		if (t.exeWait == -1)
			DrawCone3D(vc2, vc1, 20.f, 8, t.color, 0xffffff, true);
		else
			DrawCone3D(vc2, vc1, 20.f, 8, COLOR_EXECUTE, 0xffffff, true);
	}
}

bool Trap::Release() {
	traps_.clear();
	return true;
}

void Trap::ExecuteAdvTrap() {
	int stSize[2] = {};
	stage_->GetPlatformSize(stSize[0], stSize[1]);

	for (auto it = traps_.begin(); it != traps_.end();) {
		TRAP t = (*it);
		if (t.type != TRAP_TYPE::ADVANCE || t.depWait != 0 || t.exeWait != -1) {
			it++;
			continue;
		}

		it = traps_.erase(it);

		for (int x = -1; x < 2; x++) for (int y = -1; y < 2; y++) {
			bool contFlag = false;
			Vector2 nPos = { t.stagePos.x + x, t.stagePos.y + y };

			if (nPos.x > stSize[0] - 1 || nPos.x < 0 ||
				nPos.y > stSize[1] - 1 || nPos.y < 0) continue;

			for (auto& at : traps_) if (nPos == at.stagePos) {
				contFlag = true;
				break;
			}
			if (contFlag) continue;
			
			TRAP nt = {};
			nt.type = t.type;
			nt.stagePos = nPos;
			nt.color = t.color;
			nt.depWait = 0;
			nt.exeWait = WAIT_EXECUTE;
			traps_.emplace_back(nt);
		}
	}
}

void Trap::Reset() { setTrap_ = readyTrap_ = false; }

bool Trap::CheckTrapReady() const { return setTrap_ && readyTrap_; }

Vector2 Trap::GetStagePos() const { return stagePos_; }

void Trap::SetNormalTrap(const VECTOR& pos) {
	auto nt = traps_.begin();
	bool flag = true;
	for (; nt != traps_.end(); nt++) {
		if ((*nt).type == TRAP_TYPE::NORMAL) {
			flag = false;
			break;
		}
	}


	if (flag) {
		int x, z;
		stage_->ConvertStagePos(pos, x, z);

		TRAP t = {};
		t.type = TRAP_TYPE::NORMAL;
		t.stagePos = { (float)x, (float)z };
		t.color = COLOR_TRAP;
		t.depWait = WAIT_DEPLOY;
		t.exeWait = -1;
		traps_.emplace_back(t);
	}
	else if (nt == traps_.end()) return;
	else {
		if ((*nt).depWait == 0) {
			(*nt).exeWait = WAIT_EXECUTE;
		}
	}
}

void Trap::SetAdvanceTrap(const VECTOR& pos) {
	int x, z;
	stage_->ConvertStagePos(pos, x, z);

	TRAP t = {};
	t.type = TRAP_TYPE::ADVANCE;
	t.stagePos = { (float)x, (float)z };
	t.color = COLOR_SUPER_TRAP;
	t.depWait = 0;
	t.exeWait = -1;
	traps_.emplace_back(t);
}
