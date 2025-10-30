#pragma once
#include "../../Common/Geometry.h"
#include "../SceneBase.h"

class TitleScene : public SceneBase {
public:
	enum SUB_SCENE {
		TITLE,
		MENU,
	};

	bool SystemInit() override;
	void Update() override;
	void Draw() override;

private:
	static constexpr size_t MENU_LENGTH = 3;

	static constexpr const char* MENU_NAME[MENU_LENGTH] = {
		"ÉQÅ[ÉÄäJén",
		"ê›íË",
		"ñﬂÇÈ"
	};

	const Vector2 MENU_LAYOUT[MENU_LENGTH] = {
		{ 400, 640 },
		{ 400, 700 },
		{ 400, 760 }
	};

	SUB_SCENE subScene_;

	void UpdateTitle();
	void UpdateMenu();

};
