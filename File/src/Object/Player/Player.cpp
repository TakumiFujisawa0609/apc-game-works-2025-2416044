#include <algorithm>
#include "../../Common/GeometryDxLib.h"
#include "../../Common/MathUtil.h"
#include "../../Manager/InputManager.h"
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
	pos_.x = Block::BLOCK_SIZE * stage_width / 2;
	pos_.y = 0.0f;
	pos_.z = -Block::BLOCK_SIZE * stage_depth / 4 * 3;
	MV1SetPosition(modelId_, pos_);

	worldAngles_ = { 0.0f, 0.0f, 0.0f };
	MV1SetRotationMatrix(modelId_, GeometryDxLib::Multiplication(LOCAL_ANGLES, worldAngles_));

	MV1SetScale(modelId_, SCALES);

	MV1SetMaterialEmiColor(modelId_, 0, MODEL_EMI_COLOR);

	move_ = {};

	state_ = STATE::NORMAL;

	animType_ = ANIM_TYPE::IDLE;

	animControll_->Play(static_cast<int>(animType_));

	stateTimer_ = 0;

	invincible_ = 0;

	return true;
}

void Player::Update() {
	// 無敵時間の消費
	if (invincible_ > 0) invincible_--;

	// ステートメント
	switch (state_) {
	case STATE::NORMAL:
		// タイマーリセット
		stateTimer_ = 0;

		// 移動処理
		Move();

		animControll_->Play(static_cast<int>(animType_));
		break;
	case STATE::ROLLING:
		pos_.z -= ROLLING_SPEED;

		// 回転リセット
		worldAngles_.y = 0.0f;

		if (stateTimer_++ >= 1) {
			// 通常状態に移行
			state_ = STATE::NORMAL;

			// タイマーリセット
			stateTimer_ = 0;
		}

		animType_ = ANIM_TYPE::DEATH;
		animControll_->Play(static_cast<int>(animType_), false);
		break;
	case STATE::STOMP:
		if (stateTimer_++ >= STATE_STOMP_TIME) {
			// 一定時間無敵
			invincible_ = INVINCIBLE_TIME;

			// 通常状態に移行
			state_ = STATE::NORMAL;

			// タイマーリセット
			stateTimer_ = 0;
		}

		animType_ = ANIM_TYPE::DEATH;
		animControll_->Play(static_cast<int>(animType_), false);
		break;
	case STATE::OVER:
		// タイマーリセット
		stateTimer_ = 0;

		// 高さが一定値より上の間、落下し続ける
		if (pos_.y > FALL_FINISH_Y) {
			auto speed = FALL_SPEED;

			if (pos_.y <= FALL_FINISH_Y * 0.95f) {
				speed /= 2.0f;
			}
			if (pos_.y <= FALL_FINISH_Y * 0.975f) {
				speed /= 2.0f;
			}
			if (pos_.y <= FALL_FINISH_Y * 0.99f) {
				speed /= 2.0f;
			}

			pos_.y -= speed;
		}

		animType_ = ANIM_TYPE::DEATH;
		animControll_->Play(static_cast<int>(animType_), false);
		break;
	}


	MV1SetPosition(modelId_, pos_);
	MV1SetRotationMatrix(modelId_, GeometryDxLib::Multiplication(LOCAL_ANGLES, worldAngles_));

	// アニメーション
	animControll_->Update();
}

void Player::Draw() {
	VECTOR footPos = pos_;
	footPos.y += 2.f;
	GeometryDxLib::DrawShadow3D(footPos, 30.0f, 24);

	if (invincible_ % 2 == 1) return;

	MV1DrawModel(modelId_);

#ifdef _DEBUG
	DrawFormatString(10, 10, 0xFFFFFFU, "プレイヤー座標: (%.2f, %.2f, %.2f)", pos_.x, pos_.y, pos_.z);
#endif
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
	stateTimer_ = 0;
}

void Player::Rolling() {
	state_ = STATE::ROLLING;
	stateTimer_ = 0;
}

void Player::Over() {
	state_ = STATE::OVER;
	stateTimer_ = 0;
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

Player::STATE Player::GetState() const {
	return state_;
}

bool Player::IsInvincible() const { return invincible_; }

void Player::Move() {
	auto& ins = InputManager::GetInstance();

	VECTOR dir = {};

	/*
	dir.z -= ins.NowButton(InputManager::BUTTONS::LSTICK_U) / 1000.0f;
	dir.z += ins.NowButton(InputManager::BUTTONS::LSTICK_D) / 1000.0f;
	dir.x -= ins.NowButton(InputManager::BUTTONS::LSTICK_L) / 1000.0f;
	dir.x += ins.NowButton(InputManager::BUTTONS::LSTICK_R) / 1000.0f;
	*/

	if (ins.NowMap("移動上")) dir.z += 1.0f;
	if (ins.NowMap("移動下")) dir.z -= 1.0f;
	if (ins.NowMap("移動左")) dir.x -= 1.0f;
	if (ins.NowMap("移動右")) dir.x += 1.0f;

	dir.x = (std::min)((std::max)(dir.x, -1.0f), 1.0f);
	dir.z = (std::min)((std::max)(dir.z, -1.0f), 1.0f);

	if (dir.x != 0.0f || dir.z != 0.0f) {
		dir = VNorm(dir);
		float tempAngle = 0.0f;
		tempAngle = atan2f(dir.x, dir.z);

		worldAngles_.y = MathUtil::LerpRad(worldAngles_.y, tempAngle, 0.3f);

		animType_ = ANIM_TYPE::RUN;
	}
	else {
		animType_ = ANIM_TYPE::IDLE;
	}

	move_ = VScale(dir, MOVE_SPEED);
	pos_ = VAdd(pos_, move_);
}
