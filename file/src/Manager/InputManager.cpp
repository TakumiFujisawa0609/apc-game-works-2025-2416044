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
	GetPadInput(DX_INPUT_PAD1);
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

unsigned int InputManager::GetNowButtonState(BUTTONS) const {
	return 0;
}

unsigned int InputManager::GetPrevButtonState(BUTTONS) const {
	return 0;
}

bool InputManager::IsPressButton(BUTTONS b) const {
	bool now = keyInput_[static_cast<int>(b)].now || padInput_[static_cast<int>(b)].now;
	return now;
}

bool InputManager::IsDownButton(BUTTONS b) const {
	bool prev = keyInput_[static_cast<int>(b)].prev || padInput_[static_cast<int>(b)].prev;
	bool now = keyInput_[static_cast<int>(b)].now || padInput_[static_cast<int>(b)].now;
	return !prev && now;
}

bool InputManager::IsUpButton(BUTTONS b) const {
	bool prev = keyInput_[static_cast<int>(b)].prev || padInput_[static_cast<int>(b)].prev;
	bool now = keyInput_[static_cast<int>(b)].now || padInput_[static_cast<int>(b)].now;
	return prev && !now;
}

void InputManager::GetKeyInput() {
	char k[256] = {};

	GetHitKeyStateAll(k);

	for (auto& map : keyMap_) {
		keyInput_[static_cast<int>(map.first)].prev = keyInput_[static_cast<int>(map.first)].now;
		keyInput_[static_cast<int>(map.first)].now = k[map.second];
	}
}

void InputManager::GetPadInput(int pad_num) {
	DINPUT_JOYSTATE d = {};
	GetJoypadDirectInputState(pad_num, &d);

	XINPUT_STATE x = {};
	GetJoypadXInputState(pad_num, &x);

	int i = GetJoypadType(pad_num);

	prevButton_ = nowButton_;

	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_L)] = d.X < 0 ? std::abs(d.X) : 0;
	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_R)] = d.X > 0 ? std::abs(d.X) : 0;
	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_U)] = d.Y < 0 ? std::abs(d.Y) : 0;
	nowButton_[static_cast<size_t>(BUTTONS::LSTICK_D)] = d.Y > 0 ? std::abs(d.Y) : 0;

	switch (i) {
	case 1:
	case 2:
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_L)] = d.Rx < 0 ? std::abs(d.Rx) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_R)] = d.Rx > 0 ? std::abs(d.Rx) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_U)] = d.Ry < 0 ? std::abs(d.Ry) : 0;
		nowButton_[static_cast<size_t>(BUTTONS::RSTICK_D)] = d.Ry > 0 ? std::abs(d.Ry) : 0;
		break;
	}

	nowButton_[static_cast<size_t>(BUTTONS::DPAD_L)] = d.POV[0] == 4500 * 1 || d.POV[0] == 4500 * 2 || d.POV[0] == 4500 * 3 ? 1 : 0;
	nowButton_[static_cast<size_t>(BUTTONS::DPAD_R)] = d.POV[0] == 4500 * 5 || d.POV[0] == 4500 * 6 || d.POV[0] == 4500 * 7 ? 1 : 0;
	nowButton_[static_cast<size_t>(BUTTONS::DPAD_U)] = d.POV[0] == 4500 * 7 || d.POV[0] == 4500 * 0 || d.POV[0] == 4500 * 1 ? 1 : 0;
	nowButton_[static_cast<size_t>(BUTTONS::DPAD_D)] = d.POV[0] == 4500 * 3 || d.POV[0] == 4500 * 4 || d.POV[0] == 4500 * 5 ? 1 : 0;

	size_t buttonIdx = 0;
	for (; buttonIdx < 6; buttonIdx++) nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx] = d.Buttons[buttonIdx];

	switch (i) {
	default:
		for (; buttonIdx < 16; buttonIdx++) nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx] = d.Buttons[buttonIdx];

		break;
	case 1:
	case 2:
		nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx++] = x.LeftTrigger;
		nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx++] = x.RightTrigger;

		for (; buttonIdx < 16; buttonIdx++) nowButton_[static_cast<size_t>(BUTTONS::BUTTON_0) + buttonIdx] = d.Buttons[buttonIdx - 2];

		break;
	}

}
