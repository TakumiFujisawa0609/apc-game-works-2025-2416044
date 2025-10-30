#include <DxLib.h>
#include "../../App.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
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
	}
}

void TitleScene::Draw() {
	int x, y;
	GetWindowSize(&x, &y);

	auto& fIns = FontManager::GetInstance();
	auto fl = fIns.GetFontData("ロゴ").handle;
	auto fg = fIns.GetFontData("汎用").handle;
	auto fgs = fIns.GetFontData("汎用（小）").handle;

	DrawStringToHandle(x / 2 - 278, 240, "知 性 の 立 方 体", 0xFFFFFFU, fl);

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
		size_t i = 0;
		DrawStringToHandle(MENU_LAYOUT[i++].x, MENU_LAYOUT[i].y, MENU_NAME[i], 0xFFFFFFU, fg);
		DrawStringToHandle(MENU_LAYOUT[i++].x, MENU_LAYOUT[i].y, MENU_NAME[i], 0xFFFFFFU, fg);
		DrawStringToHandle(MENU_LAYOUT[i++].x, MENU_LAYOUT[i].y, MENU_NAME[i], 0xFFFFFFU, fg);
		break;
	}
}

void TitleScene::UpdateTitle() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定"))
		subScene_ = MENU;
	if (ins.DownMap("戻る"))
		App::GetInstance().Exit();
}

void TitleScene::UpdateMenu() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定"))
		nextScene_ = SceneBase::SCENE::GAME;
	if (ins.DownMap("戻る"))
		subScene_ = TITLE;
}
