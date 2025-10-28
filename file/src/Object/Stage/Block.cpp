#include <algorithm>
#include "../../Common/GeometryDxLib.h"
#include "Stage.h"
#include "Block.h"

Block::Block(int width) : WIDTH(width) {
	modelHandle_ = -1;
	type_ = TYPE::PLATFORM;
	state_ = STATE::NONE;
	stateLock_ = false;
	stateFrame_ = 0U;
}

void Block::Update() {
	switch (state_) {
	case STATE::RISING:
		if (--stateFrame_ == 0) state_ = STATE::WAIT;
		break;
	case STATE::ADD:
		if (--stateFrame_ == 0) state_ = STATE::NONE;
		break;
	case STATE::STOP:
	case STATE::SPIN:
		// 何もしない
		break;
	case STATE::VANISH:
		MV1SetMaterialEmiColor(modelHandle_, 0, MODEL_COLOR_VANISH);

		// 一定フレーム経過後に生存判定を消す
		if (stateFrame_++ >= VANISH_FRAME) {
			isAlive_ = false;
		}
		else {
			position_.y -= VANISH_SPEED_Y;
		}
		break;
	case STATE::FALL:
		MV1SetMaterialEmiColor(modelHandle_, 0, MODEL_COLOR_DEFAULT);

		// 一定フレーム経過後に生存判定を消す
		if (stateFrame_++ >= FALL_FRAME) {
			isAlive_ = false;
		}
		else {
			position_.y -= FALL_SPEED_Y;
		}
		break;
	case STATE::ALERT:
		MV1SetMaterialEmiColor(modelHandle_, 0, MODEL_COLOR_ALERT);

		// 一定フレーム経過後に落下状態へ移行
		if (stateFrame_++ >= FALL_FRAME) {
			state_ = STATE::FALL;
			stateFrame_ = 0;
		}
		break;
	default:
		// 何もしない
		break;
	}
}

void Block::Draw() {
	if (!isAlive_) return;

	MV1SetRotationXYZ(modelHandle_, GeometryDxLib::Vector3ToVECTOR(rotation_));

	Vector3 temp = position_;

	if (type_ == TYPE::PLATFORM) {
		if (state_ == STATE::ADD) {
			temp.z -= BLOCK_SIZE * 8.0f * ((float)stateFrame_ / ADD_FRAME);
		}

		for (unsigned int i = 0; i < WIDTH; ++i) {
			temp.x = position_.x, temp.y = position_.y;

			temp.x += BLOCK_SIZE * i;

			for (unsigned int i = 0; i < 3; ++i) {
				MV1SetPosition(modelHandle_, GeometryDxLib::Vector3ToVECTOR(temp));

				OutLine(temp);
				MV1DrawModel(modelHandle_);

				temp.y -= BLOCK_SIZE;
			}
		}
	}
	else {
		if (state_ == STATE::RISING) {
			temp.y -= BLOCK_SIZE * ((float)stateFrame_ / RISING_FRAME);

			MV1SetPosition(modelHandle_, GeometryDxLib::Vector3ToVECTOR(temp));
		}
		else {
			MV1SetPosition(modelHandle_, GeometryDxLib::Vector3ToVECTOR(position_));
		}

		OutLine(position_);
		MV1DrawModel(modelHandle_);
	}
}

bool Block::Release() {
	MV1DeleteModel(modelHandle_);

	return true;
}

void Block::ChangeState(STATE st, int frame) {
	// 状態がロックされている場合は、変更も処理もしない
	if (stateLock_) return;

	if (state_ == st) return;

	state_ = st;
	stateFrame_ = 0;

	switch (state_) {
	case STATE::RISING:
		if (frame == -1)
			stateFrame_ = RISING_FRAME;
		else
			stateFrame_ = frame;

		// 行列用座標を更新
		matrixPosition_ = { 0.f, position_.y - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE };
		break;
	case STATE::ADD:
		if (frame == -1)
			stateFrame_ = ADD_FRAME;
		else
			stateFrame_ = frame;
		break;
	case STATE::WAIT:
		rotation_.x = 0.f;

		// 行列用座標を更新
		matrixPosition_ = { 0.f, position_.y - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE };
		break;
	case STATE::STOP:
		rotation_.x = 0.f;

		// 行列用座標を更新
		matrixPosition_ = { 0.f, position_.y - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE };
		break;
	case STATE::SPIN:
		// 行列用座標を更新
		matrixPosition_ = { 0.f, position_.y - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE };
		break;
	case STATE::VANISH:
		rotation_.x = 0.f;

		// 行列用座標を更新
		matrixPosition_ = { 0.f, position_.y - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE };

		// 状態をロック
		stateLock_ = true;
		break;
	case STATE::FALL:
		// 状態をロック
		stateLock_ = true;
		break;
	case STATE::ALERT:
		// 状態をロック
		stateLock_ = true;
		break;
	default:
		// 何もしない
		break;
	}
}

Block::STATE Block::GetState() const {
	return state_;
}

Block::TYPE Block::GetType() const {
	return type_;
}

void Block::SetType(Block::TYPE t) {
	type_ = t;
}

bool Block::IsAlive() const {
	return isAlive_;
}

void Block::SetAlive(bool flag) {
	isAlive_ = flag;
}

bool Block::IsActiveCube() const {
	return type_ != TYPE::PLATFORM && (state_ == STATE::STOP || state_ == STATE::SPIN);
}

bool Block::IsSteppable() const {
	return type_ == TYPE::PLATFORM && (state_ == STATE::STOP || state_ == STATE::ALERT);
}

int Block::GetModelHandle() const {
	return modelHandle_;
}

void Block::SetModelHandle(int id) {
	Release();

	modelHandle_ = MV1DuplicateModel(id);
	MV1SetMaterialEmiColor(modelHandle_, 0, MODEL_COLOR_DEFAULT);
}

Vector2 Block::GetStageIndex() const {
	return stageIndex_;
}

void Block::SetStageIndex(int x, int z) {
	stageIndex_ = { (float)x, (float)z };
}

void Block::SetStageIndex(Vector2 s) {
	stageIndex_ = s;
}

Vector3 Block::GetPosition() const {
	return position_;
}

void Block::SetPosition(Vector3 v) {
	position_ = v;
	MV1SetPosition(modelHandle_, GeometryDxLib::Vector3ToVECTOR(position_));
}

VECTOR Block::GetMinPos() const {
	return VECTOR(position_.x - HALF_BLOCK_SIZE, position_.y - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE);
}

VECTOR Block::GetMaxPos() const {
	return VECTOR(position_.x + HALF_BLOCK_SIZE, position_.y + HALF_BLOCK_SIZE, position_.z + HALF_BLOCK_SIZE);
}

Vector3 Block::GetMatrixPosition() const {
	return matrixPosition_;
}

void Block::SetMatrixPosition(Vector3 v) {
	matrixPosition_ = v;
}

Vector3 Block::GetRotation() const {
	return rotation_;
}

void Block::SetRotation(Vector3 v) {
	rotation_ = v;
	MV1SetRotationXYZ(modelHandle_, GeometryDxLib::Vector3ToVECTOR(rotation_));
}

void Block::OutLine(Vector3 position) {
	// 回転行列
	MATRIX m = GeometryDxLib::GetMatrixRotateXYZ(GeometryDxLib::Vector3ToVECTOR(rotation_));

	// 前左上
	FLOAT3 pos1 = { -HALF_BLOCK_SIZE, HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE };
	// 前右上
	FLOAT3 pos2 = { HALF_BLOCK_SIZE, HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE };
	// 前右下
	FLOAT3 pos3 = { HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE };
	// 前左下
	FLOAT3 pos4 = { -HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE };
	// 奥左上
	FLOAT3 pos5 = { -HALF_BLOCK_SIZE, HALF_BLOCK_SIZE, HALF_BLOCK_SIZE };
	// 奥右上
	FLOAT3 pos6 = { HALF_BLOCK_SIZE, HALF_BLOCK_SIZE, HALF_BLOCK_SIZE };
	// 奥右下
	FLOAT3 pos7 = { HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE, HALF_BLOCK_SIZE };
	// 奥左下
	FLOAT3 pos8 = { -HALF_BLOCK_SIZE, -HALF_BLOCK_SIZE, HALF_BLOCK_SIZE };

	// 上昇中のみ特別に処理する
	if (state_ == STATE::RISING) {
		position.y -= BLOCK_SIZE * ((float)stateFrame_ / RISING_FRAME);
	}

	// ベクトルの回転
	pos1 = VAdd(VTransform(pos1, m), GeometryDxLib::Vector3ToVECTOR(position));
	pos2 = VAdd(VTransform(pos2, m), GeometryDxLib::Vector3ToVECTOR(position));
	pos3 = VAdd(VTransform(pos3, m), GeometryDxLib::Vector3ToVECTOR(position));
	pos4 = VAdd(VTransform(pos4, m), GeometryDxLib::Vector3ToVECTOR(position));
	pos5 = VAdd(VTransform(pos5, m), GeometryDxLib::Vector3ToVECTOR(position));
	pos6 = VAdd(VTransform(pos6, m), GeometryDxLib::Vector3ToVECTOR(position));
	pos7 = VAdd(VTransform(pos7, m), GeometryDxLib::Vector3ToVECTOR(position));
	pos8 = VAdd(VTransform(pos8, m), GeometryDxLib::Vector3ToVECTOR(position));

	// X軸（左右）
	DrawLine3D(pos1, pos2, OUTLINE_COLOR);
	DrawLine3D(pos4, pos3, OUTLINE_COLOR);
	DrawLine3D(pos5, pos6, OUTLINE_COLOR);
	DrawLine3D(pos7, pos8, OUTLINE_COLOR);

	// Y軸（上下）
	DrawLine3D(pos1, pos4, OUTLINE_COLOR);
	DrawLine3D(pos2, pos3, OUTLINE_COLOR);
	DrawLine3D(pos5, pos8, OUTLINE_COLOR);
	DrawLine3D(pos6, pos7, OUTLINE_COLOR);

	// Z軸（前後）
	DrawLine3D(pos1, pos5, OUTLINE_COLOR);
	DrawLine3D(pos2, pos6, OUTLINE_COLOR);
	DrawLine3D(pos3, pos7, OUTLINE_COLOR);
	DrawLine3D(pos4, pos8, OUTLINE_COLOR);
}
