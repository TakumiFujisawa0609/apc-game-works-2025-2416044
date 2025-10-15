#include "../../Manager/InputManager.h"
#include "Pause.h"

void Pause::Update() {
	auto& ins = InputManager::GetInstance();

	if (ins.DownMap("Œˆ’è"))
		nextScene_ = SceneBase::SCENE::TITLE;
	if (ins.DownMap("–ß‚é"))
		nextScene_ = SceneBase::SCENE::GAME;
}

void Pause::Draw() {
}
