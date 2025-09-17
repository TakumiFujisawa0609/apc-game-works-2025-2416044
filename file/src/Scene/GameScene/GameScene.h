#pragma once
#include "../SceneBase.h"

class Stage;

class GameScene : public SceneBase {
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
	bool Release() override;

private:
	Stage* stage_;



};
