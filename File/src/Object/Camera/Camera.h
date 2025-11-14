#pragma once
#include <DxLib.h>

class Player;

class Camera {
public:
	enum class MODE {
		FOLLOW,
		FIXED_PERFECT,
		FIXED_FAST,
		FIXED_CLEAR,
		FIXED_OVER,
	};

	bool Init();
	void BeforeDraw(int platform_size_x, int platform_size_z, int timer = -1);

	void SetFollowTarget(Player* player);
	MODE GetCameraMode() const;
	void ChangeCameraMode(MODE mode);
	static int GetFollowMode();
	static void SetFollowMode(int);

private:
	static constexpr VECTOR FOLLOW_TARGET_LOCAL_POS = { 0.0f, 0.0f, 800.0f };
	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_POS = { 0.0f, 450.0f, -900.0f };

	static constexpr VECTOR FOLLOW_TARGET_LOCAL_POS2 = { 0.0f, 0.0f, 350.0f };
	static constexpr VECTOR FOLLOW_CAMERA_LOCAL_POS2 = { 0.0f, 1400.0f, 150.0f };

	static constexpr VECTOR FIXED_OVER_TARGET_LOCAL_POS = { 0.0f, 150.0f, 0.0f };
	static constexpr VECTOR FIXED_OVER_CAMERA_LOCAL_POS = { 300.0f, 300.0f, -400.0f };
	static constexpr VECTOR FIXED_OVER_TARGET_LOCAL_POS2 = { -400.0f, 0.0f, 600.0f };
	static constexpr VECTOR FIXED_OVER_CAMERA_LOCAL_POS2 = { 400.0f, 0.0f, -600.0f };

	Player* player_;

	MODE mode_;
	VECTOR pos_;
	VECTOR prevPos_;
	VECTOR targetPos_;
	VECTOR prevTargetPos_;
	VECTOR angles_;
	VECTOR prevAngles_;
	static int followMode_;

	void Follow(int platform_size_x, int platform_size_z);
	void Follow2(int platform_size_x, int platform_size_z);
	void FixedFast(int platform_size_x, int platform_size_z);
	void FixedClear(int platform_size_x, int platform_size_z, int timer);
	void FixedOver();
};
