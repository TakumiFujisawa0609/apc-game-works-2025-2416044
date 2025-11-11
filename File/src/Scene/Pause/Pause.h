#pragma once
#include "../SceneBase.h"

class Pause : public SceneBase {
public:
	bool SystemInit() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	bool Release() override;

private:
	static constexpr size_t MENU_LENGTH = 5;

	static constexpr const char* MENU_NAME[MENU_LENGTH] = {
		"çƒäJ",
		"BGMâπó ",
		"SEâπó ",
		"éãì_",
		"íÜé~"
	};

	static constexpr float MENU_X = 320;
	static constexpr float MENU_X_ADD = 300;

	static constexpr float MENU_Y = 320;
	static constexpr float MENU_Y_ADD = 60;
	static constexpr float MENU_LAYOUT_Y[] = {
		MENU_Y + MENU_Y_ADD * 0,
		MENU_Y + MENU_Y_ADD * 1,
		MENU_Y + MENU_Y_ADD * 2,
		MENU_Y + MENU_Y_ADD * 3,
		MENU_Y + MENU_Y_ADD * 4
	};

	static constexpr float MENU_CURSORBOX_X = 250.0f;
	static constexpr float MENU_CURSORBOX_Y = 49.0f;

	int cursorIndex_;
	int tempVolumeBGM_;
	int tempVolumeSE_;
	int tempCameraFollowMode_;

};
