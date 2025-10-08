#pragma once
#include <list>
#include <vector>
#include <DxLib.h>
#include "../../Common/Geometry.h"

class Block;

class Stage {
public:
	static constexpr int SPIN_FRAME = 36;
	static constexpr float SPIN_DEGREE = 90.f / SPIN_FRAME;

	static constexpr float FAST_SPIN_DEGREE = 9.f;

	static constexpr int SPIN_DELAY_FRAME = 40;

	static constexpr int CUBE_WIDTH = 4;
	static constexpr int CUBE_DEPTH = 3;
	static constexpr float CUBE_FALL_RANGE_MAX = -1200;

	static constexpr int PLATFORM_WIDTH = CUBE_WIDTH;
	static constexpr int PLATFORM_HEIGHT = 3;
	static constexpr int PLATFORM_DEPTH = CUBE_DEPTH * 4;

	Stage() {}
	~Stage() {}

	bool SystemInit();
	bool GameInit();
	void Update();
	void Draw();
	bool Release();
	bool ReleaseWave();
	void VanishBlock(const Vector2& trap_stage_pos);

	void ConvertStagePos(const VECTOR& pos, int& x, int& z);
	VECTOR ConvertWorldPos(int x, int z);

	void GetPlatformSize(int& x, int& z) const;
	int GetPrevPlatformSizeZ() const;
	std::list<std::list<std::list<Block*>>> GetCubeList() const;

	void SetFastForward(bool);

	bool IsSpinning() const;

private:
	std::list<Block*> platformList_;
	std::list<std::list<std::list<Block*>>> cubeList_;

	int blockWidth_;
	int platformDepth_;
	int prevPlatformDepth_;
	int cubeDepth_;
	int phase_;
	int wave_;
	int fallCount_;

	int spinTimer_;
	bool isSpinning_;
	bool fastForward_;

	int normalModel_;
	int advantageModel_;
	int forbiddenModel_;

	void UpdateStop();
	void UpdateSpin();

};
