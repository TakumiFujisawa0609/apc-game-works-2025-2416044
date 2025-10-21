#pragma once
#include "../SceneBase.h"

class SceneManager;

class Camera;
class Player;
class Stage;
class Trap;

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

	Trap* GetTrapPtr();


private:
	SceneManager* sceneMana_;

	Camera* camera_;
	Player* player_;
	Stage* stage_;
	Trap* trap_;

	void CollisionCube();
	void CollisionPlatform();
	void CollisionStomp();

};
