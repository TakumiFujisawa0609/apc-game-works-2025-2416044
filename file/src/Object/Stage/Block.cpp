#include <algorithm>
#include "../../Common/GeometryDxLib.h"
#include "Stage.h"
#include "Block.h"

Block::Block(int width) : WIDTH(width) {
	type_ = TYPE::PLATFORM;
	state_ = STATE::STOP;
	stateLock_ = false;
	stateFrame_ = 0Ui8;
}

void Block::Update() {
	Matrix4x4 mat;

	switch (state_) {
	case STATE::STOP:
		// 何もしない
		break;
	case STATE::SPIN:
		// 行列計算（原点への平行移動＋回転＋座標への平行移動）
		mat = TranslationMatrix(matrixPosition_) * RotationMatrixX(DX_PI_F / 180.F * -Stage::SPIN_DEGREE) * TranslationMatrix(-matrixPosition_);

		// キューブの座標を更新
		position_ = mat * position_;

		// キューブの回転を更新
		rotation_.x += DX_PI_F / 180.F * -Stage::SPIN_DEGREE;
		break;
	case STATE::VANISH:
		// 一定フレーム経過後に生存判定を消す
		if (stateFrame_++ >= VANISH_FRAME) {
			isAlive_ = false;
		}
		else {
			position_.y -= VANISH_SPEED_Y;
		}
		break;
	case STATE::FALL:
		// 一定フレーム経過後に生存判定を消す
		if (stateFrame_++ >= FALL_FRAME) {
			isAlive_ = false;
		}
		else {
			position_.y -= FALL_SPEED_Y;
		}
		break;
	case STATE::ALERT:
		// 一定フレーム経過後に落下状態へ移行
		if (stateFrame_++ >= FALL_FRAME) {
			state_ = STATE::FALL;
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
	MV1SetPosition(modelHandle_, GeometryDxLib::Vector3ToVECTOR(position_));

	if (type_ == TYPE::PLATFORM) {
		for (int i = 0; i < WIDTH; ++i) {
			Vector3 temp = position_;
			temp.x += BLOCK_SIZE * i;
			for (int i = 0; i < 3; ++i) {
				MV1SetPosition(modelHandle_, GeometryDxLib::Vector3ToVECTOR(temp));

				OutLine(temp);
				MV1DrawModel(modelHandle_);

				temp.y -= BLOCK_SIZE;
			}
		}
	}
	else {
		OutLine(position_);
		MV1DrawModel(modelHandle_);
	}
}

bool Block::Release() {
	MV1DeleteModel(modelHandle_);
	return true;
}

void Block::ChangeState(STATE st) {
	// 状態がロックされている場合は、変更も処理もしない
	if (stateLock_) return;

	state_ = st;
	stateFrame_ = 0;

	switch (state_) {
	case STATE::STOP:
		// 行列用座標を更新
		matrixPosition_ = { 0.F, position_.y - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE };

		// 回転をリセット
		rotation_.x = 0.F;
		break;
	case STATE::SPIN:
		// 何もしない
		break;
	case STATE::VANISH:
	case STATE::FALL:
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

bool Block::IsPlatform() const {
	return type_ == TYPE::PLATFORM && (state_ == STATE::STOP || state_ == STATE::ALERT);
}

void Block::SetModelHandle(int id) {
	MV1DeleteModel(modelHandle_);
	modelHandle_ = MV1DuplicateModel(id);
	MV1SetMaterialEmiColor(modelHandle_, 0, MODEL_COLOR_DEFAULT);
}

Vector3 Block::GetPosition() const {
	return position_;
}

void Block::SetPosition(Vector3 v) {
	position_ = v;
	MV1SetPosition(modelHandle_, GeometryDxLib::Vector3ToVECTOR(position_));
}

Vector3 Block::GetMatrixPosition() const {
	return matrixPosition_;
}

void Block::SetMatrixPosition(Vector3 v) {
	matrixPosition_ = v;
}

void Block::SetRotation(Vector3 v) {
	rotation_ = v;
	MV1SetRotationXYZ(modelHandle_, GeometryDxLib::Vector3ToVECTOR(rotation_));
}

void Block::GetSquareCollisionXZ(Vector2& start, Vector2& end) {
	start = { position_.x - HALF_BLOCK_SIZE, position_.z - HALF_BLOCK_SIZE };
	end = { position_.x + HALF_BLOCK_SIZE, position_.z + HALF_BLOCK_SIZE };
}

void Block::OutLine(Vector3 position) {
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

Vector3 Block::LineMatrixProc(Vector3 v) {
	return (TranslationMatrix(position_) * RotationMatrixX(rotation_.x) * TranslationMatrix(-position_)) * v;
}
