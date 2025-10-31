#include <DxLib.h>
#include "../../App.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
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

	DrawStringToHandle(300, 180, "I.       Q.", 0x404040U, fl);
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
		DrawStringToHandle(x / 2 - 278, 640, "PRESS ENTER", 0xFFFFFFU, fg);
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
		for (size_t i = 0; i < MENU_LENGTH; i++) {
			DrawStringToHandle((int)MENU_LAYOUT_X, (int)MENU_LAYOUT_Y[i], SETTING_NAME[i], 0xFFFFFFU, fg);
			if (cursorIndex_ == i) {
				Rect box = { { MENU_LAYOUT_X, MENU_LAYOUT_Y[i] }, { MENU_LAYOUT_X + MENU_CURSORBOX_X, MENU_LAYOUT_Y[i] + MENU_CURSORBOX_Y } };
				DrawBoxAA(box.start.x, box.start.y, box.end.x, box.end.y, 0xFF0000U, false);
			}

			switch (i) {
			case 0:
				DrawFormatStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), (int)MENU_LAYOUT_Y[i], 0xFFFFFFU, fg, "%3d%%", tempVolume_ * 5);
				break;
			case 1:
				DrawStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), (int)MENU_LAYOUT_Y[i], tempTriMarkFlag_ ? "YES" : "NO", 0xFFFFFFU, fg);
				break;
			}
		}
		break;
	}
}

void TitleScene::UpdateTitle() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定"))
		subScene_ = MENU;
	if (ins.DownMap("戻る"))
		App::GetInstance().Quit();
}

void TitleScene::UpdateMenu() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定"))
		switch (cursorIndex_) {
		case 0:
			nextScene_ = SceneBase::SCENE::GAME;
			break;
		case 1:
			subScene_ = SETTING;
			cursorIndex_ = 0;
			tempVolume_ = int(AudioManager::GetInstance().GetVolumeSE() * 20);
			tempTriMarkFlag_ = Trap::GetTriMarkFlag();
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

	if (ins.DownMap("決定"))
		switch (cursorIndex_) {
		case 1:
			tempTriMarkFlag_ = !(tempTriMarkFlag_);
			break;
		case 2:
			subScene_ = MENU;
			cursorIndex_ = 0;
			AudioManager::GetInstance().SetVolumeBGM((float)tempVolume_ / 20);
			AudioManager::GetInstance().SetVolumeSE((float)tempVolume_ / 20);
			Trap::SetTriMarkFlag(tempTriMarkFlag_);
			break;
		}

	if (ins.DownMap("移動左"))
		switch (cursorIndex_) {
		case 0:
			if (tempVolume_ > 0)
				tempVolume_--;
			break;
		case 1:
			tempTriMarkFlag_ = !(tempTriMarkFlag_);
			break;
		}
	
	if (ins.DownMap("移動右"))
		switch (cursorIndex_) {
		case 0:
			if (tempVolume_ < 20)
				tempVolume_++;
			break;
		case 1:
			tempTriMarkFlag_ = !(tempTriMarkFlag_);
			break;
		}

	if (ins.DownMap("戻る")) {
		subScene_ = MENU;
		cursorIndex_ = 0;
		AudioManager::GetInstance().SetVolumeBGM((float)tempVolume_ / 20);
		AudioManager::GetInstance().SetVolumeSE((float)tempVolume_ / 20);
		Trap::SetTriMarkFlag(tempTriMarkFlag_);
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
