#include <algorithm>
#include "Block.h"

Block::Block() : isAlive_(true), isVanishing_(false) {
}

void Block::Update() {
	if (isVanishing_) {
		if (++vanishFrame_ > VANISH_FRAME) {
			isAlive_ = false;
		}
		else {
			if (type_ == TYPE::PLATFORM) {
				position_.y -= HALF_BLOCK_SIZE;
			}
			else {
				position_.y -= BLOCK_SIZE / VANISH_FRAME;
			}
		}
	}
}

void Block::Draw() {
	if (!isAlive_) return;

	MV1SetRotationXYZ(modelHandle_, ConvertVector3ToVECTOR(rotation_));
	MV1SetPosition(modelHandle_, ConvertVector3ToVECTOR(position_));

	MV1DrawModel(modelHandle_);

	if (type_ == TYPE::PLATFORM) {
		Vector3 temp = position_;
		for (int i = 0; i < 2; ++i) {
			temp.y -= BLOCK_SIZE;

			MV1SetPosition(modelHandle_, ConvertVector3ToVECTOR(temp));

			MV1DrawModel(modelHandle_);
		}
	}
}

void Block::Vanish() {
	isVanishing_ = true;
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

VECTOR Block::ConvertVector3ToVECTOR(Vector3 v) {
	return VECTOR(v.x, v.y, v.z);
}
