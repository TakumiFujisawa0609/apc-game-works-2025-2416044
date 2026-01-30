#include <DxLib.h>
#include <string>
#include "../../App.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Trap/Trap.h"
#include "TitleScene.h"

bool TitleScene::SystemInit() {
	subScene_ = TITLE;
	cursorIndex_ = 0;
	tempVolumeBGM_ = 0;
	tempVolumeSE_ = 0;
	tempTriMarkFlag_ = 0;
	tempSpinTimerIndex_ = 0;
	demoScreenTimer_ = 0;

	for (size_t idx = 0; idx < _countof(pageGraphID_); idx++) {
		std::string name = "data/image/知性の立方体_企画書及び説明書_page-";
		name += std::to_string(idx + 1);
		name += ".jpg";
		pageGraphID_[idx] = LoadGraph(name.c_str());
		if (pageGraphID_[idx] == -1) return false;
	}

	return true;
}

void TitleScene::Update() {
	switch (subScene_) {
	case TITLE:
		UpdateTitle();
		break;
	case STAGE_SELECT:
		UpdateStageSelect();
		break;
	case GUIDE:
		UpdateGuide();
		break;
	case MENU:
		UpdateMenu();
		break;
	case SETTING:
		UpdateSetting();
		break;
	}
}

void TitleScene::Draw() {
	int x, y;
	GetWindowSize(&x, &y);

	auto& fIns = FontManager::GetInstance();
	auto fl = fIns.GetFontData("ロゴ").handle;
	auto fg = fIns.GetFontData("汎用").handle;

	DrawStringToHandle(int(LOGO_X), int(LOGO_Y), "知 性 の 立 方 体", 0xFFFFFFU, fl);

	switch (subScene_) {
	case TITLE:
		DrawStringToHandle((int)ENTER_X, (int)ENTER_Y, TITLE_START_NAME, 0xFFFFFFU, fg);
		break;
	case STAGE_SELECT:
		DrawStageSelect();
		break;
	case GUIDE:
		DrawGuide();
		break;
	case MENU:
		DrawMenu();
		break;
	case SETTING:
		DrawSetting();
		break;
	}
}

bool TitleScene::Release() {
	for (size_t idx = 0; idx < _countof(pageGraphID_); idx++) {
		DeleteGraph(pageGraphID_[idx]);
	}

	return true;
}

void TitleScene::UpdateTitle() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定") || ins.DownMap("ワナ")) {
		AudioManager::GetInstance().PlaySE("トラップ設置");
		demoScreenTimer_ = 0;
		subScene_ = MENU;
	}
	if (ins.DownMap("戻る")) {
		//App::GetInstance().Quit();
		nextScene_ = SceneBase::SCENE::NONE;
		return;
	}

	if (++demoScreenTimer_ >= DEMO_SCREEN_TIMER) {
		nextScene_ = SceneBase::SCENE::DEMO;
	}
}

void TitleScene::UpdateMenu() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定") || ins.DownMap("ワナ")) {
		AudioManager::GetInstance().PlaySE("トラップ設置");
		switch (cursorIndex_) {
		case 0:
			subScene_ = STAGE_SELECT;
			cursorIndex_ = 0;
			break;
		case 1:
			subScene_ = GUIDE;
			cursorIndex_ = 0;
			break;
		case 2:
			subScene_ = SETTING;
			cursorIndex_ = 0;
			tempVolumeBGM_ = int(AudioManager::GetInstance().GetVolumeBGM() * 20);
			tempVolumeSE_ = int(AudioManager::GetInstance().GetVolumeSE() * 20);
			tempTriMarkFlag_ = Trap::GetTriMarkFlag();
			tempSpinTimerIndex_ = Stage::GetSpinFrameIndex();
			break;
		case 3:
			nextScene_ = SceneBase::SCENE::NONE;
		}
	}

	if (ins.DownMap("戻る")) {
		AudioManager::GetInstance().PlaySE("トラップ起動");
		subScene_ = TITLE;
		cursorIndex_ = 0;
	}

	if (ins.DownMap("移動下")) {
		if (++cursorIndex_ >= MENU_LENGTH)
			cursorIndex_ = 0;
	}
	if (ins.DownMap("移動上")) {
		if (--cursorIndex_ < 0)
			cursorIndex_ = MENU_LENGTH - 1;
	}
}

void TitleScene::UpdateStageSelect() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定") || ins.DownMap("ワナ")) {
		AudioManager::GetInstance().PlaySE("トラップ設置");
		SceneManager::GetInstance().SetNextStartStage((unsigned int)cursorIndex_);
		nextScene_ = SceneBase::SCENE::GAME;
	}

	if (ins.DownMap("戻る")) {
		AudioManager::GetInstance().PlaySE("トラップ起動");
		subScene_ = MENU;
		cursorIndex_ = 0;
	}

	if (ins.DownMap("移動下")) {
		if (++cursorIndex_ >= STAGE_SELECT_LENGTH)
			cursorIndex_ = 0;
	}
	if (ins.DownMap("移動上")) {
		if (--cursorIndex_ < 0)
			cursorIndex_ = STAGE_SELECT_LENGTH - 1;
	}
}

void TitleScene::UpdateGuide() {
	auto& ins = InputManager::GetInstance();
	if (ins.DownMap("決定") || ins.DownMap("ワナ") || ins.DownMap("戻る")) {
		AudioManager::GetInstance().PlaySE("トラップ起動");
		subScene_ = MENU;
		cursorIndex_ = 1;
	}

	if (ins.DownMap("移動左")) {
		if (cursorIndex_ > 0)
			cursorIndex_--;
	}

	if (ins.DownMap("移動右")) {
		if (cursorIndex_ < GUIDE_PAGE_COUNT - 1)
			cursorIndex_++;
	}
}

void TitleScene::UpdateSetting() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("戻る")) {
		AudioManager::GetInstance().PlaySE("トラップ起動");
		SettingEnd();
		return;
	}

	if (ins.DownMap("決定") || ins.DownMap("ワナ")) switch (cursorIndex_) {
	case 2:
		tempTriMarkFlag_ = !(tempTriMarkFlag_);
		break;
	case 4:
		AudioManager::GetInstance().PlaySE("トラップ設置");
		SettingEnd();
		return;
	}

	if (ins.DownMap("移動左")) switch (cursorIndex_) {
	case 0:
		if (tempVolumeBGM_ > 0) {
			tempVolumeBGM_--;
			AudioManager::GetInstance().SetVolumeBGM((float)tempVolumeBGM_ / 20);
		}
		break;
	case 1:
		if (tempVolumeSE_ > 0) {
			tempVolumeSE_--;
			AudioManager::GetInstance().SetVolumeSE((float)tempVolumeSE_ / 20);
		}
		break;
	case 2:
		tempTriMarkFlag_ = !(tempTriMarkFlag_);
		break;
	case 3:
		if (tempSpinTimerIndex_ > 0)
			tempSpinTimerIndex_--;
		else
			tempSpinTimerIndex_ = Stage::SPIN_FRAME_MAX - 1;
		break;
	}
	
	if (ins.DownMap("移動右")) switch (cursorIndex_) {
	case 0:
		if (tempVolumeBGM_ < 20) {
			tempVolumeBGM_++;
			AudioManager::GetInstance().SetVolumeBGM((float)tempVolumeBGM_ / 20);
		}
		break;
	case 1:
		if (tempVolumeSE_ < 20) {
			tempVolumeSE_++;
			AudioManager::GetInstance().SetVolumeSE((float)tempVolumeSE_ / 20);
		}
		break;
	case 2:
		tempTriMarkFlag_ = !(tempTriMarkFlag_);
		break;
	case 3:
		if (tempSpinTimerIndex_ < Stage::SPIN_FRAME_MAX - 1)
			tempSpinTimerIndex_++;
		else
			tempSpinTimerIndex_ = 0;
		break;
	}

	if (ins.DownMap("移動下")) {
		if (++cursorIndex_ >= SETTING_LENGTH)
			cursorIndex_ = 0;
	}
	if (ins.DownMap("移動上")) {
		if (--cursorIndex_ < 0)
			cursorIndex_ = SETTING_LENGTH - 1;
	}
}

void TitleScene::DrawMenu() {
	auto fg = FontManager::GetInstance().GetFontData("汎用").handle;

	for (size_t i = 0; i < MENU_LENGTH; i++) {
		DrawStringToHandle((int)MENU_X, (int)MENU_LAYOUT_Y[i], MENU_NAME[i], 0xFFFFFFU, fg);

		if (cursorIndex_ == i) {
			Rect box = { { MENU_X, MENU_LAYOUT_Y[i] }, { MENU_X + MENU_CURSORBOX_X, MENU_LAYOUT_Y[i] + MENU_CURSORBOX_Y } };
			DrawBoxAA(box.start.x, box.start.y, box.end.x, box.end.y, 0xFF0000U, false);
		}
	}
}

void TitleScene::DrawStageSelect() {
	auto fg = FontManager::GetInstance().GetFontData("汎用").handle;

	for (size_t i = 0; i < STAGE_SELECT_LENGTH; i++) {
		DrawStringToHandle((int)MENU_X, (int)MENU_LAYOUT_Y[i], STAGE_SELECT_MAP[i], 0xFFFFFFU, fg);

		if (cursorIndex_ == i) {
			Rect box = { { MENU_X, MENU_LAYOUT_Y[i] }, { MENU_X + MENU_CURSORBOX_X, MENU_LAYOUT_Y[i] + MENU_CURSORBOX_Y } };
			DrawBoxAA(box.start.x, box.start.y, box.end.x, box.end.y, 0xFF0000U, false);
		}
	}
}

void TitleScene::DrawGuide() {
	auto fg = FontManager::GetInstance().GetFontData("汎用").handle;

	DrawRotaGraph(1280 / 2, 960 / 2, 0.32, 0.0, pageGraphID_[cursorIndex_], false);

	DrawFormatStringToHandle(270, 850, 0xFFFFFFU, fg, GUIDE_TEXT);
}

void TitleScene::DrawSetting() {
	auto fg = FontManager::GetInstance().GetFontData("汎用").handle;

	for (size_t i = 0; i < SETTING_LENGTH; i++) {
		DrawStringToHandle((int)MENU_X_SETTING, (int)MENU_LAYOUT_Y[i], SETTING_NAME[i], 0xFFFFFFU, fg);

		if (cursorIndex_ == i) {
			Rect box = { { MENU_X_SETTING, MENU_LAYOUT_Y[i] }, { MENU_X_SETTING + MENU_CURSORBOX_X, MENU_LAYOUT_Y[i] + MENU_CURSORBOX_Y } };
			DrawBoxAA(box.start.x, box.start.y, box.end.x, box.end.y, 0xFF0000U, false);
		}

		switch (i) {
		case 0:
			DrawFormatStringToHandle(int(MENU_X_SETTING + MENU_X_ADD), (int)MENU_LAYOUT_Y[i], 0xFFFFFFU, fg, "%3d%%", tempVolumeBGM_ * 5);
			break;
		case 1:
			DrawFormatStringToHandle(int(MENU_X_SETTING + MENU_X_ADD), (int)MENU_LAYOUT_Y[i], 0xFFFFFFU, fg, "%3d%%", tempVolumeSE_ * 5);
			break;
		case 2:
			DrawStringToHandle(int(MENU_X_SETTING + MENU_X_ADD), (int)MENU_LAYOUT_Y[i], tempTriMarkFlag_ ? "YES" : "NO", 0xFFFFFFU, fg);
			break;
		case 3:
			DrawFormatStringToHandle(int(MENU_X_SETTING + MENU_X_ADD), (int)MENU_LAYOUT_Y[i], 0xFFFFFFU, fg, "%d", tempSpinTimerIndex_ + 1);
			break;
		}
	}
}

void TitleScene::SettingEnd() {
	subScene_ = MENU;
	cursorIndex_ = 2;
	Trap::SetTriMarkFlag(tempTriMarkFlag_);
	Stage::SetSpinFrameIndex(tempSpinTimerIndex_);
}
