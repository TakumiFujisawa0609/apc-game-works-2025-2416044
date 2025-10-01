#include "../../Common/GeometryDxLib.h"
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
	pos_.x = Block::BLOCK_SIZE * stage_width / 2.f;
	pos_.y = 0.f;
	pos_.z = -(Block::BLOCK_SIZE * stage_depth - Block::BLOCK_SIZE * 3.f);
	MV1SetPosition(modelId_, pos_);

	worldAngles_ = { 0.f, 0.f, 0.f };
	MV1SetRotationMatrix(modelId_, GeometryDxLib::Multiplication(LOCAL_ANGLES, worldAngles_));

	MV1SetScale(modelId_, SCALES);

	MV1SetMaterialEmiColor(modelId_, 0, MODEL_EMI_COLOR);

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

		if (dir.x != 0.f || dir.z != 0.f) {
			dir = VNorm(dir);
			worldAngles_.y = atan2f(dir.x, dir.z);

			animType_ = ANIM_TYPE::RUN;
		}
		else {
			animType_ = ANIM_TYPE::IDLE;
		}

		move_ = VScale(dir, MOVE_SPEED);
		pos_ = VAdd(pos_, move_);

		MV1SetPosition(modelId_, pos_);
		MV1SetRotationMatrix(modelId_, GeometryDxLib::Multiplication(LOCAL_ANGLES, worldAngles_));
		break;
	case STATE::STOMP:
		break;
	}

	animControll_->Play(static_cast<int>(animType_));
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
