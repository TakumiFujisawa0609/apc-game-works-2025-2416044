#include <algorithm>
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "ResultScene.h"

bool ResultScene::SystemInit() {
	return true;
}

bool ResultScene::GameInit() {
	return true;
}

void ResultScene::Update() {
	auto& ins = InputManager::GetInstance();

	if (timer_ < 180) timer_++;

	if (timer_ >= 180 && (ins.DownMap("決定") || ins.DownMap("ワナ"))) {
		nextScene_ = SceneBase::SCENE::TITLE;
	}
}

void ResultScene::Draw() {
}

void ResultScene::DrawUI() {
	auto fl = FontManager::GetInstance().GetFontData("汎用（大）");

	if (timer_ >= 0) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(255 * std::clamp(timer_, 0, 60) / 60.0));
		DrawFormatStringToHandle(330, 415, 0xFFFFFFU, fl.handle, "YOUR");
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (timer_ >= 60) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(255 * std::clamp(timer_ - 60, 0, 60) / 60.0));
		DrawFormatStringToHandle(385, 415, 0xFFFFFFU, fl.handle, "INT.");
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (timer_ >= 120) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, int(255 * std::clamp(timer_ - 120, 0, 60) / 60.0));
		DrawFormatStringToHandle(450, 415, 0xFFFFFFU, fl.handle, "%d", int(SceneManager::GetInstance().GetLastScore() / 85.0));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

bool ResultScene::Release() {
	return true;
}
