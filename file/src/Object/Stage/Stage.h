#pragma once
#include <list>
#include <vector>
#include "../../Common/Geometry.h"

class Block;

class Stage {
public:
	static constexpr int SPIN_FRAME = 60;
	static constexpr float SPIN_DEGREE = 90.F / SPIN_FRAME;

	static constexpr int DELAY_FRAME = 30;
	static constexpr int EXTRA_DELAY_FRAME = DELAY_FRAME * 2;

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

	void GetPlatformSize(int& x, int& z) const;

private:
	std::list<Block*> platformList_;
	std::list<std::list<std::list<Block*>>> cubeList_;

	int cubeWidth_;
	int cubeDepth_;
	int platformDepth_;
	int wave_;
	unsigned __int8 fallCount_;

	int spinTimer_;
	int delayTimer_;
	float totalRotate_;

	int normalModel_;
	int advantageModel_;
	int forbiddenModel_;

};
