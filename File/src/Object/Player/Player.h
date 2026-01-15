#pragma once
#include <DxLib.h>

class AnimationController;

class Player {
public:
	enum class STATE {
		// 通常
		NORMAL,
		// 体勢が崩れてゴロゴロ
		ROLLING,
		// ぺしゃんこ
		STOMP,
		// ゲームオーバー
		OVER,
	};

	enum class ANIM_TYPE {
		DEATH = 0,
		IDLE = 3,
		RUN = 11,
	};

	static constexpr float FALL_FINISH_Y = -8000.0f;

	bool SystemInit();
	bool GameInit(int stage_width, int stage_depth);
	void Update();
	void UpdateClear();
	void Draw();
	bool Release();

	void Stomp();
	void Rolling();
	void Over();

	VECTOR GetPos() const;
	void SetPos(VECTOR pos);
	VECTOR GetMove() const;

	STATE GetState() const;
	bool IsInvincible() const;

private:
	// アニメーション速度
	static constexpr float ANIM_SPEED = 40.0f;
	// 自己発光
	static constexpr COLOR_F MODEL_EMI_COLOR = { 0.2F, 0.2F, 0.2F, 1.0f };
	// 大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };
	// ローカル角度
	static constexpr VECTOR LOCAL_ANGLES = { 0.0f, DX_PI_F, 0.0f };
	// 移動速度
	static constexpr float MOVE_SPEED = 9.0f;
	// ローリング速度
	static constexpr float ROLLING_SPEED = 200.0f / 5;
	// 踏み潰され状態の時間
	static constexpr int STATE_STOMP_TIME = 150;
	// 無敵状態の時間
	static constexpr int INVINCIBLE_TIME = 120;
	// 落下速度
	static constexpr float FALL_SPEED = 40.0f;

	AnimationController* animControll_;

	int modelId_;
	ANIM_TYPE animType_;

	STATE state_;
	int stateTimer_;

	int invincible_;
	VECTOR pos_;
	VECTOR worldAngles_;
	VECTOR move_;

	void Move();

};