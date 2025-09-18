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
	};

	static constexpr float BLOCK_SIZE = 200.F;
	static constexpr float HALF_BLOCK_SIZE = BLOCK_SIZE / 2.F;

	static constexpr int VANISH_FRAME = 30;
	static constexpr int FALL_FRAME = 30;

	static constexpr float VANISH_SPEED_Y = BLOCK_SIZE / VANISH_FRAME;
	static constexpr float FALL_SPEED_Y = 75.F;

	Block();

	void Update();
	void Draw();
	void ChangeState(STATE);
	bool Release();

	Block::TYPE GetType() const;
	void SetType(Block::TYPE);

	bool IsAlive() const;
	void SetAlive(bool);

	void SetModelHandle(int);

	Vector3 GetPosition() const;
	void SetPosition(Vector3);
	Vector3 GetMatrixPosition() const;
	void SetMatrixPosition(Vector3);
	void SetRotation(Vector3);

private:
	TYPE type_;
	STATE state_;
	int modelHandle_;

	Vector3 position_;
	Vector3 matrixPosition_;
	Vector3 rotation_;

	bool stateLock_;
	int stateFrame_;

	bool isAlive_;

	Vector3 LineMatrixProc(Vector3);
	VECTOR ConvertVector3ToVECTOR(Vector3);

};
