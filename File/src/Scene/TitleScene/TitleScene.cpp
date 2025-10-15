#include <DxLib.h>
#include "../../App.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/InputManager.h"
#include "TitleScene.h"

bool TitleScene::SystemInit() {
	return true;
}

void TitleScene::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定"))
		nextScene_ = SceneBase::SCENE::GAME;
	if (ins.DownMap("戻る"))
		App::GetInstance().Exit();
}

void TitleScene::Draw() {
	int x, y;
	GetWindowSize(&x, &y);

	auto& fIns = FontManager::GetInstance();
	int fl = fIns.GetFontHandle("ロゴ");
	int fs = fIns.GetFontHandle("汎用（小）");

	DrawStringToHandle(x / 2 - 278, y / 2 - 16, "知 性 の 立 方 体", 0xFFFFFFU, fl);

	int fy = 16;
	DrawStringToHandle(10, y - fy - 130, "W,A,S,D: 移動", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 110, "J      : ワナの設置／起動", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 90, "K      : スーパーワナの起動", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 70, "L      : 早送り", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 50, "ENTER  : ゲーム開始／離脱", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 30, "BACK   : ゲーム中ポーズ切替", 0xFFFFFFU, fs);
	DrawStringToHandle(10, y - fy - 10, "ESCAPE : ゲーム終了", 0xFFFFFFU, fs);

	DrawStringToHandle(320, y - fy - 50, "ワナの種類", 0xFFFFFFU, fs);
	DrawStringToHandle(320, y - fy - 30, "ワナ   : 設置場所の上にある立方体を消す", 0xFFFFFFU, fs);
	DrawStringToHandle(320, y - fy - 10, "ｽｰﾊﾟｰﾜﾅ: 設置場所とその周囲の上にある立方体を消す", 0xFFFFFFU, fs);

	DrawStringToHandle(800, y - fy - 70, "立方体の種類", 0xFFFFFFU, fs);
	DrawStringToHandle(800, y - fy - 50, "灰: 消すべき種類。特殊な効果は無い", 0xFFFFFFU, fs);
	DrawStringToHandle(800, y - fy - 30, "紫: 消すべき種類。消した地点にスーパーワナが設置される", 0xFFFFFFU, fs);
	DrawStringToHandle(800, y - fy - 10, "茶: 消すべきでない種類。消すと足場崩壊が1段階進む", 0xFFFFFFU, fs);
}
