#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance() {
	// instance_ 変数が空ポインタならインスタンスを生成
	if (instance_ == nullptr) instance_ = new InputManager;
}

InputManager& InputManager::GetInstance() {
	return *instance_;
}

bool InputManager::Init() {
	return true;
}

void InputManager::Update() {
	GetKeyInput();

	int num = GetJoypadNum();
	for (int i = DX_INPUT_PAD1; i <= num || i <= DX_INPUT_PAD4; i++) {
		GetPadInput(i);
	}
}

bool InputManager::Release() {
	buttonMap_.clear();

	return true;
}

void InputManager::AddButtonMap(const char* name, BUTTONS padMap, std::array<int, 2> keyMap) {
	buttonMap_.emplace(name, BUTTON_MAP(padMap, keyMap));
}

void InputManager::ReplacePadMap(const char* name, BUTTONS replace) {
	for (auto& pad : buttonMap_) if (name == pad.first) {
		pad.second.padMap = replace;
		return;
	}
}

void InputManager::ReplaceKeyMap(const char* name, std::array<int, 2> replace) {
	for (auto& pad : buttonMap_) if (name == pad.first) {
		pad.second.keyMap = replace;
		return;
	}
}

void InputManager::ReplaceKeyMap(const char* name, int replace, size_t index) {
	for (auto& pad : buttonMap_) if (name == pad.first) {
		pad.second.keyMap[index] = replace;
		return;
	}
}

int InputManager::GetNowButtonState(BUTTONS) const {
	return 0;
}

int InputManager::GetPrevButtonState(BUTTONS) const {
	return 0;
}

void InputManager::GetKeyInput() {
	char k[256] = {};

	GetHitKeyStateAll(k);
}

void InputManager::GetPadInput(int pad_num) {
	int n = GetJoypadButtonNum(pad_num);

	int t = GetJoypadType(pad_num);

	DINPUT_JOYSTATE dIn = {};
	GetJoypadDirectInputState(pad_num, &dIn);

	bool xFlg = CheckJoypadXInput(pad_num) ? true : false;
	XINPUT_STATE xIn = {};
	if (xFlg) GetJoypadXInputState(pad_num, &xIn);

	prevButton_ = nowButton_;

	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_L)] = dIn.X < 0 ? std::abs(dIn.X) : 0;
	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_R)] = dIn.X > 0 ? std::abs(dIn.X) : 0;
	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_U)] = dIn.Y < 0 ? std::abs(dIn.Y) : 0;
	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_D)] = dIn.Y > 0 ? std::abs(dIn.Y) : 0;

	switch (t) {
	case 0:
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_L)] = dIn.Z < 0 ? std::abs(dIn.Z) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_R)] = dIn.Z > 0 ? std::abs(dIn.Z) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_U)] = dIn.Rz < 0 ? std::abs(dIn.Rz) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_D)] = dIn.Rz > 0 ? std::abs(dIn.Rz) : 0;
		break;
	case 1:
	case 2:
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_L)] = dIn.Rx < 0 ? std::abs(dIn.Rx) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_R)] = dIn.Rx > 0 ? std::abs(dIn.Rx) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_U)] = dIn.Ry < 0 ? std::abs(dIn.Ry) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_D)] = dIn.Ry > 0 ? std::abs(dIn.Ry) : 0;
		break;
	}

	nowButton_[static_cast<size_t>(BUTTONS::DPAD_L)] = dIn.POV[0] == 4500 * 1 || dIn.POV[0] == 4500 * 2 || dIn.POV[0] == 4500 * 3 ? 1 : 0;
	nowButton_[static_cast<size_t>(BUTTONS::DPAD_R)] = dIn.POV[0] == 4500 * 5 || dIn.POV[0] == 4500 * 6 || dIn.POV[0] == 4500 * 7 ? 1 : 0;
	nowButton_[static_cast<size_t>(BUTTONS::DPAD_U)] = dIn.POV[0] == 4500 * 7 || dIn.POV[0] == 4500 * 0 || dIn.POV[0] == 4500 * 1 ? 1 : 0;
	nowButton_[static_cast<size_t>(BUTTONS::DPAD_D)] = dIn.POV[0] == 4500 * 3 || dIn.POV[0] == 4500 * 4 || dIn.POV[0] == 4500 * 5 ? 1 : 0;

	size_t buttonIdx = 0;
	for (; buttonIdx < 6; buttonIdx++) nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx] = dIn.Buttons[buttonIdx];

	switch (t) {
	default:
		for (; buttonIdx < 16; buttonIdx++) nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx] = dIn.Buttons[buttonIdx];

		break;
	case 1:
	case 2:
		nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx++] = xIn.LeftTrigger;
		nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx++] = xIn.RightTrigger;

		for (; buttonIdx < 16; buttonIdx++) nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx] = dIn.Buttons[buttonIdx - 2];

		break;
	}

}
