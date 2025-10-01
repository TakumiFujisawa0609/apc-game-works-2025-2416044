#pragma once
#include "../SceneBase.h"

class Camera;
class Player;
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

	void Collision();

private:
	Camera* camera_;
	Player* player_;
	Stage* stage_;

	void CollisionCube();
	void CollisionPlatform();
	void CollisionStomp();

};
