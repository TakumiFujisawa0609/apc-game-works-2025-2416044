#pragma once
#include <DxLib.h>
#include "../../Common/Geometry.h"

class Block {
public:
	enum class TYPE {
		PLATFORM,
		NORMAL,
		ADVANTAGE,
		FORBIDDEN,
	};

	static constexpr float BLOCK_SIZE = 200.F;
	static constexpr float HALF_BLOCK_SIZE = BLOCK_SIZE / 2.F;

	static constexpr int VANISH_FRAME = 30;

	Block();

	void Update();
	void Draw();
	void Vanish();
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
	int modelHandle_;

	Vector3 position_;
	Vector3 matrixPosition_;
	Vector3 rotation_;

	int vanishFrame_;

	bool isVanishing_;
	bool isAlive_;

	VECTOR ConvertVector3ToVECTOR(Vector3);

};
