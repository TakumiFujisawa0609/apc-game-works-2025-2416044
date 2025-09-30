#pragma once
#include <DxLib.h>

class AnimationController;

class Player {
public:
	enum class STATE {
		NORMAL,
		INVINCIBLE,
		STOMP,
	};

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

	void Stomp();

	VECTOR GetPos() const;
	void SetPos(VECTOR pos);
	VECTOR GetMove() const;

private:
	static constexpr float ANIM_SPEED = 40.F;

	static constexpr VECTOR SCALES = { 0.5F, 0.5F, 0.5F };

	static constexpr float INIT_LOCAL_POS_Z = 600.F;

	static constexpr float MOVE_SPEED = 8.F;

	AnimationController* animControll_;

	int modelId_;
	ANIM_TYPE animType_;
	VECTOR pos_;
	VECTOR move_;

	STATE state_;

	void UpdateNormal();
	void UpdateStomp();

};