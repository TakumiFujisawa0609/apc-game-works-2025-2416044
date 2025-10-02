#include <cmath>
#include "../../Common/GeometryDxLib.h"
#include "../Stage/Block.h"
#include "Camera.h"

bool Camera::Init() {
	SetCameraNearFar(1.f, 50000.f);

	return false;
}

void Camera::BeforeDraw(const VECTOR& player_pos, int platform_size_x, int platform_size_z) {
	const float HALF_PLATFORM_X = platform_size_x * Block::BLOCK_SIZE / 2;
	VECTOR newAngles = {};
	newAngles.y = -DegToRad((player_pos.x - HALF_PLATFORM_X) / HALF_PLATFORM_X * 30.f);
	newAngles.x = -DegToRad(5.f + powf(player_pos.z / (platform_size_z * Block::BLOCK_SIZE) * 3.6f, 3));

	angles_ = GeometryDxLib::VLeap(prevAngles_, newAngles, 0.1f);

	// カメラの回転行列を作成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotX(angles_.x));
	mat = MMult(mat, MGetRotY(angles_.y));
	//mat = MMult(mat, MGetRotZ(angles_.z));

	// 注視点の移動
	const VECTOR FOLLOW_TARGET_LOCAL_POS = { 0.0f, 0.0f, 800.0f };
	VECTOR followPos = player_pos;
	VECTOR targetLocalRotPos = VTransform(FOLLOW_TARGET_LOCAL_POS, mat);
	targetPos_ = VAdd(followPos, targetLocalRotPos);

	// カメラの移動
	// 相対座標を回転させて、回転後の相対座標を取得する
	const VECTOR FOLLOW_CAMERA_LOCAL_POS = { 0.0f, 400.0f, -800.0f };
	VECTOR cameraLocalRotPos = VTransform(FOLLOW_CAMERA_LOCAL_POS, mat);

	// 相対座標からワールド座標に直して、カメラ座標とする
	pos_ = VAdd(followPos, cameraLocalRotPos);

	// カメラの上方向を計算
	VECTOR up = VTransform({ 0.f, 1.f, 0.f }, mat);

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, up);

	prevAngles_ = angles_;
}
