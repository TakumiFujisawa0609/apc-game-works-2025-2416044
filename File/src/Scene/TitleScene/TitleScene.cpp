#include <DxLib.h>
#include "../../App.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Trap/Trap.h"
#include "TitleScene.h"

bool TitleScene::SystemInit() {
	subScene_ = TITLE;
	return true;
}

void TitleScene::Update() {
	switch (subScene_) {
	case TITLE:
		UpdateTitle();
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
	auto fgs = fIns.GetFontData("汎用（小）").handle;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
	DrawStringToHandle(300, 180, "Intelligent Qube", 0x404040U, fl);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);
	DrawStringToHandle(300, 180, "知 性 の 立 方 体", 0xFFFFFFU, fl);

	/*
	int fy = 16;
	DrawStringToHandle(10, y - fy - 150, "操作", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 130, "W,A,S,D: 移動", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 110, "J      : ワナの設置／起動", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 90, "K      : スーパーワナの起動", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 70, "L      : 早送り", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 50, "ENTER  : ゲーム開始／離脱", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 30, "BACK   : ゲーム中ポーズ切替", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 10, "ESCAPE : ゲーム終了", 0xFFFFFFU, fs);

	DrawStringToHandle(760, y - fy - 150, "ワナの種類", 0xFFFFFFU, fs);
	DrawStringToHandle(760, y - fy - 130, "ワナ   : 設置場所の上にある立方体を消す", 0xFFFFFFU, fs);
	DrawStringToHandle(760, y - fy - 110, "ｽｰﾊﾟｰﾜﾅ: 設置場所とその周囲の上にある立方体を消す", 0xFFFFFFU, fs);

	DrawStringToHandle(760, y - fy - 70, "立方体の種類", 0xFFFFFFU, fs);
	DrawStringToHandle(760, y - fy - 50, "灰: 消すべき種類。特殊な効果は無い", 0xFFFFFFU, fs);
	DrawStringToHandle(760, y - fy - 30, "紫: 消すべき種類。消した地点にスーパーワナが設置される", 0xFFFFFFU, fs);
	DrawStringToHandle(760, y - fy - 10, "茶: 消すべきでない種類。消すと手前側の足場が1列崩れる", 0xFFFFFFU, fs);
	*/

	switch (subScene_) {
	case TITLE:
		DrawStringToHandle((int)MENU_LAYOUT_X, (int)MENU_LAYOUT_Y[0], "PRESS ENTER", 0xFFFFFFU, fg);
		break;
	case MENU:
		for (size_t i = 0; i < MENU_LENGTH; i++) {
			DrawStringToHandle((int)MENU_LAYOUT_X, (int)MENU_LAYOUT_Y[i], MENU_NAME[i], 0xFFFFFFU, fg);

			if (cursorIndex_ == i) {
				Rect box = { { MENU_LAYOUT_X, MENU_LAYOUT_Y[i] }, { MENU_LAYOUT_X + MENU_CURSORBOX_X, MENU_LAYOUT_Y[i] + MENU_CURSORBOX_Y } };
				DrawBoxAA(box.start.x, box.start.y, box.end.x, box.end.y, 0xFF0000U, false);
			}
		}
		break;
	case SETTING:
		for (size_t i = 0; i < SETTING_LENGTH; i++) {
			DrawStringToHandle((int)MENU_LAYOUT_X, (int)MENU_LAYOUT_Y[i], SETTING_NAME[i], 0xFFFFFFU, fg);

			if (cursorIndex_ == i) {
				Rect box = { { MENU_LAYOUT_X, MENU_LAYOUT_Y[i] }, { MENU_LAYOUT_X + MENU_CURSORBOX_X, MENU_LAYOUT_Y[i] + MENU_CURSORBOX_Y } };
				DrawBoxAA(box.start.x, box.start.y, box.end.x, box.end.y, 0xFF0000U, false);
			}

			switch (i) {
			case 0:
				DrawFormatStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), (int)MENU_LAYOUT_Y[i], 0xFFFFFFU, fg, "%3d%%", tempVolumeBGM_ * 5);
				break;
			case 1:
				DrawFormatStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), (int)MENU_LAYOUT_Y[i], 0xFFFFFFU, fg, "%3d%%", tempVolumeSE_ * 5);
				break;
			case 2:
				DrawStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), (int)MENU_LAYOUT_Y[i], tempTriMarkFlag_ ? "YES" : "NO", 0xFFFFFFU, fg);
				break;
			case 3:
				DrawFormatStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), (int)MENU_LAYOUT_Y[i], 0xFFFFFFU, fg, "%d", tempSpinTimerIndex_ + 1);
				break;
			}
		}
		break;
	}
}

void TitleScene::UpdateTitle() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定") || ins.DownMap("ワナ"))
		subScene_ = MENU;
	if (ins.DownMap("戻る"))
		App::GetInstance().Quit();
}

void TitleScene::UpdateMenu() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定") || ins.DownMap("ワナ"))
		switch (cursorIndex_) {
		case 0:
			nextScene_ = SceneBase::SCENE::GAME;
			break;
		case 1:
			subScene_ = SETTING;
			cursorIndex_ = 0;
			tempVolumeBGM_ = int(AudioManager::GetInstance().GetVolumeBGM() * 20);
			tempVolumeSE_ = int(AudioManager::GetInstance().GetVolumeSE() * 20);
			tempTriMarkFlag_ = Trap::GetTriMarkFlag();
			tempSpinTimerIndex_ = Stage::GetSpinFrameIndex();
			break;
		case 2:
			App::GetInstance().Quit();
			break;
		}

	if (ins.DownMap("戻る")) {
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

void TitleScene::UpdateSetting() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("戻る")) {
		SettingEnd();
		return;
	}

	if (ins.DownMap("決定") || ins.DownMap("ワナ")) switch (cursorIndex_) {
	case 2:
		tempTriMarkFlag_ = !(tempTriMarkFlag_);
		break;
	case 4:
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

void TitleScene::SettingEnd() {
	subScene_ = MENU;
	cursorIndex_ = 0;
	Trap::SetTriMarkFlag(tempTriMarkFlag_);
	Stage::SetSpinFrameIndex(tempSpinTimerIndex_);
}
