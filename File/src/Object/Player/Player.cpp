#include "../Common/AnimationController.h"
#include "../Stage/Block.h"
#include "Player.h"

bool Player::SystemInit() {
	modelId_ = MV1LoadModel("Data/Model/Player/Player.mv1");

	MV1SetScale(modelId_, SCALES);

	animControll_ = new AnimationController(modelId_);
	animControll_->AddInFbx(static_cast<int>(ANIM_TYPE::DEATH), ANIM_SPEED, static_cast<int>(ANIM_TYPE::DEATH));
	animControll_->AddInFbx(static_cast<int>(ANIM_TYPE::IDLE), ANIM_SPEED, static_cast<int>(ANIM_TYPE::IDLE));
	animControll_->AddInFbx(static_cast<int>(ANIM_TYPE::RUN), ANIM_SPEED, static_cast<int>(ANIM_TYPE::RUN));

	return true;
}

bool Player::GameInit(int stage_width, int stage_depth) {
	pos_.x = Block::BLOCK_SIZE * stage_width / 2.F;
	pos_.y = 0.F;
	pos_.z = -(Block::BLOCK_SIZE * stage_depth - Block::BLOCK_SIZE * 3.F);
	MV1SetPosition(modelId_, pos_);

	move_ = {};

	state_ = STATE::NORMAL;

	animType_ = ANIM_TYPE::RUN;

	animControll_->Play(static_cast<int>(animType_));

	return true;
}

void Player::Update() {
	VECTOR dir = {};

	switch (state_) {
	case STATE::NORMAL:
	case STATE::INVINCIBLE:
		// ‰¼‚ÌˆÚ“®ˆ—
		if (CheckHitKey(KEY_INPUT_W)) dir.z += 1.f;
		if (CheckHitKey(KEY_INPUT_S)) dir.z -= 1.f;
		if (CheckHitKey(KEY_INPUT_A)) dir.x -= 1.f;
		if (CheckHitKey(KEY_INPUT_D)) dir.x += 1.f;

		if (dir.x != 0.f || dir.z != 0.f)
			dir = VNorm(dir);

		move_ = VScale(dir, MOVE_SPEED);
		pos_ = VAdd(pos_, move_);

		MV1SetPosition(modelId_, pos_);
		break;
	case STATE::STOMP:
		break;
	}

	animControll_->Update();
}

void Player::Draw() {
	MV1DrawModel(modelId_);
}

bool Player::Release() {
	animControll_->Release();
	delete animControll_;

	MV1DeleteModel(modelId_);

	return true;
}

void Player::Stomp() {
	if (state_ != STATE::NORMAL) return;

	state_ = STATE::STOMP;

	animType_ = ANIM_TYPE::DEATH;
	animControll_->Play(static_cast<int>(animType_), false);
}

VECTOR Player::GetPos() const {
	return pos_;
}

void Player::SetPos(VECTOR pos) {
	pos_ = pos;
	MV1SetPosition(modelId_, pos_);
}

VECTOR Player::GetMove() const {
	return move_;
}
