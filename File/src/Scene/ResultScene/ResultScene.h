#pragma once
#include "../SceneBase.h"

class ResultScene : public SceneBase {
public:
	enum class SCENE {
		PAUSE = 0,

		TITLE,
		GAME,
		SETTING,

	};

	bool SystemInit() override;
	bool GameInit() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	bool Release() override;

private:
	int timer_;

};

