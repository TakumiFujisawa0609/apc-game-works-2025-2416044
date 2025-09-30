#pragma once
#include <DxLib.h>
#include "../../Common/Geometry.h"

class Block {
public:
	enum class TYPE {
		// ノーマルキューブ
		NORMAL,
		// アドバンテージキューブ
		ADVANTAGE,
		// フォービドゥンキューブ
		FORBIDDEN,
		// 足場
		PLATFORM,
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

	static constexpr float BLOCK_SIZE = 200.f;
	static constexpr float HALF_BLOCK_SIZE = BLOCK_SIZE / 2.f;

	static constexpr unsigned int VANISH_FRAME = 30;
	static constexpr unsigned int FALL_FRAME = 30;
	static constexpr unsigned int ALERT_FRAME = 20;

	static constexpr float VANISH_SPEED_Y = BLOCK_SIZE / VANISH_FRAME;
	static constexpr float FALL_SPEED_Y = 50.f;

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

	bool IsActiveCube() const;
	bool IsSteppable() const;

	int GetModelHandle() const;
	void SetModelHandle(int);

	Vector3 GetPosition() const;
	void SetPosition(Vector3);

	VECTOR GetMinPos() const;
	VECTOR GetMaxPos() const;

	Vector3 GetMatrixPosition() const;
	void SetMatrixPosition(Vector3);
	
	Vector3 GetRotation() const;
	void SetRotation(Vector3);

	void GetSquareCollisionXZ(Vector2& start, Vector2& end);

private:
	static constexpr COLOR_F MODEL_COLOR_DEFAULT = { 0.2F, 0.2F, 0.2F, 1.f };
	static constexpr COLOR_F MODEL_COLOR_ALERT = { 0.5F, 0.2F, 0.2F, 1.f };
	static constexpr COLOR_F MODEL_COLOR_VANISH = { 0.5F, 0.5F, 0.5F, 1.f };

	static constexpr unsigned int OUTLINE_COLOR = 0x303030U;

	const unsigned int WIDTH;

	TYPE type_;
	STATE state_;
	int modelHandle_;

	bool isAlive_;
	bool stateLock_;
	unsigned int stateFrame_;

	Vector3 position_;
	Vector3 matrixPosition_;
	Vector3 rotation_;

	void OutLine(Vector3 position);
	Vector3 LineMatrixProc(Vector3);

};
