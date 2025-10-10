#include <DxLib.h>
#include "../../App.h"
#include "../../Manager/InputManager.h"
#include "TitleScene.h"

bool TitleScene::SystemInit() {
	return true;
}

void TitleScene::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.NowKey(KEY_INPUT_RETURN) && !ins.PrevKey(KEY_INPUT_RETURN))
		nextScene_ = SCENE::GAME;

	if (!ins.NowKey(KEY_INPUT_ESCAPE) && ins.PrevKey(KEY_INPUT_ESCAPE))
		App::GetInstance().Exit();
}

void TitleScene::Draw() {
	int x, y;
	GetWindowSize(&x, &y);

	SetFontSize(32);
	DrawString(x / 2 - 148, y / 2 - 16, "知 性 の 立 方 体", 0xFFFFFFU);
	SetFontSize(16);

	int fy = 16;
	DrawString(10, y - fy - 110, "W,A,S,D: 移動", 0xFFFFFFU);
	DrawString(10, y - fy - 90, "J      : ワナの設置／起動", 0xFFFFFFU);
	DrawString(10, y - fy - 70, "L      : 早送り", 0xFFFFFFU);
	DrawString(10, y - fy - 50, "ENTER  : ゲーム開始／離脱", 0xFFFFFFU);
	DrawString(10, y - fy - 30, "BACK   : ゲーム中ポーズ切替", 0xFFFFFFU);
	DrawString(10, y - fy - 10, "ESCAPE : ゲーム終了", 0xFFFFFFU);

	DrawString(320, y - fy - 50, "ワナの種類", 0xFFFFFFU);
	DrawString(320, y - fy - 30, "ワナ   : 設置場所の上にある立方体を消す", 0xFFFFFFU);
	DrawString(320, y - fy - 10, "ｽｰﾊﾟｰﾜﾅ: 設置場所とその周囲の上にある立方体を消す", 0xFFFFFFU);

	DrawString(800, y - fy - 70, "立方体の種類", 0xFFFFFFU);
	DrawString(800, y - fy - 50, "灰: 消すべき種類。特殊な効果は無い", 0xFFFFFFU);
	DrawString(800, y - fy - 30, "紫: 消すべき種類。消した地点にスーパーワナが設置される", 0xFFFFFFU);
	DrawString(800, y - fy - 10, "茶: 消すべきでない種類。消すと足場崩壊が1段階進む", 0xFFFFFFU);
}
