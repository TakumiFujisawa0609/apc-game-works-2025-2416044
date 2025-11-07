#include <DxLib.h>
#include "../../Common/Geometry.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
#include "../../Object/Camera/Camera.h"
#include "Pause.h"

bool Pause::SystemInit() {
	cursorIndex_ = 0;

	tempVolumeBGM_ = int(AudioManager::GetInstance().GetVolumeBGM() * 20);
	tempVolumeSE_ = int(AudioManager::GetInstance().GetVolumeSE() * 20);
	tempCameraFollowMode_ = Camera::GetFollowMode();

	return true;
}

void Pause::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定") || ins.DownMap("ワナ")) switch (cursorIndex_) {
	case 0:
		nextScene_ = SceneBase::SCENE::GAME;
		break;
	case 3:
		if (tempCameraFollowMode_ > 1)
			tempCameraFollowMode_--;
		else
			tempCameraFollowMode_ = 2;
		break;
	case 4:
		nextScene_ = SceneBase::SCENE::TITLE;
		break;
	}

	if (ins.DownMap("戻る"))
		nextScene_ = SceneBase::SCENE::GAME;


	if (ins.DownMap("移動左")) switch (cursorIndex_) {
	case 1:
		if (tempVolumeBGM_ > 0) {
			tempVolumeBGM_--;
			AudioManager::GetInstance().SetVolumeBGM((float)tempVolumeBGM_ / 20);
		}
		break;
	case 2:
		if (tempVolumeSE_ > 0) {
			tempVolumeSE_--;
			AudioManager::GetInstance().SetVolumeSE((float)tempVolumeSE_ / 20);
		}
		break;
	case 3:
		if (tempCameraFollowMode_ > 1)
			tempCameraFollowMode_--;
		else
			tempCameraFollowMode_ = 2;
		break;
	}

	if (ins.DownMap("移動右")) switch (cursorIndex_) {
	case 1:
		if (tempVolumeBGM_ < 20) {
			tempVolumeBGM_++;
			AudioManager::GetInstance().SetVolumeBGM((float)tempVolumeBGM_ / 20);
		}
		break;
	case 2:
		if (tempVolumeSE_ < 20) {
			tempVolumeSE_++;
			AudioManager::GetInstance().SetVolumeSE((float)tempVolumeSE_ / 20);
		}
		break;
	case 3:
		if (tempCameraFollowMode_ < 2)
			tempCameraFollowMode_++;
		else
			tempCameraFollowMode_ = 1;
		break;
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

void Pause::Draw() {
	// ウインドウサイズ取得
	int x, y;
	GetWindowSize(&x, &y);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
	DrawFillBox(0, 0, x, y, 0x000000u);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Pause::DrawUI() {
	// ウインドウサイズ取得
	int x, y;
	GetWindowSize(&x, &y);

	Rect rect = { { 200.0f, 200.0f }, { (float)x - 200.0f, (float)y - 200.0f } };
	DrawBoxAA(rect.start.x, rect.start.y, rect.end.x, rect.end.y, 0x000000u, true);

	rect.Expand(-4);
	DrawBoxAA(rect.start.x, rect.start.y, rect.end.x, rect.end.y, 0xffffffu, true);

	rect.Expand(-4);
	DrawBoxAA(rect.start.x, rect.start.y, rect.end.x, rect.end.y, 0x000000u, true);

	auto fm = FontManager::GetInstance().GetFontData("汎用");

	for (int i = 0; i < MENU_LENGTH; i++) {
		DrawFormatStringToHandle(int(MENU_LAYOUT_X), int(MENU_LAYOUT_Y[i]), 0xFFFFFFU, fm.handle, MENU_NAME[i]);

		if (cursorIndex_ == i) {
			Rect box = { { MENU_LAYOUT_X, MENU_LAYOUT_Y[i] }, { MENU_LAYOUT_X + MENU_CURSORBOX_X, MENU_LAYOUT_Y[i] + MENU_CURSORBOX_Y } };
			DrawBoxAA(box.start.x, box.start.y, box.end.x, box.end.y, 0xFF0000U, false);
		}

		switch (i) {
		case 1:
			DrawFormatStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), int(MENU_LAYOUT_Y[i]), 0xFFFFFFU, fm.handle, "%3d%%", tempVolumeBGM_ * 5);
			break;
		case 2:
			DrawFormatStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), int(MENU_LAYOUT_Y[i]), 0xFFFFFFU, fm.handle, "%3d%%", tempVolumeSE_ * 5);
			break;
		case 3:
			DrawFormatStringToHandle(int(MENU_LAYOUT_X + MENU_LAYOUT_X_ADD), int(MENU_LAYOUT_Y[i]), 0xFFFFFFU, fm.handle, "%d", tempCameraFollowMode_);
			break;
		}
	}
}

bool Pause::Release() {
	if (nextScene_ == SCENE::GAME) 
		Camera::SetFollowMode(tempCameraFollowMode_);
	else
		Camera::SetFollowMode(1);
	return false;
}
