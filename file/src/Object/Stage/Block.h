#pragma once
#include <DxLib.h>
#include "../../Common/Geometry.h"

class Block {
public:
	enum class TYPE {
		// 足場
		PLATFORM,
		// ノーマルキューブ
		NORMAL,
		// アドバンテージキューブ
		ADVANTAGE,
		// フォービドゥンキューブ
		FORBIDDEN,
	};

	enum class STATE {
		// 静止
		STOP,
		// 回転
		SPIN,
		// 消す
		VANISH,
		// 落下
		FALL,
		// 落下前警告（足場専用）
		ALERT,
	};

	static constexpr float BLOCK_SIZE = 200.F;
	static constexpr float HALF_BLOCK_SIZE = BLOCK_SIZE / 2.F;

	static constexpr unsigned __int8 VANISH_FRAME = 30;
	static constexpr unsigned __int8 FALL_FRAME = 30;
	static constexpr unsigned __int8 ALERT_FRAME = 20;

	static constexpr float VANISH_SPEED_Y = BLOCK_SIZE / VANISH_FRAME;
	static constexpr float FALL_SPEED_Y = 50.F;

	Block(int width = 1);

	void Update();
	void Draw();
	bool Release();

	void ChangeState(STATE);

	Block::STATE GetState() const;

	Block::TYPE GetType() const;
	void SetType(Block::TYPE);

	bool IsAlive() const;
	void SetAlive(bool);
	bool IsPlatform() const;

	void SetModelHandle(int);

	Vector3 GetPosition() const;
	void SetPosition(Vector3);
	Vector3 GetMatrixPosition() const;
	void SetMatrixPosition(Vector3);
	void SetRotation(Vector3);

private:
	static constexpr COLOR_F MODEL_COLOR_DEFAULT = { 0.2F, 0.2F, 0.2F, 1.F };
	static constexpr COLOR_F MODEL_COLOR_ALERT = { 0.5F, 0.2F, 0.2F, 1.F };
	static constexpr COLOR_F MODEL_COLOR_VANISH = { 0.5F, 0.5F, 0.5F, 1.F };

	static constexpr unsigned int OUTLINE_COLOR = 0x303030U;

	const unsigned __int8 WIDTH;

	TYPE type_;
	STATE state_;
	int modelHandle_;

	bool isAlive_;
	bool stateLock_;
	unsigned __int8 stateFrame_;

	Vector3 position_;
	Vector3 matrixPosition_;
	Vector3 rotation_;

	void OutLine(Vector3 position);
	Vector3 LineMatrixProc(Vector3);

};
