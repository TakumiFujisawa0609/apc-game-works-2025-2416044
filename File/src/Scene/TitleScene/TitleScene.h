#pragma once
#include "../../Common/Geometry.h"
#include "../SceneBase.h"

class TitleScene : public SceneBase {
public:
	enum SUB_SCENE {
		TITLE,
		GUIDE,
		MENU,
		SETTING,
	};

	bool SystemInit() override;
	void Update() override;
	void Draw() override;

private:
	static constexpr float ENTER_X = 500;
	static constexpr float ENTER_Y = 620;

	static constexpr const char* TITLE_START_NAME =
		"PRESS ENTER";

	static constexpr size_t MENU_LENGTH = 4;
	static constexpr size_t SETTING_LENGTH = 5;

	static constexpr const char* MENU_NAME[MENU_LENGTH] = {
		"ゲーム開始",
		"操作方法",
		"設定",
		"ゲーム終了"
	};

	static constexpr float MENU_X = 490;
	static constexpr float MENU_Y = 620;
	static constexpr float MENU_X_ADD = 360;
	static constexpr float MENU_Y_ADD = 60;
	static constexpr float MENU_LAYOUT_Y[] = {
		MENU_Y + MENU_Y_ADD * 0,
		MENU_Y + MENU_Y_ADD * 1,
		MENU_Y + MENU_Y_ADD * 2,
		MENU_Y + MENU_Y_ADD * 3,
		MENU_Y + MENU_Y_ADD * 4
	};

	static constexpr const char* GUIDE_NAME_KEY[] = {
		"  キーボード",
		"   W/A/S/D",
		"SPACE, ENTER",
		"三角マーカー",
		"難易度",
		"戻る"
	};

	static constexpr const char* SETTING_NAME[SETTING_LENGTH] = {
		"BGM音量",
		"SE音量",
		"三角マーカー",
		"難易度",
		"戻る"
	};

	static constexpr float MENU_X_SETTING = MENU_X - 100;

	static constexpr float MENU_CURSORBOX_X = 300.0f;
	static constexpr float MENU_CURSORBOX_Y = 49.0f;

	SUB_SCENE subScene_;
	int cursorIndex_;
	int tempVolumeBGM_;
	int tempVolumeSE_;
	bool tempTriMarkFlag_;
	int tempSpinTimerIndex_;

	void UpdateTitle();
	void UpdateMenu();
	void UpdateGuide();
	void UpdateSetting();

	void DrawMenu();
	void DrawGuide();
	void DrawSetting();

	void SettingEnd();

};
