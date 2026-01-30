#pragma once
#include "../SceneBase.h"

class DemoScene : public SceneBase {
public:
	bool SystemInit() override;
	void Update() override;
	void Draw() override;
	bool Release() override;

private:
	static constexpr double MOVIE_EXTEND_RATE = 1.0 / 0.75;

	int screenID_;
	int movieID_;

};
