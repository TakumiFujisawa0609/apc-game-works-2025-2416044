#include "../../Manager/InputManager.h"
#include "Pause.h"

void Pause::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.NowKey(KEY_INPUT_RETURN) && !ins.PrevKey(KEY_INPUT_RETURN))
		nextScene_ = SceneBase::SCENE::TITLE;
	if (ins.NowKey(KEY_INPUT_BACK) && !ins.PrevKey(KEY_INPUT_BACK))
		nextScene_ = SceneBase::SCENE::GAME;
}

void Pause::Draw() {
}
