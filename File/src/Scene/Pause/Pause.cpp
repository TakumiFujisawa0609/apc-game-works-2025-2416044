#include <DxLib.h>
#include "../../Common/Geometry.h"
#include "../../Manager/InputManager.h"
#include "Pause.h"

void Pause::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("決定"))
		nextScene_ = SceneBase::SCENE::GAME;
	if (ins.DownMap("戻る"))
		nextScene_ = SceneBase::SCENE::TITLE;
}

void Pause::Draw() {
	// ウインドウサイズ取得
	int x, y;
	GetWindowSize(&x, &y);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
	DrawFillBox(0, 0, x, y, 0x000000u);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Rect rect = { { 200.0f, 200.0f }, { (float)x - 200.0f, (float)y - 200.0f } };
	DrawBoxAA(rect.start.x, rect.start.y, rect.end.x, rect.end.y, 0x000000u, true);

	rect.Expand(-4);
	DrawBoxAA(rect.start.x, rect.start.y, rect.end.x, rect.end.y, 0xffffffu, true);

	rect.Expand(-4);
	DrawBoxAA(rect.start.x, rect.start.y, rect.end.x, rect.end.y, 0x000000u, true);
}
