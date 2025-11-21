#pragma once

class SceneBase {
public:
	enum class SCENE {
		NONE,
		PAUSE,

		TITLE,
		GAME,
		SETTING,

	};

	// システム初期化関数
	virtual bool SystemInit() { return false; }
	// 初期化関数
	virtual bool GameInit() { return false; }
	// 更新関数
	virtual void Update() {}
	// 二次更新関数
	virtual void LateUpdate() {}
	// 描画関数
	virtual void Draw() {}
	// 画面エフェクトを無視する描画関数
	virtual void DrawUI() {}
	// 解放関数
	virtual bool Release() { return false; }

	void SetScene(SCENE);
	SCENE GetMyScene() const;
	SCENE GetNextScene() const;

protected:
	SCENE myScene_;
	SCENE nextScene_;

};
