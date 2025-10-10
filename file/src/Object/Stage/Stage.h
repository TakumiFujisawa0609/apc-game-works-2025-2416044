#pragma once
#include <array>
#include <list>
#include <string>
#include <vector>
#include <DxLib.h>
#include "../../Common/Geometry.h"

class Block;
class Trap;

class Stage {
public:
	static constexpr int SPIN_FRAME = 35;
	static constexpr float SPIN_DEGREE = 90.f / SPIN_FRAME;

	static constexpr float FAST_SPIN_DEGREE = 9.f;

	static constexpr int SPIN_DELAY_FRAME = SPIN_FRAME * 2;
	static constexpr int EXTRA_DELAY_FRAME = 90;

	static constexpr int CUBE_WIDTH = 4;
	static constexpr int CUBE_DEPTH = 3;
	static constexpr float CUBE_FALL_RANGE_MAX = -1200;

	static constexpr int PLATFORM_WIDTH = CUBE_WIDTH;
	static constexpr int PLATFORM_HEIGHT = 3;
	static constexpr int PLATFORM_DEPTH = CUBE_DEPTH * 4;

	static constexpr float STOMP_DEGREE_THRESHOLD = -70.f;

	static constexpr int CUBE_PATTERN_MAX = 5;

	Stage() {}
	~Stage() {}

	void SetTrapPointer(Trap*);

	bool SystemInit();
	bool GameInit();
	void Update();
	void Draw();
	bool Release();

	void VanishBlock(const Vector2& trap_stage_pos);

	void ConvertStagePos(const VECTOR& pos, int& x, int& z);
	VECTOR ConvertWorldPos(int x, int z);

	void GetPlatformSize(int& x, int& z) const;
	int GetPrevPlatformSizeZ() const;
	std::list<std::list<std::list<Block*>>> GetCubeList() const;

	void SetFastForward(bool);

	bool IsSpinning() const;

private:
	Trap* trap_;

	std::list<Block*> platformList_;
	std::list<std::list<std::list<Block*>>> cubeList_;
	std::vector<std::vector<std::string>> cubePattern_;
	std::list<int> stepQuota_;

	int blockWidth_;
	int platformDepth_;
	int prevPlatformDepth_;
	int cubeDepth_;
	int phase_;
	int wave_;
	int fallCount_;
	int stepCount_;

	int spinTimer_;
	int extraTimer_;
	bool nextwave_;
	bool isSpinning_;
	bool fastForward_;

	std::array<int, 3ULL> blockModels_;

	void SetUpCube();
	void LoadPattern();

	void UpdateStop();
	void UpdateSpin();

	bool ReleaseWave();

};
