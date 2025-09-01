#pragma once

class SceneBase {
public:
	enum class SCENE {
		TITLE = 0,
		GAME,
		SETTING,

		PAUSE = 10,
	};

	virtual bool Init();
	virtual void Update();
	virtual void Draw();
	virtual bool Release();

	SCENE GetMyScene() const;
	SCENE GetNextScene() const;

private:
	SCENE myScene_;
	SCENE nextScene_;

};
