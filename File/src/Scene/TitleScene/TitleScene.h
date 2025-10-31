#pragma once
#include "../../Common/Geometry.h"
#include "../SceneBase.h"

class TitleScene : public SceneBase {
public:
	enum SUB_SCENE {
		TITLE,
		MENU,
		SETTING,
	};

	bool SystemInit() override;
	void Update() override;
	void Draw() override;

private:
	static constexpr size_t MENU_LENGTH = 3;
	static constexpr size_t SETTING_LENGTH = 3;

	static constexpr const char* MENU_NAME[MENU_LENGTH] = {
		"ゲーム開始",
		"設定",
		"ゲーム終了"
	};

	static constexpr float MENU_LAYOUT_X = 420;
	static constexpr float MENU_LAYOUT_X_ADD = 360;
	static constexpr float MENU_LAYOUT_Y[] = {
		640, 700, 760, 820, 880
	};

	static constexpr const char* SETTING_NAME[SETTING_LENGTH] = {
		"音量",
		"三角マーカー",
		"戻る"
	};

	static constexpr float MENU_CURSORBOX_X = 300.0f;
	static constexpr float MENU_CURSORBOX_Y = 49.0f;

	SUB_SCENE subScene_;
	int cursorIndex_;
	int tempVolume_;
	bool tempTriMarkFlag_;

	void UpdateTitle();
	void UpdateMenu();
	void UpdateSetting();

};
