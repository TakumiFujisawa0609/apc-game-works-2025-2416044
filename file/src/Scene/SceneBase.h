#pragma once

class SceneBase {
public:
	enum class SCENE {
		PAUSE = 0,

		TITLE,
		GAME,
		SETTING,

	};

	virtual bool SystemInit() { return false; }
	virtual bool GameInit() { return false; }
	virtual void Update() {}
	virtual void Draw() {}
	virtual bool Release() { return false; }

	void SetScene(SCENE);
	SCENE GetMyScene() const;
	SCENE GetNextScene() const;

protected:
	SCENE myScene_;
	SCENE nextScene_;

};
