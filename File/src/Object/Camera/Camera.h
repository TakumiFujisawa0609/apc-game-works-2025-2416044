#pragma once
#include <DxLib.h>

class Player;

class Camera {
public:
	enum class MODE {
		FOLLOW,
		FIXED_PERFECT,
		FIXED_CLEAR,
		FIXED_FAST,
		FIXED_OVER,
	};

	bool Init();
	void BeforeDraw(int platform_size_x, int platform_size_z);

	void SetFollowTarget(Player* player);
	void ChangeCameraMode(MODE mode);

private:
	static constexpr VECTOR FOLLOW_TARGET_LOCAL_POS = { 0.0f, 0.0f, 800.0f };

	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_POS = { 0.0f, 400.0f, -800.0f };

	Player* player_;

	MODE mode_;
	VECTOR pos_;
	VECTOR prevPos_;
	VECTOR targetPos_;
	VECTOR prevTargetPos_;
	VECTOR angles_;
	VECTOR prevAngles_;

	void Follow(int platform_size_x, int platform_size_z);
	void FixedFast(int platform_size_x, int platform_size_z);
};
