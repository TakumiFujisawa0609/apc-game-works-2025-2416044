#include "../Common/AnimationController.h"
#include "../Stage/Block.h"
#include "Player.h"

bool Player::SystemInit() {
	modelId_ = MV1LoadModel("Data/Model/Player/Player.mv1");

	MV1SetScale(modelId_, SCALES);

	animControll_ = new AnimationController(modelId_);
	animControll_->AddInFbx(static_cast<int>(ANIM_TYPE::DEATH), 30.F, static_cast<int>(ANIM_TYPE::DEATH));
	animControll_->AddInFbx(static_cast<int>(ANIM_TYPE::IDLE), 30.F, static_cast<int>(ANIM_TYPE::IDLE));
	animControll_->AddInFbx(static_cast<int>(ANIM_TYPE::RUN), 30.F, static_cast<int>(ANIM_TYPE::RUN));

	return true;
}

bool Player::GameInit(int stage_width, int stage_depth) {
	pos_.x = Block::BLOCK_SIZE * stage_width / 2.F;
	pos_.y = 0.F;
	pos_.z = -(Block::BLOCK_SIZE * stage_depth - Block::BLOCK_SIZE * 3.F);
	MV1SetPosition(modelId_, pos_);

	animType_ = ANIM_TYPE::RUN;

	animControll_->Play(static_cast<int>(animType_));

	return true;
}

void Player::Update() {
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
