#pragma once
#include <list>

class Cube;

class Stage {
public:
	Stage() {}
	~Stage() {}

	bool SystemInit();
	bool GameInit();
	void Update();
	void Draw();
	bool Release();

private:
	static constexpr int SPIN_FRAME = 30;
	static constexpr float SPIN_DEGREE = 90.F / SPIN_FRAME;

	static constexpr int DELAY_FRAME = 30;
	static constexpr int EXTRA_DELAY_FRAME = 60;

	static constexpr int CUBE_DEPTH = 3;
	static constexpr int CUBE_WIDTH = 4;
	static constexpr float CUBE_SIZE = 10.F;
	static constexpr float HALF_CUBE_SIZE = CUBE_SIZE / 2.F;
	static constexpr int PLATFORM_DEPTH = CUBE_DEPTH * 4;

	std::list<std::list<Cube*>> platformList_;
	std::list<std::list<Cube*>> cubeList_;
	int spinTimer_;
	int delayTimer_;

	// キューブ山の最前列のキューブの現在位置（奥から数える）
	int cubeDepth_;

	int normalImg_;
	int advantageImg_;
	int forbiddenImg_;

};
