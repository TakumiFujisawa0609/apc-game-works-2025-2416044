#include "../../Manager/InputManager.h"
#include "Pause.h"

void Pause::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownKey(KEY_INPUT_RETURN) || ins.DownButton(InputManager::BUTTONS::BUTTON_9))
		nextScene_ = SceneBase::SCENE::TITLE;
	if (ins.DownKey(KEY_INPUT_BACK) || ins.DownButton(InputManager::BUTTONS::BUTTON_8))
		nextScene_ = SceneBase::SCENE::GAME;
}

void Pause::Draw() {
}
