#pragma once
#include <DxLib.h>

class Camera {
public:
	bool Init();
	void BeforeDraw(const VECTOR& player_pos, float platform_size_x, float platform_size_z);

private:
	static constexpr VECTOR LOCAL_POS = { 0.f, 300.f, -800.f };

	VECTOR pos_;
	VECTOR targetPos_;
	VECTOR angles_;

};
