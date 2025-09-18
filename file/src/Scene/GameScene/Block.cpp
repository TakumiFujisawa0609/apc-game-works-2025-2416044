#include <algorithm>
#include "Stage.h"
#include "Block.h"

Block::Block() {
	type_ = TYPE::PLATFORM;
	state_ = STATE::STOP;
	stateLock_ = false;
	stateFrame_ = 0;
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
	default:
		// 何もしない
		break;
	}
}

void Block::Draw() {
	if (!isAlive_) return;

	MV1SetRotationXYZ(modelHandle_, ConvertVector3ToVECTOR(rotation_));
	MV1SetPosition(modelHandle_, ConvertVector3ToVECTOR(position_));

	MV1DrawModel(modelHandle_);

	Vector3 tPos, tPos1, tPos2;
	tPos1.x -= HALF_BLOCK_SIZE;
	tPos1.y += HALF_BLOCK_SIZE;
	tPos1.z += HALF_BLOCK_SIZE;
	tPos2.x += HALF_BLOCK_SIZE;
	tPos2.y += HALF_BLOCK_SIZE;
	tPos2.z += HALF_BLOCK_SIZE;
	DrawLine3D(ConvertVector3ToVECTOR(LineMatrixProc(tPos1)), ConvertVector3ToVECTOR(LineMatrixProc(tPos2)), 0xFFFFFFU);

	if (type_ == TYPE::PLATFORM) {
		Vector3 temp = position_;
		for (int i = 0; i < 2; ++i) {
			temp.y -= BLOCK_SIZE;

			MV1SetPosition(modelHandle_, ConvertVector3ToVECTOR(temp));

			MV1DrawModel(modelHandle_);
		}
	}
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
		// 状態をロック
		stateLock_ = true;
		break;
	default:
		// 何もしない
		break;
	}
}

bool Block::Release() {
	MV1DeleteModel(modelHandle_);
	return true;
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

void Block::SetModelHandle(int id) {
	MV1DeleteModel(modelHandle_);
	modelHandle_ = MV1DuplicateModel(id);
}

Vector3 Block::GetPosition() const {
	return position_;
}

void Block::SetPosition(Vector3 v) {
	position_ = v;
	MV1SetPosition(modelHandle_, ConvertVector3ToVECTOR(position_));
}

Vector3 Block::GetMatrixPosition() const {
	return matrixPosition_;
}

void Block::SetMatrixPosition(Vector3 v) {
	matrixPosition_ = v;
}

void Block::SetRotation(Vector3 v) {
	rotation_ = v;
	MV1SetRotationXYZ(modelHandle_, ConvertVector3ToVECTOR(rotation_));
}

Vector3 Block::LineMatrixProc(Vector3 v) {
	return (TranslationMatrix(position_) * RotationMatrixX(rotation_.x) * TranslationMatrix(-position_)) * v;
}

VECTOR Block::ConvertVector3ToVECTOR(Vector3 v) {
	return VECTOR(v.x, v.y, v.z);
}
