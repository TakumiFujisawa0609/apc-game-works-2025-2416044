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

	static constexpr unsigned int SCORE_MAX = 9999999;
	static constexpr unsigned int SCORE_MAX_DIGIT = 7;

	bool SystemInit() override;
	bool GameInit() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	bool Release() override;

	void Collision();

	Camera* GetCameraPtr();
	Trap* GetTrapPtr();

	unsigned int GetScore() const;
	void AddScore(int);
	unsigned int GetIQ() const;
	void AddIQ(int);

private:
	Camera* camera_;
	Player* player_;
	Stage* stage_;
	Trap* trap_;

	unsigned int score_;
	unsigned int iq_;

	void CollisionCube();
	void CollisionPlatform();
	void CollisionStomp();

};
