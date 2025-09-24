#pragma once
#include <DxLib.h>

class AnimationController;

class Player {
public:
	enum class ANIM_TYPE {
		DEATH = 0,
		IDLE = 3,
		RUN = 11,
	};

	bool SystemInit();
	bool GameInit(int stage_width, int stage_depth);
	void Update();
	void Draw();
	bool Release();



private:
	static constexpr VECTOR SCALES = { 0.5F, 0.5F, 0.5F };

	static constexpr float INIT_LOCAL_POS_Z = 600.F;

	static constexpr float MOVE_SPEED = 6.F;

	AnimationController* animControll_;

	int modelId_;
	ANIM_TYPE animType_;
	VECTOR pos_;

};