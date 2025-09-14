#pragma once
#include <vector>

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


	std::vector<std::vector<Cube>> cubes_;
	int spinTimer_;
	int delayTimer_;

	int normalImg_;
	int advantageImg_;
	int forbiddenImg_;

};
