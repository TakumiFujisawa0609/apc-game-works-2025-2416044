#include <cmath>
#include "../../Common/GeometryDxLib.h"
#include "../Player/Player.h"
#include "../Stage/Block.h"
#include "Camera.h"

bool Camera::Init() {
	SetCameraNearFar(1.f, 50000.f);

	mode_ = MODE::FOLLOW;

	return false;
}

void Camera::BeforeDraw(int platform_size_x, int platform_size_z) {
	switch (mode_) {
	case MODE::FOLLOW:
		Follow(platform_size_x, platform_size_z);
		break;
	case MODE::FIXED_FAST:
		FixedFast(platform_size_x, platform_size_z);
		break;
	}
}

void Camera::SetFollowTarget(Player* player) { player_ = player; }

void Camera::ChangeCameraMode(MODE mode) { mode_ = mode; }

void Camera::Follow(int platform_size_x, int platform_size_z) {
	VECTOR playerPos = player_->GetPos();
	const float HALF_PLATFORM_X = platform_size_x * Block::BLOCK_SIZE / 2;

	VECTOR newAngles = {};
	newAngles.y = -DegToRad((playerPos.x - HALF_PLATFORM_X) / HALF_PLATFORM_X * 30.f);
	newAngles.x = -DegToRad(6.f + powf(playerPos.z / (platform_size_z * Block::BLOCK_SIZE) * 3.5f, 3));

	prevAngles_ = angles_;
	// 少しずつカメラを回転
	angles_ = GeometryDxLib::VLerpRad(prevAngles_, newAngles, 0.12f);

	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));

	prevTargetPos_ = targetPos_;
	// 注視点の移動
	VECTOR targetLocalRotPos = VTransform(FOLLOW_TARGET_LOCAL_POS, mat);
	VECTOR newTargetPos = VAdd(playerPos, targetLocalRotPos);
	targetPos_ = GeometryDxLib::VLerp(prevTargetPos_, newTargetPos, 1.f);

	// カメラの移動
	// 相対座標を回転させて、回転後の相対座標を取得する
	VECTOR cameraLocalRotPos = VTransform(FOLLOW_CAMERA_LOCAL_POS, mat);

	prevPos_ = pos_;
	// 相対座標からワールド座標に直して、カメラ座標とする
	VECTOR newPos = VAdd(playerPos, cameraLocalRotPos);
	pos_ = GeometryDxLib::VLerp(prevPos_, newPos, 1.f);

	// カメラの上方向を計算
	VECTOR up = VTransform({ 0.f, 1.f, 0.f }, mat);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, up);
}

void Camera::FixedFast(int platform_size_x, int platform_size_z) {
	VECTOR playerPos = player_->GetPos();

	VECTOR newAngles = {};
	newAngles.y = atan2f((float)-platform_size_x, (float)platform_size_z);

	prevAngles_ = angles_;
	// 少しずつカメラを回転
	angles_ = GeometryDxLib::VLerpRad(prevAngles_, newAngles, 0.18f);

	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));

	prevTargetPos_ = targetPos_;
	// 注視点の移動
	VECTOR newTargetPos = { platform_size_x * Block::BLOCK_SIZE / 2.f, 0.f, -platform_size_z * Block::BLOCK_SIZE / 2.f };
	targetPos_ = GeometryDxLib::VLerp(prevTargetPos_, newTargetPos, 0.12f);

	prevPos_ = pos_;
	// カメラの移動
	VECTOR newPos = VTransform({ 0.f, 600.f, -platform_size_z * Block::BLOCK_SIZE * 1.25f }, mat);
	pos_ = GeometryDxLib::VLerp(prevPos_, newPos, 0.12f);

	// カメラの上方向を計算
	VECTOR up = VTransform({ 0.f, 1.f, 0.f }, mat);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, up);
}
